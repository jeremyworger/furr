#include "stdafx.h"
#include "furrmaindlg.h"
#include "resource.h"
#include "furr.h"
#include "preferencesdlg.h"
#include "furrsession.h"
#include "cameracontrolsdlg.h"

extern DWORD g_dwFrameCount;

BOOL FURRMainDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_lv.Create(WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | LVS_NOSORTHEADER | LVS_REPORT, 
              0, 0, 0, 0, m_hWnd, 0, GetModuleHandle(NULL));

  m_il.Create(16, 16, ILC_COLOR32 | ILC_MASK, 0, 1, GetModuleHandle(NULL));

  m_il.AddIcon((INT_PTR)IDI_WARNING);
  m_il.AddIcon((INT_PTR)IDI_INFORMATION);
  m_il.AddIcon((INT_PTR)IDI_ERROR);

  m_lv.SetImageList(m_il, LVSIL_SMALL);
  
  LVCOLUMN lvc  = {0};

  lvc.mask    = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
  lvc.fmt     = LVCFMT_CENTER;
  lvc.pszText = _T("Details");
  lvc.iOrder  = 0;

  m_lv.InsertColumn(0, &lvc);

  lvc.fmt     = LVCFMT_RIGHT;
  lvc.pszText = _T("Timestamp");
  lvc.iOrder  = 1;

  m_lv.InsertColumn(1, &lvc);

  m_sb.Create(0UL, STATUSCLASSNAME, _T(""), WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, m_hWnd, 0);

  SetSmIcon(LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_MAINICON)));

  OnSize(0, 0);

  return TRUE;
}

BOOL FURRMainDlg::OnSize(WPARAM wParam, LPARAM lParam) { 

  RECT rcClient = {0};
  WINDOWING wSb;

  GetClientRect(&rcClient);
  GetWindowCoords(m_sb.m_hWnd, &wSb);

  m_lv.SetColumnWidth(0, static_cast<INT_PTR>((RectCX(rcClient) * .80)));
  m_lv.SetColumnWidth(1, static_cast<INT_PTR>((RectCX(rcClient) * .20) - 5));

  m_lv.MoveWindow(0, 0, RectCX(rcClient), (RectCY(rcClient) - wSb.iHeight), TRUE);

  m_sb.SendMsg(WM_SIZE, 0, 0);
  m_sb.UpdateWindow();

  return FALSE;
}

BOOL FURRMainDlg::OnClose(WPARAM wParam, LPARAM lParam) {

  DestroyIcon(GetSmIcon());

  PostQuitMessage(0);
  return FALSE;
}

BOOL FURRMainDlg::OnNotify(WPARAM wParam, LPARAM lParam) {

  LPNMITEMACTIVATE pnmia = reinterpret_cast<LPNMITEMACTIVATE>(lParam);

  if (NULL != pnmia) {

    if (pnmia->hdr.code == NM_DBLCLK) {

      if (0 != OpenClipboard(m_hWnd)) {

        if (0 != EmptyClipboard()) {

          LVNODE node;
          TCHAR szNode[2048] = {0};

          node.iItem      = pnmia->iItem;
          node.fMask      = LVIF_TEXT;
          node.szText     = szNode;
          node.uiMaxChars = 2048;
          
          if (m_lv.GetNode(&node)) {

            HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, (_tcslen(szNode) * sizeof(TCHAR)) + sizeof(TCHAR));

            if (NULL != hMem) {

              PVOID pMem = GlobalLock(hMem);

              if (NULL != pMem) {

                CopyMemory(pMem, szNode, (_tcslen(szNode) * sizeof(TCHAR)) + sizeof(TCHAR));
                
                GlobalUnlock(hMem);

                UINT uiFormat = 0U;

#ifdef UNICODE

                uiFormat = CF_UNICODETEXT;

#else

                uiFormat = CF_TEXT;

#endif // !UNICODE

                if (NULL != SetClipboardData(uiFormat, hMem)) {

                  odsf(_T("Copied data to clipboard.\n"));

                }

              }

              GlobalFree(hMem);

            }

          }

        }

        CloseClipboard();
      }

    }

  }

  return FALSE;
}

