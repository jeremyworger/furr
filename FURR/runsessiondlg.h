#ifndef _RUNSESSIONDLG_H
#define _RUNSESSIONDLG_H

#include "stdafx.h"
#include "furrtypes.h"
#include "resource.h"
#include "furrsession.h"
#include "furrjobdialog.h"

/*
 * RunSessionDlg Class
 *
 * Displays information about the progress
 * of a slideshow currently being displayed,
 * with options to abort, etc.
 */
class RunSessionDlg : public FURRJobDialog {

public:
  RunSessionDlg() {

  }
  ~RunSessionDlg() {

  }

public:

  bool DoModal(HINSTANCE hInstance, PCTSTR pszTemplate, HWND hWndParent);
  DLG_VMSG OnCreate(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnCommand(WPARAM wParam, LPARAM lParam);

  void Done(void);
  void Die(JobStatus Status);
  void UpdateUI(const FSEPROGRESS *pfsep);

  static void ThreadCallback(UINT_PTR uiEvent, DWORD_PTR dwUser, const void *pData);

private:

  void _InitialSetUIData(void);

private:
  FURRSession m_Session;
  Window      m_dr;
  Window      m_fr;
  Window      m_cd;
  Window      m_et;
  Window      m_md;
  Window      m_st;
  Window      m_te;
  Window      m_tr;
  Window      m_pb;
  Window      m_ab;
  Window      m_fb;

};

#endif // !_RUNSESSIONDLG_H