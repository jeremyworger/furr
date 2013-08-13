#ifndef _PREFERENCESDLG_H
#define _PREFERENCESDLG_H

#include "stdafx.h"

/*
 * PREFERENCES Structure
 *
 * Contains preference data
 * to be serialized to the registry
 */
typedef struct tagPREFERENCES {

  tagPREFERENCES() {

    bLockSettings = false;
    bVistaMode    = false;
    dwID          = 4UL;
    fIE           = 0.0f;

  }

  bool bLockSettings;
  bool bVistaMode;
  DWORD dwID;
  float fIE;

} PREFERENCES, *PPREFERENCES;

/*
 * PreferencesDlg Class
 *
 * Represents the FURR preferences dialog
 * and associated code
 */
class PreferencesDlg : public Dialog {

public:
  PreferencesDlg() { }
  ~PreferencesDlg() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  BOOL OnClose(WPARAM wParam, LPARAM lParam);

private:
  PREFERENCES m_Prefs;
  Window m_LockSettings;
  Window m_VistaMode;
  Window m_ID;
  Window m_IE;

  void _LoadSettings(void);
  void _LockSettings(bool bLock = true);
  float _GetFloatValue(Window &Wnd);
};


/*
 * Global functions
 */


bool SerializePreferences(const PREFERENCES &p);
bool LoadPreferences(PREFERENCES &p);

#endif // !_PREFERENCESDLG_H