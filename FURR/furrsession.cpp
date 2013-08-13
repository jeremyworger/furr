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

 /*
  * We ain't spawning another session
  * if one is in progress.
  */
  if (!m_bRunning) {

   /*
    * Verify pointer(s)
    */
    if (NULL != pRS) {

     /*
      * Create event handle to signal worker
      * thread to terminate, if need be.
      */
      if (NULL != (m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {

       /*
        * Create data structure to send to session thread.
        */
        PFURRSESSIONTHREAD pfst = new FURRSESSIONTHREAD;

       /*
        * Successfully allocated heap memory?
        */
        if (NULL != pfst) {

          pfst->hEvent = m_hEvent;
          pfst->pRS    = pRS;
          pfst->pSCB   = pSCB;
          pfst->dwUser = dwUser;

         /*
          * Spawn session thread
          */
          UINT uiThreadID;

          if (NULL != (m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, _Thread, pfst, 0, &uiThreadID)))) {

           /*
            * Thread is executing
            */
            m_bRunning  = true;
            r           = true;

          }

        }

      }

    }

  }

 /*
  * Clean up resources
  */
  if (!r && (NULL != m_hEvent)) { CloseHandle(m_hEvent); }

  return r;
}

void FURRSession::Terminate(void) {

 /*
  * Do not terminate unless we're actually
  * executing currently
  */
  if (m_bRunning) {

   /*
    * Signal session thread to terminate
    */
    SetEvent(m_hEvent);

   /*
    * Wait for session thread to clean up & return control
    */
    while (WAIT_OBJECT_0 != WaitForSingleObject(m_hThread, 0UL)) {

      DoEvents(NULL, 0, 0, PM_REMOVE);

    }

   /*
    * Reset state variable(s)
    */
    m_bRunning  = false;
    m_hThread   = NULL;
    m_hEvent    = NULL;

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

void FURRSession::_FireFatal(PFURRSESSIONTHREAD pfst) {

  if (NULL != pfst->pSCB) {

    pfst->pSCB(FSE_FATAL, pfst->dwUser, NULL);

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

void FURRSession::_AMSCommErrorCallback(CTSTRING &tsMessage, DWORD_PTR dwUserData) {

  PFURRSESSIONTHREAD pfst = reinterpret_cast<PFURRSESSIONTHREAD>(dwUserData);

  if (NULL != pfst) {

    pfst->pSession->_FireError(pfst, tsMessage.c_str());

  }

}

unsigned __stdcall FURRSession::_Thread(void *pVoid) {

  FURRSESSIONTHREAD fst;

 /*
  * Copy thread arguments to stack-based memory and
  * free heap-allocated counterpart
  */
  fst = (*reinterpret_cast<PFURRSESSIONTHREAD>(pVoid));
  SAFE_DELETE(pVoid);

 /*
  * Initialize local variables
  */
  DWORD_PTR dwCurFrame  = 1UL;      // Current frame counter
  FRAME f;                          // FRAME used to iterate frames
  FURRImage *pCurImg    = NULL;     // Temporary FURRImage pointer
  FURRImage *pRAImg     = NULL;     // Read-ahead FURRImage pointer
  HANDLE hEventExpose   = NULL;     // Handle to event for exposure timer
  AxisQuery Query;                  // Value to store results from driver
  FURRSessionComm Comm;             // Peripheral communication interface

 /*
  * Calculate average exposure time
  */
  DWORD_PTR dwAvgExposure;

  if (fst.pRS->AP.Global.Mode == CHANNELSPLIT || fst.pRS->AP.Global.Mode == COLORWHEEL) {

    dwAvgExposure = ((fst.pRS->AP.Red.dwET + fst.pRS->AP.Green.dwET + fst.pRS->AP.Blue.dwET) / 3);

  } else {

    dwAvgExposure = (fst.pRS->AP.Full.dwET);

  }

 /*
  * Make note of the current tick count to use
  * in calculation of elapsed and remaining time
  */
  DWORD dwTimeStart = GetTickCount();
  TSTRING tsET      = _T("");

  _FormatMsec(dwAvgExposure * fst.pRS->FrameCount(), tsET);

  _FireOutput(&fst, _T("FURRSession: Begin session - Frame(s): %d, Mode: %s, Estimated time: %s"),
               fst.pRS->FrameCount(),
               fst.pRS->AP.Global.Mode2String().c_str(),
               tsET.c_str());

 /*
  * Initialize peripheral communication interface
  */
  Comm.SetCallback(_AMSCommErrorCallback, reinterpret_cast<DWORD_PTR>(&fst));

 /*
  * Initialize data structure
  * used by directdraw window.
  */
  DDRAWWNDINIT ddwi;

  ddwi.bInitVisible     = true;
  ddwi.bPrimaryDisplay  = true;// false;
  ddwi.bWindowed        = false;
  ddwi.hEventRender     = CreateEvent(NULL, TRUE, FALSE, NULL);

 /*
  * Establish serial communication line
  * with AMS driver
  */
  if (!Comm.Open()) {

    _FireError(&fst, _T("FURRSession: Failed to open serial communication port [%s];")
                     _T(" Win32 Error: %d"),
                     _AMSC_PORT,
                     GetLastError());
    TERMINATESESSION();

  }

 /*
  * Send initialization parameters to the AMS driver
  * for the camera axis
  */
  TSTRING tsParameters;

  sprintf(tsParameters, _T("%s ; %s ; %s ; %s ; %s ; %s ; %s"),
          FAMSC_CI_POWER,
          FAMSC_CI_STEPW,
          FAMSC_CI_MAXVELO,
          FAMSC_CI_RAMPSPD,
          FAMSC_CI_FIXDRES,
          FAMSC_CI_SETTLE,
          FAMSC_CI_INIVELO);

  _FireOutput(&fst, _T("FURRSession: Initializing camera on AMS axis '%c'"), FAMSC_CAMERAAXIS);
  _FireOutput(&fst, _T("FURRSession: AMS axis '%c' parameters: %s"),
               FAMSC_CAMERAAXIS,
               tsParameters.c_str());

  
  //if (!Comm.InitializeCameraAxis()) { AMSFAILURE(); }

 /*
  * Send initialization parameters to the AMS
  * driver for the color wheel axis
  */
  sprintf(tsParameters, _T("%s ; %s ; %s ; %s ; %s ; %s ; %s"),
          FAMSC_WI_POWER,
          FAMSC_WI_STEPW,
          FAMSC_WI_MAXVELO,
          FAMSC_WI_RAMPSPD,
          FAMSC_WI_FIXDRES,
          FAMSC_WI_SETTLE,
          FAMSC_WI_INIVELO);

  _FireOutput(&fst, _T("FURRSession: Initializing color wheel on AMS axis '%c'"), FAMSC_CWAXIS);
  _FireOutput(&fst, _T("FURRSession: AMS axis '%c' parameters: %s"),
               FAMSC_CWAXIS,
               tsParameters.c_str());

  
  //if (!Comm.InitializeColorWheelAxis()) { AMSFAILURE(); }

 /*
  * Wind camera axis to home sensor
  */
  //if (!Comm.Camera_WindHome()) { AMSFAILURE(); }

 /*
  * Wait for camera to wind home
  */
  //WAITFORWIND(FAMSC_CAMERAAXIS);

 /*
  * Wind color wheel axis to CLEAR filter
  */
  //if (!Comm.ColorWheel_WindClear()) { AMSFAILURE(); }

 /*
  * Wait for color wheel to wind home
  */
  //WAITFORWIND(FAMSC_CWAXIS);

 /*
  * Rewind frame list, and
  * image loading mechanism
  */
  fst.pRS->Rewind();
  g_LoaderMap.Rewind();

 /*
  * Begin session! Show recording display
  */
  if (!g_DDDisplayWnd.Display(ddwi, *fst.pRS)) {

    _FireError(&fst, _T("FURRSession: Failed to initialize recording display;")
                     _T(" Win32 Error: %d"));
    TERMINATESESSION();
    
  }

  while (fst.pRS->EnumFrames(f)) {

   /*
    * Aborted? -> Terminate
    */
    ABORTIFNECESSARY();

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

   /*
    * Aborted? -> Terminate
    */
    ABORTIFNECESSARY();

    if (!pCurImg->PrepareForShoot(fst.pRS->AP, fst.pRS->Annotation)) {

       _FireError(&fst, _T("FURRSession: Failed to prepare image file '%s' for shooting."), f.tsFileName.c_str());
       TERMINATESESSION();

    }

   /*
    * Begin frame exposure(s)
    */
    bool bColorWheel = false;

    switch (fst.pRS->AP.Global.Mode) {

      case COLORWHEEL:
      {
       /*
        * We're splitting the frame into
        * 3 different greyscale images that
        * represent each channel as greyscale
        * data, so that they can be singularly
        * exposed through the color wheel.
        */
        bColorWheel = true;

      }
      case CHANNELSPLIT:
      {
       /*
        * We're going to split the frame
        * into 3 distinct images that contain
        * only one channel of image data, so
        * that exposure times may vary for each
        * channel.
        */


       /*
        * RED
        */


        if (bColorWheel) {

         /*
          * Advance color wheel to RED filter
          */
          if (!Comm.ColorWheel_WindRed()) { AMSFAILURE(); }

         /*
          * Wait for color wheel to wind
          */
          WAITFORWIND(FAMSC_CWAXIS);

        }

       /*
        * Display RED channel image
        */
        if (!g_DDDisplayWnd.DisplayImage(FIC_RED, *pCurImg)) {

          _FireError(&fst, _T("FURRSession: Failed to display RED channel image"));
          TERMINATESESSION();

        }

       /*
        * Wait for the frame to render completely
        */
        WAITFORRENDER();

       /*
        * Check to see if session was aborted
        * before exposing this frame to the camera
        */
        ABORTIFNECESSARY();

       /*
        * Open camera shutter
        */
        CAMERATOGGLE(true);

       /*
        * Expose frame
        */
        EXPOSEFRAME(fst.pRS->AP.Red.dwET);


       /*
        * GREEN
        */


        if (bColorWheel) {

         /*
          * Advance color wheel to GREEN filter
          */
          if (!Comm.ColorWheel_WindGreen()) { AMSFAILURE(); }

         /*
          * Wait for color wheel to wind
          */
          WAITFORWIND(FAMSC_CWAXIS);

        }

       /*
        * Display GREEN channel image
        */
        if (!g_DDDisplayWnd.DisplayImage(FIC_GREEN, *pCurImg)) {

          _FireError(&fst, _T("FURRSession: Failed to display GREEN channel image"));
          TERMINATESESSION();

        }

       /*
        * Wait for the frame to render completely
        */
        WAITFORRENDER();

       /*
        * Check to see if session was aborted
        * before exposing this frame to the camera
        */
        ABORTIFNECESSARY();

       /*
        * Expose frame
        */
        EXPOSEFRAME(fst.pRS->AP.Green.dwET);


       /*
        * BLUE
        */


        if (bColorWheel) {

         /*
          * Advance color wheel to BLUE filter
          */
          if (!Comm.ColorWheel_WindBlue()) { AMSFAILURE(); }

         /*
          * Wait for color wheel to wind
          */
          WAITFORWIND(FAMSC_CWAXIS);

        }

       /*
        * Display BLUE channel image
        */
        if (!g_DDDisplayWnd.DisplayImage(FIC_BLUE, *pCurImg)) {

          _FireError(&fst, _T("FURRSession: Failed to display BLUE channel image"));
          TERMINATESESSION();

        }

       /*
        * Wait for the frame to render completely
        */
        WAITFORRENDER();

       /*
        * Check to see if session was aborted
        * before exposing this frame to the camera
        */
        ABORTIFNECESSARY();

       /*
        * Expose frame
        */
        EXPOSEFRAME(fst.pRS->AP.Blue.dwET);

       /*
        * Close camera shutter
        */
        CAMERATOGGLE(false);

        if (bColorWheel) {

         /*
          * Send color wheel back to CLEAR filter
          */
          if (!Comm.ColorWheel_WindClear()) { AMSFAILURE(); }

         /*
          * Wait for color wheel to wind
          */
          WAITFORWIND(FAMSC_CWAXIS);

        }

        break;
      }
      case FULLIMAGE:
      {
       /*
        * We're shooting a full-color/black-white
        * image. No channel splitting will take place.
        */
        if (!g_DDDisplayWnd.DisplayImage(FIC_FULL, *pCurImg)) {

          _FireError(&fst, _T("FURRSession(FULLIMAGE) : Failed to display image '%s'!"),
                     f.tsFileName.c_str());
          TERMINATESESSION();

        } else {

         /*
          * Image is rendering; wait for render event
          * and then expose image to film.
          */
          WAITFORRENDER();

         /*
          * Aborted? -> Terminate
          */
          ABORTIFNECESSARY();

         /*
          * Open shutter
          */
          CAMERATOGGLE(true);

         /*
          * Expose frame for desired msec
          */
          EXPOSEFRAME(fst.pRS->AP.Full.dwET);

         /*
          * Close shutter
          */
          CAMERATOGGLE(false);

        }

        break;
       }

    }

    _FireProgress(&fst, dwTimeStart, fst.pRS->FrameCount(), dwCurFrame++, dwAvgExposure);

    SAFE_DELETE(pCurImg);

  }

 /*
  * We're done; report session stats
  * and we're Audi 5000
  */
  DWORD dwTimeEnd   = GetTickCount();
  TSTRING tsTT      = _T("");

  _FormatMsec(dwTimeEnd - dwTimeStart, tsTT);

  _FireOutput(&fst, _T("FURRSession: End session - Frame(s): %d, Mode: %s, Elapsed time: %s"),
               fst.pRS->FrameCount(),
               fst.pRS->AP.Global.Mode2String().c_str(),
               tsTT.c_str());
  TERMINATESESSION();

 /*
  * Execution will never reach here, but
  * the compiler will bitch without it
  */
  return 0U;
}

unsigned __stdcall FURRSession::_ImageLoaderThread(void *pVoid) {

  IMAGELOADERTHREAD ilt;

  ilt = (*reinterpret_cast<PIMAGELOADERTHREAD>(pVoid));

 /*
  * Load the image file from disk
  */
  if (g_LoaderMap.LoadImage(ilt.tsImageFile, &ilt.pImg)) {

   /*
    * Color-adjust image, annotate, and split channels
    */
    if (ilt.pImg->PrepareForShoot(ilt.pRS->AP, ilt.pRS->Annotation)) {

     /*
      * Done read-ahead
      */
      SetEvent(ilt.hEventDone);
      ilt.bSuccess = true;

    }

  }

 /*
  * Done loading and splitting image
  */
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
