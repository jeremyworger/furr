#include "stdafx.h"
#include "resource.h"
#include "cameracontrolsdlg.h"
#include "furrmaindlg.h"

/*
 * CameraControlsDlg : Public
 */
BOOL CameraControlsDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

 /*
  * Attempt to open serial port communication
  * with AMS driver.
  */
  if (!m_AMSComm.Open()) {

    m_pParent->Output(FURR_OT_ERROR,
                      _T("CameraControls: Couldn't establish communication link")
                      _T(" with AMS driver!  Win32 Error %d"),
                      GetLastError());
    EndDialog(0);

  }

  m_WindFwdFrames.Attach(GetDlgItem(IDC_WINDFWDFRAMES));
  m_WindRevFrames.Attach(GetDlgItem(IDC_WINDREVFRAMES));

  m_WindFwdFrames.SetWindowText(_T("0"));
  m_WindRevFrames.SetWindowText(_T("0"));

  return TRUE;
}

BOOL CameraControlsDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDC_STEPFWD:
    {

      if (!_Wind(1)) {

        _ThrowAMSError();

      }

    }
    break;

    case IDC_STEPREV:
    {

      if (!_Wind(1, false)) {

        _ThrowAMSError();

      }

    }
    break;

    case IDC_WINDFWD:
    { 

      _DoWind();

    }
    break;

    case IDC_WINDREV:
    {

      _DoWind(false);

    }
    break;
    case IDC_ZEROACCUMULATOR:
    {
      
      _DoZeroAccumulator();

    }
    break;

    case IDCANCEL:
    {
      EndDialog(0);
    }
    break;

  }

  return FALSE;
}

BOOL CameraControlsDlg::OnDestroy(WPARAM wParam, LPARAM lParam) {

  if (!m_AMSComm.Close()) {

    m_pParent->Output(FURR_OT_WARNING, 
                      _T("CameraControls: Couldn't close serial port!")
                      _T(" Win32 Error %d"),
                      GetLastError());

  }

  return FALSE;
}

/*
 * CameraControlsDlg : Private
 */
bool CameraControlsDlg::_Wind(int iSteps, bool bForward /* = true */) {

  bool r = false;

  m_CappingShutter.Cap();

  r = m_AMSComm.Camera_Wind(iSteps, bForward);

  m_CappingShutter.Uncap();

  return r;
}

void CameraControlsDlg::_DoWind(bool bForward /* = true */) {

  int iDlgItem = bForward ? IDC_WINDFWDFRAMES : IDC_WINDREVFRAMES;
  
  if (0 == GetWindowTextLength(GetDlgItem(iDlgItem))) {

    MessageBox(m_hWnd, _T("You must enter at least 1 frame to wind"),
                _T("FURR - Error"),
                MB_OK | MB_ICONSTOP);
    return;

  }

  BOOL bTranslated  = FALSE;
  UINT uiSteps      = GetDlgItemInt(m_hWnd, iDlgItem, &bTranslated, FALSE);

  if (FALSE == bTranslated) {

    m_pParent->Output(FURR_OT_ERROR, _T("CameraControls:")
                      _T(" An internal error has ocurred.")
                      _T(" Win32 Error %d"),
                      GetLastError());
    return;

  }

  if (0 < uiSteps) {

    if (!_Wind(static_cast<int>(uiSteps), bForward)) {

      _ThrowAMSError();

    }

  } else {

    MessageBox(m_hWnd, _T("You must enter at least 1 frame to wind"),
                _T("FURR - Error"),
                MB_OK | MB_ICONSTOP);

  }

}

void CameraControlsDlg::_DoZeroAccumulator(void) {

  TSTRING tsMsg;

  sprintf(tsMsg, _T("Are you sure you want")
          _T(" to zero the step accumulator and")
          _T(" global frame count?\n\n")
          _T("Frame count is currently: %d\n"),
          *m_pdwFrameCount);

  if (IDYES == MessageBox(m_hWnd, tsMsg.c_str(),
                          _T("FURR - Confirm Accumulator Reset"),
                          MB_YESNO | MB_ICONQUESTION))
  {

    if (!m_AMSComm.Camera_ResetAccumulator()) {

      _ThrowAMSError();

    } else {

      *m_pdwFrameCount = 0UL;
      m_pParent->Output(FURR_OT_INFORMATION, _T("CameraControls:")
                        _T("Set step accumulator and frame count to zero"));

    }

  }

}

void CameraControlsDlg::_ThrowAMSError(void) {

    m_pParent->Output(FURR_OT_ERROR,
                      _T("CameraControls: Couldn't communicate")
                      _T(" with AMS driver! Win32 Error %d"),
                      GetLastError());

}
