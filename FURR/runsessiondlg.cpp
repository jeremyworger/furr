#include "stdafx.h"
#include "runsessiondlg.h"
#include "furrmaindlg.h"
#include "directdrawwnd.h"

extern FURRMainDlg g_MainDlg;

/*
 * RunSessionDlg : Public
 */
bool RunSessionDlg::DoModal(HINSTANCE hInstance, PCTSTR pszTemplate, HWND hWndParent) {

  bool r = false;

  if (NULL != m_pRS) {
      r = true;
  }

  return r ? Dialog::DoModal(hInstance, pszTemplate, hWndParent) : r;
}

BOOL RunSessionDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_dr.Attach(GetDlgItem(IDC_DISPLAYRES));
  m_fr.Attach(GetDlgItem(IDC_FRAMERES));
  m_cd.Attach(GetDlgItem(IDC_COLOURDEPTH));
  m_et.Attach(GetDlgItem(IDC_EXPOSURETIME));
  m_md.Attach(GetDlgItem(IDC_MODE));
  m_st.Attach(GetDlgItem(IDC_STATUSTEXT));
  m_te.Attach(GetDlgItem(IDC_ELAPSEDTIME));
  m_tr.Attach(GetDlgItem(IDC_REMAININGTIME));
  m_pb.Attach(GetDlgItem(IDC_PROGRESS));
  m_ab.Attach(GetDlgItem(IDCANCEL));
  m_fb.Attach(GetDlgItem(IDOK));

  _InitialSetUIData();

  if (!m_Session.Spawn(m_pRS, ThreadCallback, reinterpret_cast<DWORD_PTR>(this))) {

    g_MainDlg.Output(FURR_OT_ERROR, _T("Failed to initialize FURRSession object!"));
    Die(FAILED);

  }

  return TRUE;
}

BOOL RunSessionDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDCANCEL:
    {
      Die(CANCELLED);
      break;
    }
    case IDOK:
    {
      Die(SUCCEEDED);
      break;
    }

    default:
      return FALSE;
  }

  return FALSE;
}

void RunSessionDlg::Done(void) {

  m_fb.Enable();
  m_ab.Disable();

}

void RunSessionDlg::Die(JobStatus Status) {

  if (Status == FAILED || Status == CANCELLED) { m_Session.Terminate(); }

  return FURRJobDialog::Die(Status);
}

void RunSessionDlg::UpdateUI(const FSEPROGRESS *pfsep) {

  // Progress bar
  m_pb.SendMsg(PBM_SETPOS, (100 * pfsep->dwCurFrame / pfsep->dwMaxFrame), 0);

  // Current frame counter
  TSTRING t;

  sprintf(t, _T("%d of %d"), pfsep->dwCurFrame, pfsep->dwMaxFrame);
  m_st.SetWindowText(t);

  // Elapsed time
  m_te.SetWindowText(pfsep->tsElapsedTime);

  // Remaining time
  m_tr.SetWindowText(pfsep->tsRemainingTime);

}

void RunSessionDlg::ThreadCallback(UINT_PTR uiEvent, DWORD_PTR dwUser, const void *pData) {
  
  RunSessionDlg *pThis = reinterpret_cast<RunSessionDlg *>(dwUser);

  switch (uiEvent) {

    case FSE_OUTPUT:
    {
      // Throw output to main dialog
      g_MainDlg.Output(FURR_OT_INFORMATION, _T("%s"), reinterpret_cast<const TCHAR *>(pData));

    }
    break;

    case FSE_PROGRESS:
     {
       // Update User Interface with current progress data
       pThis->UpdateUI(reinterpret_cast<const FSEPROGRESS *>(pData));

     }
    break;

    case FSE_WARNING:
    {
      // Throw warning to main dialog
      g_MainDlg.Output(FURR_OT_WARNING, _T("%s"), reinterpret_cast<const TCHAR *>(pData));
    }
    break;

    case FSE_ERROR:
    {
      // Throw error to main dialog
      g_MainDlg.Output(FURR_OT_ERROR, _T("%s"), reinterpret_cast<const TCHAR *>(pData));
    }
    break;

    case FSE_ABORTED:
    {
      // We know it was aborted, but notify main dialog about it
      g_MainDlg.Output(FURR_OT_INFORMATION, _T("Session aborted by user"));

    }
    break;

    case FSE_FINISHED:
    {
      // Session is over; enable finished button
      pThis->Done();
    }
    break;
    case FSE_FATAL:
    {
      // Something horrible has happened; hide dialog
      pThis->EndDialog(0);
    }
    break;

  }

}

/*
 * RunSessionDlg : Private
 */

void RunSessionDlg::_InitialSetUIData(void) {

  if (NULL != m_pRS) {

    TSTRING t;

    sprintf(t, _T("%dx%d pixels"), DDWND_DEFAULTX, DDWND_DEFAULTY);
    m_dr.SetWindowText(t);

    sprintf(t, _T("%dx%d pixels"), ImgRes2NumericX(m_pRS->AP.Global.ImgRes), ImgRes2NumericY(m_pRS->AP.Global.ImgRes));
    m_fr.SetWindowText(t);

    sprintf(t, _T("%d bits-per-pixel"), BitDepth2Numeric(m_pRS->AP.Global.Depth));
    m_cd.SetWindowText(t);

    switch (m_pRS->AP.Global.Mode) {

      case COLORWHEEL:
      case CHANNELSPLIT:
      {
        sprintf(t, _T("R: %d ms / %.02f s | G: %d ms / %.02f s | B: %d ms / %.02f s"),
                m_pRS->AP.Red.dwET,
                static_cast<float>(m_pRS->AP.Red.dwET / 1000.0f),
                m_pRS->AP.Green.dwET,
                static_cast<float>(m_pRS->AP.Green.dwET / 1000.0f),
                m_pRS->AP.Blue.dwET,
                static_cast<float>(m_pRS->AP.Blue.dwET / 1000.0f));
      }
      break;
      case FULLIMAGE:
      {
        sprintf(t, _T("%d msec (%.02f seconds)"),
                m_pRS->AP.Full.dwET,
                static_cast<float>(m_pRS->AP.Full.dwET / 1000.0f));
      }
      break;

    }

    m_et.SetWindowText(t);

    m_md.SetWindowText(m_pRS->AP.Global.Mode2String());

    m_st.SetWindowText(_T("<preparing session>"));

    m_te.SetWindowText(_T("<calculating>"));

    m_tr.SetWindowText(_T("<calculating>"));

  }

}
