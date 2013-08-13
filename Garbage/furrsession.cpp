#include "stdafx.h"
#include "furrsession.h"
#include "furrsessionmacros.h"
#include "directdrawwnd.h"

extern DirectDrawWnd g_DDDisplayWnd;
extern LoaderMap     g_LoaderMap;

/*
 * FURRSession : Public
 */
bool FURRSession::Spawn(PRECORDSESSION pRS, PSESSIONCALLBACK pSCB, DWORD_PTR dwUser) {

  bool r = false;

  // We ain't spawning another session
  // if one is in progress.
  if (!m_bRunning) {

    // Make sure we've not been passed a bunk pointer
    if (NULL != pRS) {

      // Create an event handle to signal
      // the worker thread to terminate, if needed.
      if (NULL != (m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {

        // Assemble arguments to pass to the worker thread
        PFURRSESSIONTHREAD pfst = new FURRSESSIONTHREAD;

        // Out of memory? (I sure as hell hope not...)
        if (NULL != pfst) {

          pfst->hEvent = m_hEvent;
          pfst->pRS    = pRS;
          pfst->pSCB   = pSCB;
          pfst->dwUser = dwUser;

          // Spawn the worker thread
          UINT uiThreadID;

          if (NULL != (m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, _Thread, pfst, 0, &uiThreadID)))) {

            // Viola
            m_bRunning  = true;
            r           = true;

          }

        }

      }

    }

  }

  // Don't want to leak a handle
  if (!r && (NULL != m_hEvent)) { CloseHandle(m_hEvent); }

  return r;
}

void FURRSession::Terminate(void) {

  // No need to terminate if we're not
  // even currently executing.
  if (m_bRunning) {

    // Signal the worker thread event
    SetEvent(m_hEvent);

    // Wait for the thread to exit
    WaitForSingleObject(m_hThread, INFINITE);

    // Reset running flag
    m_bRunning = false;

  }

}

/*
 * FURRSession : Protected
 */
void FURRSession::_FireOutput(PFURRSESSIONTHREAD pfst, const TCHAR *szOutputMsg, ...) {

  if (NULL != pfst->pSCB) {

    va_list val;
    va_start(val, szOutputMsg);
    _PrintMsg(pfst, FSE_OUTPUT, szOutputMsg, val);

  }

}

void FURRSession::_FireProgress(PFURRSESSIONTHREAD pfst, DWORD_PTR dwStartTime, 
                                DWORD_PTR dwMaxFrames, DWORD_PTR dwCurFrame, DWORD_PTR dwAvgExposure)
{

  if (NULL != pfst->pSCB) {

    FSEPROGRESS fsep;

    fsep.dwCurFrame = dwCurFrame;
    fsep.dwMaxFrame = dwMaxFrames;
    
    _FormatMsec(GetTickCount() - dwStartTime, fsep.tsElapsedTime);
    _FormatMsec(((dwMaxFrames - dwCurFrame) * dwAvgExposure) * 1000, fsep.tsRemainingTime);

    pfst->pSCB(FSE_PROGRESS, pfst->dwUser, &fsep);

  }

}

void FURRSession::_FireWarning(PFURRSESSIONTHREAD pfst, const TCHAR *szWarningMsg, ...) {

  if (NULL != pfst->pSCB) {

    va_list val;
    va_start(val, szWarningMsg);
    _PrintMsg(pfst, FSE_WARNING, szWarningMsg, val);

  }

}

void FURRSession::_FireError(PFURRSESSIONTHREAD pfst, const TCHAR *szErrorMsg, ...) {

  if (NULL != pfst->pSCB) {

    va_list val;
    va_start(val, szErrorMsg);
    _PrintMsg(pfst, FSE_ERROR, szErrorMsg, val);

  }

}

void FURRSession::_FireAbort(PFURRSESSIONTHREAD pfst) {

  if (NULL != pfst->pSCB) {

    pfst->pSCB(FSE_ABORTED, pfst->dwUser, NULL);

  }

}

void FURRSession::_FireFinished(PFURRSESSIONTHREAD pfst) {

  if (NULL != pfst->pSCB) {

    pfst->pSCB(FSE_FINISHED, pfst->dwUser, NULL);

  }

}

