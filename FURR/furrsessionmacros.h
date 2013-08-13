#ifndef _FURRSESSIONMACROS_H
#define _FURRSESSIONMACROS_H

/*
 * Macros used by FURRSession
 */

/*
 * TERMINATESESSION Macro
 *
 * Used by _Thread() to do cleanup
 * and end execution of the session thread.
 */
#define TERMINATESESSION() \
  _FireFinished(&fst); \
  CloseHandle(ddwi.hEventRender); \
  g_DDDisplayWnd.Hide(); \
  g_DDDisplayWnd.Destroy(); \
  SAFE_DELETE(pCurImg); \
  Comm.Close(); \
  _endthreadex(0U); \

/*
 * ISABORTEDSESSION Macro
 *
 * Used by _Thread() to determine if
 * the session has been aborted.
 */
#define ISABORTEDSESSION() \
  (WAIT_OBJECT_0 == WaitForSingleObject(fst.hEvent, 0)) ? true : false

/*
 * ABORTIFNECESSARY Macro
 *
 * Checks to see if the session should
 * be aborted, and aborts if so.
 */
#define ABORTIFNECESSARY() \
  if (ISABORTEDSESSION()) { TERMINATESESSION(); }

/*
 * ISDONERENDERING Macro
 *
 * Used by _Thread() to determine if
 * the Direct Draw window is done rendering
 * the current frame.
 */
#define ISDONERENDERING() \
  (WAIT_OBJECT_0 == WaitForSingleObject(ddwi.hEventRender, 0)) ? true : false

/*
 * WAITFORRENDER Macro
 *
 * Used by _Thread() to wait on the
 * Direct Draw window to finish rendering,
 * while checking if the session has been
 * aborted by the user. If so, the session
 * is aborted and execution of the thread is terminated.
 */
#define WAITFORRENDER() \
  while (!ISDONERENDERING()) { \
    Sleep(0); \
    ABORTIFNECESSARY(); \
  } \
  ResetEvent(ddwi.hEventRender);

/*
 * WAITFORWIND Macro
 *
 * Used by _Thread() to wait for
 * an axis to finish winding.  Will
 * not return until the axis has finished
 * winding, an error occurs, or the session
 * has been aborted by the user.
 */
#define WAITFORWIND(axis) \
  while (AXISIDLE != (Query = Comm.IsAxisMoving(axis)) \
         && (AXISERROR != Query)) \
  { \
    Sleep(0); \
    ABORTIFNECESSARY(); \
  } \
  if (AXISERROR == Query) { AMSFAILURE(); }

/*
 * CAMERATOGGLE Macro
 *
 * Toggles the camera from capped to
 * uncapped and vice versa.  Issues
 * the wind command, waits for the wind
 * to complete, and then toggles the
 * electronic capping shutter.
 *
 * Note: true = uncap, false = cap
 */
#define CAMERATOGGLE(cap) \
  if (!Comm.Camera_ToggleMechShutter()) { AMSFAILURE(); } \
  WAITFORWIND(FAMSC_CAMERAAXIS); \
  if (cap) { Comm.Uncap(); } else { Comm.Cap(); }

/*
 * EXPOSEFRAME Macro
 *
 * Used by _Thread() to sleep off
 * thread execution for a given amount
 * of milliseconds. Also checks to see
 * if the session has been aborted, and if so
 * ends the session.
 */
#define EXPOSEFRAME(msec) \
  hEventExpose = CreateEvent(NULL, TRUE, FALSE, NULL); \
  if (NULL == timeSetEvent(msec, FS_TIMERRESOLUTION, reinterpret_cast<LPTIMECALLBACK>(hEventExpose), \
                           0UL, TIME_CALLBACK_EVENT_SET | TIME_ONESHOT)) \
  { \
    _FireError(&fst, _T("FURRSession: Failed to create multimedia timer! Win32 Error: %d"), GetLastError()); \
  }  else { \
    while (WAIT_OBJECT_0 != WaitForSingleObject(hEventExpose, 0)) { \
      Sleep(0); \
      if (ISABORTEDSESSION()) { \
        CloseHandle(hEventExpose); \
        TERMINATESESSION(); \
      } \
    } \
    if (NULL != hEventExpose) { CloseHandle(hEventExpose); } \
  } \

/*
 * AMSFAILURE Macro
 *
 * Used in the event that communications
 * with the AMS driver has failed.
 */
#define AMSFAILURE() \
  _FireError(&fst, _T("FURRSession: Unable to communicate with AMS driver.") \
                  _T(" Check driver power supply and connection to PC.")); \
  _FireFatal(&fst); \
  TERMINATESESSION(); \

#endif // !_FURRSESSIONMACROS_H
