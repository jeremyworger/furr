#ifndef _SESSIONPREPAREDLG_H
#define _SESSIONPREPAREDLG_H

#include "stdafx.h"
#include "resource.h"
#include "furrjobdialog.h"
#include <process.h>
#include <shlobj.h>

/*
 * Constants
 */
#define SPD_TITLE     _T("Verifying image file(s) ... ")
#define SPD_FINISHED  _T("Finished!")

/*
 * PREPARESESSIONTHREAD Structure
 *
 * Contains information used by a
 * worker thread to verify images.
 */
typedef struct tagPREPARESESSIONTHREAD {

  tagPREPARESESSIONTHREAD() {
    
    hEvent    = NULL;
    pDlg      = NULL;
    pRS       = NULL;

  }

  HANDLE hEvent;
  RECORDSESSION *pRS;
  class SessionPrepareDlg *pDlg;

} PREPARESESSIONTHREAD, *PPREPARESESSIONTHREAD;

/*
 * SessionPrepareDlg Class
 *
 * Displays progress while a session is being prepared
 * (files are being parses, and image frames
 * checked for validity and compatibility)
 */
class SessionPrepareDlg : public FURRJobDialog {

public:
  SessionPrepareDlg() {

    m_hEvent        = NULL;
    m_hThread       = NULL;
    m_uiThread      = 0U;
    m_uiMaxFiles    = 0U;
    m_Status        = FAILED;

  }

  ~SessionPrepareDlg() {

    CloseHandle(m_hEvent);
    CloseHandle(m_hThread);
    
  }

  DLG_VMSG OnCreate(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnCommand(WPARAM wParam, LPARAM lParam);

  bool SetStatusText(CTSTRING &tsText);
  void SetMaxFileNum(UINT_PTR uiMaxFiles);
  void SetProgress(UINT_PTR uiProgress);

  void Die(JobStatus Status);

private:
    bool _SpawnThread(void);
    static unsigned __stdcall _PrepareSessionThread(void *pVoid);
    bool _BrowseForFolder(JobStatus &Status);
    static int CALLBACK _BrowseCallback(HWND hWnd, UINT uiMsg, LPARAM lParam, LPARAM lpData);
    bool _VerifyImage(CTSTRING &tsImg);

private:
  Window          m_pb;
  Window          m_st;
  Window          m_cb;
  Window          m_db;
  HANDLE          m_hEvent;
  HANDLE          m_hThread;
  UINT_PTR        m_uiThread;
  UINT_PTR        m_uiMaxFiles;

};

#endif // !_SESSIONPREPAREDLG_H