void FURRSession::_PrintMsg(PFURRSESSIONTHREAD pfst, DWORD_PTR dwEvent, const TCHAR *szMsg, va_list val) {

  TCHAR szBuf[FS_MAXMSGSIZE] = {0};

  _vsntprintf(szBuf, FS_MAXMSGSIZE, szMsg, val);
  va_end(val);

  pfst->pSCB(dwEvent, pfst->dwUser, szBuf);

}

unsigned __stdcall FURRSession::_Thread(void *pVoid) {

  FURRSESSIONTHREAD fst;

  // Copy thread arguments to stack-based memory and
  // free heap-allocated version
  fst = (*reinterpret_cast<PFURRSESSIONTHREAD>(pVoid));
  delete pVoid;

  // Make note of the current tick count to use
  // in calculation of elapsed and remaining time
  DWORD dwTimeStart = GetTickCount();

  // Calculate average exposure time
  DWORD_PTR dwAvgExposure = ((fst.pRS->AP.Red.dwET + fst.pRS->AP.Green.dwET + fst.pRS->AP.Blue.dwET) / 3);

  // Rewind list of frames
  fst.pRS->Rewind();

  // Create communication object for camera driver
  CameraComm Camera;

  // Display Direct Draw window
  DDRAWWNDINIT ddwi;

  ddwi.bInitVisible     = true;
  ddwi.bPrimaryDisplay  = true;// false;
  ddwi.bWindowed        = false;
  ddwi.hEventRender     = CreateEvent(NULL, TRUE, FALSE, NULL);
 
  if (!g_DDDisplayWnd.Display(ddwi, *fst.pRS)) {

    _FireError(&fst, _T("FURRSession: Failed to initialize recording display;")
                     _T(" Win32 Error: %d"));
    TERMINATESESSION();
    
  }

  // Recording loop
  DWORD_PTR dwCurFrame  = 1UL;      // Current frame counter
  FRAME f;                          // FRAME used to iterate frames
  FURRImage *pCurImg    = NULL;     // Temporary FURRImage pointer
  FURRImage *pRAImg     = NULL;     // Read-ahead FURRImage pointer
  HANDLE hEventExpose   = NULL;     // Handle to event for exposure timer

  //
  // Open the communication port for the camera
  //
  if (!Camera.Open()) {

    _FireError(&fst, _T("FURRSession: Failed to open serial port! Win32 Error = %d"), GetLastError());
    TERMINATESESSION();

  }

  g_LoaderMap.Rewind();

  while (fst.pRS->EnumFrames(f)) {

    if (ISABORTEDSESSION()) { TERMINATESESSION(); }

    // TODO: Verify that the imgae loader thread has finished
    // loading this image
    /*IMAGELOADERTHREAD ilt;

    ilt.hEventAbort = fst.hEvent;
    ilt.hEventDone  = CreateEvent(NULL, TRUE, FALSE, NULL);
    ilt.pImg        = pCurImg;
    ilt.pRS         = fst.pRS;
    ilt.pSCB        = fst.pSCB;
    ilt.tsImageFile = f.tsFileName;
    
    if (!_SpawnImageLoaderThread(&ilt)) {

      _FireError(&fst, _T("FURRSession: Failed to spawn read-ahead thread! FATAL ERROR!"));
      TERMINATESESSION();

    } REMOVED: do read-ahead at a later time */

    if ((!g_LoaderMap.LoadImage(f.tsFileName, &pCurImg)) || (NULL == pCurImg)) {

      _FireError(&fst, _T("FURRSession: Failed to load image file '%s'!"), f.tsFileName.c_str());
      TERMINATESESSION();

    }

    if (!pCurImg->PrepareForShoot(fst.pRS->AP, fst.pRS->Annotation)) {

       _FireError(&fst, _T("FURRSession: Failed to prepare image file '%s' for shooting."), f.tsFileName.c_str());
       TERMINATESESSION();

    }

    //
    // If we're shooting a black / white image,
    // we simply display the image (no channel-splitting involved).
    //
    if (!(fst.pRS->AP.Global.Mode == FULLIMAGE)) {

      if (!g_DDDisplayWnd.DisplayImage(FIC_FULL, *pCurImg)) {

        _FireError(&fst, _T("FURRSession: Failed to display image '%s'!"),
                  f.tsFileName.c_str());

      }

      //
      // Wait for the directdraw window to render the frame
      //
      WAITFORRENDER();

      //
      // Shoot the frame
      //
      Camera.Shoot();

      //
      // Expose frame for desired time
      //
      EXPOSEFRAME(fst.pRS->AP.Red.dwET);

    } else {

// TODO: Channel-split/colorwheel

      // Display the image on the tube,
      // split up by R, G, and B channels
      if (!g_DDDisplayWnd.DisplayImage(FIC_RED, *pCurImg)) {

        _FireError(&fst, _T("FURRSession: Failed to display RED channel for image '%s'!"),
                  f.tsFileName.c_str());

      }

      //
      // Wait for the directdraw window to render the frame
      //
      WAITFORRENDER();

      Camera.Shoot();

      // Expose RED frame for specified time
      EXPOSEFRAME(fst.pRS->AP.Red.dwET);

      if (!g_DDDisplayWnd.DisplayImage(FIC_GREEN, *pCurImg)) {

        _FireError(&fst, _T("FURRSession: Failed to display GREEN channel for image '%s'!"),
                  f.tsFileName.c_str());

      }

      //
      // Wait for the directdraw window to render the frame
      //
      WAITFORRENDER();

      Camera.Shoot();

      // Expose GREEN frame for specified time
      EXPOSEFRAME(fst.pRS->AP.Green.dwET);

      if (!g_DDDisplayWnd.DisplayImage(FIC_BLUE, *pCurImg)) {

        _FireError(&fst, _T("FURRSession: Failed to display BLUE channel for image '%s'!"),
                  f.tsFileName.c_str());

      }

      //
      // Wait for the directdraw window to render the frame
      //
      WAITFORRENDER();

      Camera.Shoot();

      // Expose BLUE frame for specified time
      EXPOSEFRAME(fst.pRS->AP.Blue.dwET);

    }

    _FireProgress(&fst, dwTimeStart, fst.pRS->FrameCount(), dwCurFrame++, dwAvgExposure);

    delete pCurImg;
  }


  // We're done
  TERMINATESESSION();

  // Execution will never reach here, but
  // the compiler will bitch without it
  return 0U;
}

