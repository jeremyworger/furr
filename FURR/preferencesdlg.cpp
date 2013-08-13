#include "stdafx.h"
#include "preferencesdlg.h"
#include "furrconfigs.h"
#include "resource.h"

/*
 * PreferencesDlg : Public
 */
BOOL PreferencesDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_LockSettings.Attach(GetDlgItem(IDC_CHKLOCKSETTINGS));
  m_VistaMode.Attach(GetDlgItem(IDC_CHKVISTAMODE));
  m_ID.Attach(GetDlgItem(IDC_ID));
  m_IE.Attach(GetDlgItem(IDC_IE));

  LoadPreferences(m_Prefs);
  
  if (m_Prefs.bLockSettings) {

    CheckDlgButton(m_hWnd, IDC_CHKLOCKSETTINGS, BST_CHECKED);
    _LockSettings(true);

  }

  if (m_Prefs.bVistaMode) {

    CheckDlgButton(m_hWnd, IDC_CHKVISTAMODE, BST_CHECKED);

  }

  TSTRING tsTemp;

  sprintf(tsTemp, _T("%d"), m_Prefs.dwID);

  m_ID.SetWindowText(tsTemp);

  sprintf(tsTemp, _T("%.02f"), m_Prefs.fIE);

  m_IE.SetWindowText(tsTemp);

  return TRUE;
}

BOOL PreferencesDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDOK:
    {
      if (IsDlgButtonChecked(m_hWnd, IDC_CHKLOCKSETTINGS) == BST_CHECKED) {

        m_Prefs.bLockSettings = true;

      } else {

        m_Prefs.bLockSettings = false;

      }

      if (IsDlgButtonChecked(m_hWnd, IDC_CHKVISTAMODE) == BST_CHECKED) {

        m_Prefs.bVistaMode = true;

      } else {

        m_Prefs.bVistaMode = false;

      }

      BOOL bTranslated;
      m_Prefs.dwID = ::GetDlgItemInt(m_hWnd, IDC_ID, &bTranslated, FALSE);

      m_Prefs.fIE = _GetFloatValue(m_IE);

      SerializePreferences(m_Prefs);
      EndDialog(0);
    }
    break;
    case IDCANCEL:
    {
      EndDialog(0);
    }
    break;
    case IDC_CHKLOCKSETTINGS:
    {

      if (IsDlgButtonChecked(m_hWnd, IDC_CHKLOCKSETTINGS)) {

        _LockSettings(true);

      } else {

        _LockSettings(false);

      }

    }
    break;

  }

  return FALSE;
}

BOOL PreferencesDlg::OnClose(WPARAM wParam, LPARAM lParam) {

  EndDialog(0);

  return FALSE;
}


/*
 * PreferencesDlg : Private
 */

void PreferencesDlg::_LockSettings(bool bLock) {

  if (bLock) {

    m_VistaMode.Disable();
    m_ID.Disable();
    m_IE.Disable();

  } else {

    m_VistaMode.Enable();
    m_ID.Enable();
    m_IE.Enable();

  }

}

float PreferencesDlg::_GetFloatValue(Window &Wnd) {

  float r = 0.0f;
  TSTRING tsText;

  if (Wnd.GetWindowText(tsText)) {

    r = static_cast<float>(_tstof(tsText.c_str()));
  }

  return r;
}

/*
 * Global functions
 */


bool SerializePreferences(const PREFERENCES &p) {

  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH)) {

    MessageBox(GetForegroundWindow(), _T("Failed to open preferences registry key!"),
                                      _T("FURR - Error"), MB_OK | MB_ICONSTOP);
    return false;

  }

  if (!Key.WriteBinary(FURR_PREFERENCESVALUE, &p, sizeof(p))) {

    MessageBox(GetForegroundWindow(), _T("Failed to write preferences to registry!"),
                                      _T("FURR - Error"), MB_OK | MB_ICONSTOP);
    return false;

  }
  
  return true;
}

bool LoadPreferences(PREFERENCES &p) {

  bool r = false;

  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH)) {

    MessageBox(GetForegroundWindow(), _T("Failed to open preferences registry key!"),
                                      _T("FURR - Error"), MB_OK | MB_ICONSTOP);
    return false;

  }

  if (!Key.ReadBinary(FURR_PREFERENCESVALUE,  &p, sizeof(p))) {

    MessageBox(GetForegroundWindow(), _T("Failed to read preferences from registry!"),
                                      _T("FURR - Error"), MB_OK | MB_ICONSTOP);
    return false;

  }

  return true;
}
