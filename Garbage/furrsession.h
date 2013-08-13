#ifndef _SESSION_H
#define _SESSION_H

#include "stdafx.h"
#include <process.h>
#include "cameracomm.h"
#include "furrtypes.h"
#include "loadermap.h"
#include "../inpout32/include/inpout32.h"

/*
 * Constants
 */
#define FSE_OUTPUT    0x00000000U
#define FSE_PROGRESS  0x00000001U
#define FSE_FINISHED  0x00000002U
#define FSE_ABORTED   0x00000003U
#define FSE_WARNING   0x00000004U
#define FSE_ERROR     0x00000005U

#define FS_MAXMSGSIZE 2048

#define FS_TIMERRESOLUTION  0

/*
 * FSEPROGRESS Structure
 *
 * Provides information about the
 * progress of the session to the caller.
 * When the uiEvent parameter of the PSESSIONCALLBACK
 * function is FSE_PROGRESS, the pData argument points
 * to an instance of this structure.
 */
typedef struct tagFSEPROGRESS {

  tagFSEPROGRESS() {

    dwCurFrame      = 0UL;
    dwMaxFrame      = 0UL;
    tsElapsedTime   = _T("000:00:00");
    tsRemainingTime = _T("000:00:00");

  }

  tagFSEPROGRESS &operator=(const tagFSEPROGRESS &in) {

    dwCurFrame      = in.dwCurFrame;
    dwMaxFrame      = in.dwMaxFrame;
    tsElapsedTime   = in.tsElapsedTime;
    tsRemainingTime = in.tsRemainingTime;

    return (*this);

  }

  DWORD_PTR dwCurFrame;     // Current frame (x of n)
  DWORD_PTR dwMaxFrame;     // Max frame (n)
  TSTRING tsElapsedTime;    // Elapsed time (HHH:MM:SS)
  TSTRING tsRemainingTime;  // Remaining time (HHH:MM:SS)

} FSEPROGRESS, *PFSEPROGRESS;

/*
 * PSESSIONCALLBACK Function typedef
 *
 * Used by the record session thread to report progress
 * or failure to the calling object.
 *
 * If uiEvent is:
 *
 * FSE_PROGRESS, pData points to an FSEPROGRESS structure.
 * FSE_FINISHED, pData is NULL.
 * FSE_ABORTED, pData is NULL.
 * FSE_WARNING, pData is a pointer to a TCHAR array containing the warning message.
 * FSE_ERROR, pData is a pointer to a TCHAR array containing the error message.
 */
typedef void (*PSESSIONCALLBACK)(UINT_PTR uiEvent, DWORD_PTR dwUser, const void *pData);

/*
 * FURRSESSIONTHREAD Structure
 *
 * Contains all data required by
 * the session thread.
 */
typedef struct tagFURRSESSIONTHREAD {

  tagFURRSESSIONTHREAD() {

    pRS         = NULL;
    pSCB        = NULL;
    hEvent      = NULL;
    dwUser      = 0UL;

  }

  tagFURRSESSIONTHREAD &operator=(const tagFURRSESSIONTHREAD &in) {

    pRS     = in.pRS;
    pSCB    = in.pSCB;
    hEvent  = in.hEvent;
    dwUser  = in.dwUser;

    return (*this);

  }

  PRECORDSESSION pRS;
  PSESSIONCALLBACK pSCB;
  HANDLE hEvent;
  DWORD_PTR dwUser;

} FURRSESSIONTHREAD, *PFURRSESSIONTHREAD;

/*
 * IMAGELOADERTHREAD Structure
 *
 * Used to pass needed information
 * to the image loader worker thread.
 */
typedef struct tagIMAGELOADERTHREAD {

  tagIMAGELOADERTHREAD() {

    bSuccess    = false;
    pRS         = NULL;
    pSCB        = NULL;
    hEventDone  = NULL;
    hEventAbort = NULL;
    dwUser      = 0UL;
    tsImageFile = _T("");
    pImg        = NULL;

  }

  tagIMAGELOADERTHREAD &operator=(const tagIMAGELOADERTHREAD &in) {

    bSuccess    = in.bSuccess;
    pRS         = in.pRS;
    pSCB        = in.pSCB;
    hEventDone  = in.hEventDone;
    hEventAbort = in.hEventAbort;
    dwUser      = in.dwUser;
    tsImageFile = in.tsImageFile;
    pImg        = in.pImg;

    return (*this);
  }

  bool bSuccess;
  PRECORDSESSION pRS;
  PSESSIONCALLBACK pSCB;
  HANDLE hEventDone;
  HANDLE hEventAbort;
  DWORD_PTR dwUser;
  TSTRING tsImageFile;
  FURRImage *pImg;

} IMAGELOADERTHREAD, *PIMAGELOADERTHREAD;

/*
 * FURRSession Class
 *
 * This object wraps the slide-show
 * functionality of FURR.  Images are
 * displayed and motors controlled by
 * this object.
 */
class FURRSession {

public:
  FURRSession() {

    m_hEvent    = NULL;
    m_hThread   = NULL;
    m_bRunning  = false;

  }
  virtual ~FURRSession() {

    CloseHandle(m_hThread);

  }

  // Begins a new session. A new session cannot be spawned until
  // this session has completed, or is terminated.
  bool Spawn(PRECORDSESSION pRS, PSESSIONCALLBACK pSCB = NULL, DWORD_PTR dwUser = 0UL);

  // Terminates a session that is in-progress after the current
  // frame has been completely processed. This function blocks
  // execution of the calling thread until the session has been
  // successfully stopped.
  void Terminate(void);

protected:
  // Event handlers called by the worker thread
  // to fire off events to the calling thread
  static void _FireOutput(PFURRSESSIONTHREAD pfst, const TCHAR *szOutputMsg, ...);
  static void _FireProgress(PFURRSESSIONTHREAD pfst, DWORD_PTR dwStartTime,
                            DWORD_PTR dwMaxFrames, DWORD_PTR dwCurFrame, DWORD_PTR dwAvgExposure);
  static void _FireWarning(PFURRSESSIONTHREAD pfst, const TCHAR *szWarningMsg, ...);
  static void _FireError(PFURRSESSIONTHREAD pfst, const TCHAR *szErrorMsg, ...);
  static void _FireAbort(PFURRSESSIONTHREAD pfst);
  static void _FireFinished(PFURRSESSIONTHREAD pfst);
  static void _PrintMsg(PFURRSESSIONTHREAD pfst, DWORD_PTR dwEvent, const TCHAR *szMsg, va_list val);

  // Session worker thread (The Real Meat)
  static unsigned __stdcall _Thread(void *pVoid);

  // Image loader worker thread
  static unsigned __stdcall _ImageLoaderThread(void *pVoid);

  // Function to spawn image loader worker thread
  static bool _SpawnImageLoaderThread(PIMAGELOADERTHREAD pilt);
  
  // Utility function used to format a time string
  static void _FormatMsec(DWORD_PTR dwMsec, TSTRING &tsOut);

protected:
  HANDLE  m_hEvent;
  HANDLE  m_hThread;
  bool    m_bRunning;

};

#endif // !_SESSION_H