BOOL FURRMainDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case ID_TOOLS_SESSIONSETTINGS:
    {
      AdjustParamsDlg dlg;

      if (!dlg.DoModal(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ADJUSTPARAMS), m_hWnd)) {

        MessageBox(m_hWnd, _T("Failed to create dialog!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }

      break;
    }

    case ID_TOOLS_CAMERACONTROLS:
    {
      CameraControlsDlg dlg(this, &g_dwFrameCount);

      if (!dlg.DoModal(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CAMERACONTROLS), m_hWnd)) {

        MessageBox(m_hWnd, _T("Failed to create dialog!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }

      break;
    }

    case ID_TOOLS_PREFERENCES:
    {
      PreferencesDlg dlg;

      if (!dlg.DoModal(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PREFERENCES), m_hWnd)) {

        MessageBox(m_hWnd, _T("Failed to create dialog!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }
    }
    break;

    case ID_FILE_NEWSESSION: 
    {

      _DoNewSession();
      
    }
    break;

    case ID_FILE_EXIT:
    {

      if (IDYES == MessageBox(m_hWnd, _T("Are you sure you wish to exit FURR?"), _T("FURR - Exit Confirmation"),
                              MB_YESNO | MB_ICONQUESTION))
      {

        PostQuitMessage(0);

      }

    }
    break;

    case ID_FILE_NEWWEDGESESSION:
    {
      _DoNewWedgeSession();
    }
    break;

    case ID_LOG_CLEARLOG:
    {

      if (0 == m_lv.GetCount()) {

        MessageBox(m_hWnd, _T("The log is already empty."), _T("FURR - Information"),
                   MB_OK | MB_ICONINFORMATION);

        break;

      }

      if (IDYES == MessageBox(m_hWnd, _T("Are you sure you wish to clear the log contents?"),
                              _T("FURR - Question"), MB_YESNO | MB_ICONQUESTION))
      {

        if (!m_lv.RemAllNodes()) {

          MessageBox(m_hWnd, _T("Failed to clear log contents!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

        }

      }

    }
    break;

    case ID_LOG_SAVELOG:
    {
      _SaveLogFile();

    }
    break;

  }

  return FALSE;
}

void FURRMainDlg::Output(FURR_OT type, TCHAR *szDetails, ...) {

  SYSTEMTIME st     = {0};
  TCHAR szTime[256] = {0};
  TCHAR szBuf[2048] = {0};
  va_list l         = {0};

  GetLocalTime(&st);

  wsprintf(szTime, _T("%02d:%02d:%02d - %02d/%02d/%04d"), st.wHour, st.wMinute, st.wSecond,
           st.wMonth, st.wDay, st.wYear);

  va_start(l, szDetails);
  _vsntprintf(szBuf, 2048, szDetails, l);
  va_end(l);

  LVNODE lvn;

  lvn.fMask   = LVIF_IMAGE | LVIF_TEXT;
  lvn.iItem   = m_lv.GetCount();
  lvn.szText  = szBuf;

  switch (type) {
    case FURR_OT_WARNING:
      lvn.iImage = 0;
    break;
    case FURR_OT_INFORMATION:
      lvn.iImage = 1;
    break;
    case FURR_OT_ERROR:
      lvn.iImage = 2;
    break;
  }

  int iNewItem = m_lv.AddNode(&lvn);

  lvn.fMask     = LVIF_TEXT;
  lvn.iItem     = iNewItem;
  lvn.szText    = szTime;
  lvn.iSubItem  = 1;

  m_lv.SetNode(&lvn);

  m_lv.EnsureVisible(iNewItem, true);

  /*
   * Depending on the type of event that
   * occurred, play a sound if necessary
   */
  if (FURR_OT_WARNING == type) {

    MessageBeep(MB_ICONEXCLAMATION);

  } else if (FURR_OT_ERROR == type) {

    MessageBeep(MB_ICONHAND);

  }

}

/*
 * FURRMainDlg : Private
 */
void FURRMainDlg::_SaveLogFile(void) {

  bool r = false;

  if (0 != m_lv.GetCount()) {

    FileDialog dlg;
    FILEDIALOG fd;

    fd.dwFlags        = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    fd.tsDlgTitle     = FMD_SAVELOGTITLE;
    fd.hWndParent     = m_hWnd;
    fd.tszFilter      = FMD_SAVELOGFILTER;

    INT_PTR iRes = FD_ERROR;

    if (FD_SUCCESS == (iRes = dlg.GetSaveFileName(&fd))) {

      TSTRING::reverse_iterator it = fd.tsFile.rend();
      TSTRING tsExt;

      for (int n = 0; n < 3; it--, n++) { tsExt += (*it); }

      StrToUpper(tsExt);

      if (_T("FLF") != tsExt) {

        fd.tsFile += _T(".flf");

      }

      HANDLE hFile = CreateFile(fd.tsFile.c_str(),
                                GENERIC_WRITE,
                                0UL,
                                NULL,
                                OPEN_ALWAYS,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);

      if (INVALID_HANDLE_VALUE != hFile) {

        INT_PTR iCount = m_lv.GetCount();
        LVNODE lvn;
        TCHAR pBuf[2048]  = {0};
        TCHAR pBuf2[256]  = {0};
        DWORD dwWritten   = 0;
        TSTRING tsWrite;

        lvn.fMask       = LVIF_TEXT;
        lvn.uiMaxChars  = 2048;

        for (int n = 0; n < iCount; n++) {

          lvn.iItem     = n;
          lvn.szText    = pBuf;
          lvn.iSubItem  = 0;
          
          m_lv.GetNode(&lvn);

          lvn.iSubItem    = 1;
          lvn.szText      = pBuf2;
          lvn.uiMaxChars  = 256;

          m_lv.GetNode(&lvn);

          tsWrite = pBuf2;
          tsWrite += "\t";
          tsWrite += pBuf;
          tsWrite += "\r\n";

          if (FALSE == WriteFile(hFile, tsWrite.c_str(), tsWrite.size() * sizeof(TCHAR), &dwWritten, NULL)
              && (dwWritten == tsWrite.size() * sizeof(TCHAR)))
          {

            goto lblFail;

          }

          RtlZeroMemory(pBuf, 2048 * sizeof(TCHAR));
          RtlZeroMemory(pBuf2, 256 * sizeof(TCHAR));
        }

        r = true;

lblFail:
        CloseHandle(hFile);

        if (r) {

          PrintMsgBox(m_hWnd, _T("FURR - Log file saved"), MB_OK | MB_ICONINFORMATION,
                      _T("Log successfully saved as\n\n%s\n"),
                      fd.tsFile.c_str());

        } else {

          PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONSTOP,
                      _T("Failed to save log file\n\n%s\n\nWin32 Error: %d\n"),
                      fd.tsFile.c_str(),
                      GetLastError());
        }

      }


    } else if (FD_CANCELLED == iRes) {

      r = true;

    }

  } else {

    MessageBox(m_hWnd, _T("There is no log data to save currently."), _T("FURR - Error"),
               MB_OK | MB_ICONSTOP);

  }

}

bool FURRMainDlg::_DoNewSession(void) {

  bool r = false;
  RECORDSESSION rs;
  ADJUSTPARAMS ap;
  TSTRING tsCurParamsPath;
  Serializer<ADJUSTPARAMS> s;

  GetCurrentParamsPath(tsCurParamsPath);

  if (s.Read(tsCurParamsPath.c_str(), &ap)) {

   CopyMemory(&rs.AP, &ap, sizeof(ADJUSTPARAMS));

   r = FURR_RunSession(rs);

  } else {

    PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONWARNING,
                _T("The parameters file:\n\n%s\n\n")
                _T("appears to be missing or corrupted.  FURR will now")
                _T(" attempt to load the default parameters file:\n\n%s"),
                tsCurParamsPath.c_str(),
                FURR_DEFIMAGEPARAMSFILE);

    if (s.Read(FURR_DEFIMAGEPARAMSFILE, &ap)) {

      CopyMemory(&rs.AP, &ap, sizeof(ADJUSTPARAMS));

      SetCurrentParamsPath(FURR_DEFIMAGEPARAMSFILE);

      r = FURR_RunSession(rs);

    } else {

      MessageBox(m_hWnd, _T("The default parameters file appears to be missing or corrupted!"),
                 _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    }

  }

  return r;
}

void FURRMainDlg::_DoNewWedgeSession(void) {

  // Temporary code
  // TODO: Put wedge session code here

  // End temporary code

}