unsigned __stdcall FURRSession::_ImageLoaderThread(void *pVoid) {

  IMAGELOADERTHREAD ilt;

  ilt = (*reinterpret_cast<PIMAGELOADERTHREAD>(pVoid));

  // Load the image file from disk
  if (g_LoaderMap.LoadImage(ilt.tsImageFile, &ilt.pImg)) {

    // Color-adjust image, annotate, and split channels
    if (ilt.pImg->PrepareForShoot(ilt.pRS->AP, ilt.pRS->Annotation)) {

      // Done read-ahead
      SetEvent(ilt.hEventDone);
      ilt.bSuccess = true;

    }

  }

  // Done loading and splitting image
  _endthreadex(ilt.bSuccess ? 0U : 1U);
  return 0U;
}

bool FURRSession::_SpawnImageLoaderThread(PIMAGELOADERTHREAD pilt) {

  bool r          = false;
  UINT uiThreadID = 0U;
  
  if (NULL != _beginthreadex(NULL, 0, _ImageLoaderThread, pilt, 0, &uiThreadID)) {

    r = true;

  }

  return r;
}

void FURRSession::_FormatMsec(DWORD_PTR dwMsec, TSTRING &tsOut) {

  if (0UL < dwMsec) {

    DWORD_PTR dwHours = 0UL;
    DWORD_PTR dwMins  = 0UL;
    DWORD_PTR dwSec   = 0UL;

    dwHours = (((dwMsec / 1000) / 60) / 60);
    dwMins  = (((dwMsec / 1000) / 60) % 60);
    dwSec   = ((dwMsec / 1000) % 60);

    sprintf(tsOut, _T("%03d:%02d:%02d"), dwHours, dwMins, dwSec);

  }

}
