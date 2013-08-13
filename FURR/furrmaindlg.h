#ifndef _FRSMAINDLG_H
#define _FRSMAINDLG_H

#include "stdafx.h"
#include "adjustparamsdlg.h"
#include "furrtypes.h"
#include <stdarg.h>

/*
 * Constants
 */
#define FMD_SAVELOGTITLE  _T("FURR - Choose filename to save log as")
#define FMD_SAVELOGFILTER _T("FURR Log Files (*.FLF)\0All Files (*.*)\0\0")

enum FURR_OT {
  FURR_OT_WARNING = 0,
  FURR_OT_INFORMATION,
  FURR_OT_ERROR
};

class FURRMainDlg : public Dialog {

public:
  FURRMainDlg() {}
  ~FURRMainDlg() {}

  DLG_VMSG OnCreate(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnSize(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnClose(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnNotify(WPARAM wParam, LPARAM lParam);
  DLG_VMSG OnCommand(WPARAM wParam, LPARAM lParam);

  void Output(FURR_OT type, TCHAR *szDetails, ...);

private:
  Window m_sb;
  ListView  m_lv;
  ImageList m_il;

  void _SaveLogFile(void);
  bool _DoNewSession(void);
  void _DoNewWedgeSession(void);

};

#endif // !_FRSMAINDLG_H