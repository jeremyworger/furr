#include "stdafx.h"
#include "wedgesettingsdlg.h"
#include "resource.h"

BOOL WedgeSettingsDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_ID.Attach(GetDlgItem(IDC_ID));
  m_IEStart.Attach(GetDlgItem(IDC_IESTART));
  m_IEEnd.Attach(GetDlgItem(IDC_IEEND));
  m_IEStep.Attach(GetDlgItem(IDC_IESTEP));

  m_ID.SetWindowText(_T("0"));
  m_IEStart.SetWindowText(_T("0.0"));
  m_IEEnd.SetWindowText(_T("0.0"));
  m_IEStep.SetWindowText(_T("0.0"));

  return TRUE;
}

BOOL WedgeSettingsDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {
    case IDOK:
    {
      BOOL bTranslated;
      m_pRS->Wedge.dwID = ::GetDlgItemInt(m_hWnd, IDC_ID, &bTranslated, FALSE);
      m_pRS->Wedge.fIEStart = _GetFloatValue(m_IEStart);
      m_pRS->Wedge.fIEEnd   = _GetFloatValue(m_IEEnd);
      m_pRS->Wedge.fIEStep  = _GetFloatValue(m_IEStep);
    
    EndDialog(0);
    }
    
    break;
  }

  return FALSE;
}