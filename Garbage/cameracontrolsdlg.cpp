#include "stdafx.h"
#include "resource.h"
#include "cameracontrolsdlg.h"


/*
 * CameraControlsDlg : Public
 */


BOOL CameraControlsDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_FrameCount.Attach(GetDlgItem(IDC_FRAMECOUNTER));
  m_FastCount.Attach(GetDlgItem(IDC_MOVEVAL));
  m_ManualString.Attach(GetDlgItem(IDC_ANYSTRING));

  m_FastCount.SetWindowText(_T("1"));

  if (!m_Camera.Open()) {

    MessageBox(m_hWnd, _T("Failed to open serial port to communicate with the camera!")
                       _T(" Is it already in use?"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);
    EndDialog(0);

  }

  return TRUE;
}

BOOL CameraControlsDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDC_STEPFWD:
    {
      m_Camera.SetDirection(true);
      m_Camera.Step();
    }
    break;
    case IDC_STEPREV:
    {
      m_Camera.SetDirection(false);
      m_Camera.Step();
    }
    break;
    case IDC_FASTFWD:
    {
      TSTRING tsString;

      if (m_FastCount.GetWindowText(tsString)) {

        m_Camera.SetDirection(true);
        m_Camera.Advance(tsString);

      }
    }
    break;
    case IDC_FASTREV:
    {
      TSTRING tsString;

      if (m_FastCount.GetWindowText(tsString)) {

        m_Camera.SetDirection(false);
        m_Camera.Advance(tsString);

      }
    }
    break;
    case IDC_SENDANYSTRING:
    {
      TSTRING tsString;

      if (m_ManualString.GetWindowText(tsString)) {

        if (_T("") != tsString) {

          tsString += _T("\r");
          m_Camera.Write(tsString);

        }

      }
    }
    break;
    case IDOK:
      EndDialog(0);
    break;

  }

  return FALSE;
}

BOOL CameraControlsDlg::OnClose(WPARAM wParam, LPARAM lParam) {

  EndDialog(0);

  return FALSE;
}