#ifndef _WEDGESETTINGSDLG_H
#define _WEDGESETTINGSDLG_H

#include "stdafx.h"
#include "furrtypes.h"

class WedgeSettingsDlg : public Dialog {

public:
  WedgeSettingsDlg(RECORDSESSION *pRS) { m_pRS = pRS; }
  ~WedgeSettingsDlg() {}

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
  Window m_ID;
  Window m_IEStart;
  Window m_IEEnd;
  Window m_IEStep;
  RECORDSESSION *m_pRS;

float _GetFloatValue(Window &Wnd) {

  float r = 0.0f;
  TSTRING tsText;

  if (Wnd.GetWindowText(tsText)) {

    r = _tstof(tsText.c_str());

  }

  return r;
}
};

#endif // !_WEDGESETTINGSDLG_H