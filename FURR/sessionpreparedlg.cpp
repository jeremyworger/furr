#include "stdafx.h"
#include "sessionpreparedlg.h"
#include "furrmaindlg.h"
#include "loadermap.h"
#include "adjustparams.h"

extern FURRMainDlg g_MainDlg;
extern LoaderMap   g_LoaderMap;

/*
 * SessionPrepareDlg : Public
 */
BOOL SessionPrepareDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  JobStatus Status;

  if ((NULL != m_pRS) && _BrowseForFolder(Status)) {

    if (_SpawnThread()) {

      SetWindowText(SPD_TITLE);

      m_pb.Attach(GetDlgItem(IDC_PROGRESS));
      m_st.Attach(GetDlgItem(IDC_STATUSTEXT));
      m_cb.Attach(GetDlgItem(IDCANCEL));
      m_db.Attach(GetDlgItem(IDC_DONE));
      m_db.Disable();

    } else {

      g_MainDlg.Output(FURR_OT_ERROR, _T("SessionPrepare: An error occurred while spawning a worker thread; Error: %d"),
                       GetLastError());
      Die(FAILED);

    }
  
  }

  if (SUCCEEDED != Status) { Die(Status); }

  return TRUE;
}

BOOL SessionPrepareDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch(LOWORD(wParam)) {

    case IDCANCEL:
    {

      m_cb.Disable();
      Die(CANCELLED);

      break;
    }
    case IDC_DONE:
    {

      Die(SUCCEEDED);

      break;
    }

    default:
      return FALSE;
  }

  return FALSE;
}

bool SessionPrepareDlg::SetStatusText(CTSTRING &tsText) {

  return SetDlgItemText(IDC_STATUSTEXT, tsText);

}

void SessionPrepareDlg::SetMaxFileNum(UINT_PTR uiMaxFiles) {

  m_uiMaxFiles = uiMaxFiles;

}

void SessionPrepareDlg::SetProgress(UINT_PTR uiProgress) {

  if (0 < m_uiMaxFiles) {

    UINT_PTR n = ((100 * uiProgress) / m_uiMaxFiles);

    m_pb.SendMsg(PBM_SETPOS, static_cast<INT_PTR>(n), 0);

    TSTRING tsNewTitle;

    sprintf(tsNewTitle, _T("%s%d%%"),
            SPD_TITLE,
            n);

    SetWindowText(tsNewTitle);

    if (100 == n) {

      m_db.Enable();

    }

  }

}

void SessionPrepareDlg::Die(JobStatus Status) {

  if (CANCELLED == Status) {

    if (TRUE == SetEvent(m_hEvent)) {

      while (WAIT_OBJECT_0 != WaitForSingleObject(m_hThread, 0UL)) { 

        DoEvents(NULL, 0, 0, PM_REMOVE);

      }

    } else {

      if (NULL != m_hEvent) {

        g_MainDlg.Output(FURR_OT_ERROR, _T("PrepareSession: Failed to signal thread terminating event!"));

      }

    }

  }

  m_Status = Status;

  EndDialog(0);

}

/*
 * SessionPrepareDlg : Private
 */
bool SessionPrepareDlg::_SpawnThread(void) {

  bool r = false;

  m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

  if (NULL != m_hEvent) {

    PPREPARESESSIONTHREAD ppst = new PREPARESESSIONTHREAD;

    if (NULL != ppst) {

      ppst->pDlg   = this;
      ppst->hEvent = m_hEvent;
      ppst->pRS    = m_pRS;

      m_hThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, _PrepareSessionThread, ppst, 0, &m_uiThread));

      if (NULL != m_hThread) {

        r = true;

      } else {

        delete ppst;

      }

    }

  }

  return r;
}

unsigned __stdcall SessionPrepareDlg::_PrepareSessionThread(void *pVoid) {

  PREPARESESSIONTHREAD pst;
  
  RtlCopyMemory(&pst, pVoid, sizeof(PREPARESESSIONTHREAD));

  delete pVoid;

  FRAME f;
  UINT_PTR n = 0U;
  UINT_PTR e = 0U;

  pst.pDlg->SetMaxFileNum(pst.pRS->FrameCount());

  pst.pRS->Rewind();

  g_MainDlg.Output(FURR_OT_INFORMATION, _T("Parsing directory '%s' for image(s)..."),
                   pst.pRS->Directory().c_str());

  while (pst.pRS->EnumFrames(f)) {

    if (WAIT_OBJECT_0 == WaitForSingleObject(pst.hEvent, 0UL)) {

      _endthreadex(1U);

    }

    n++;

    TSTRING tsStatusText;
    TSTRING tsShortFile;

    GetShortFileName(f.tsFileName, tsShortFile);

    tsStatusText += tsShortFile;

    sprintf(tsStatusText, _T("Verifying image: '%s' (%d of %d)"),
            tsShortFile.c_str(),
            n,
            pst.pRS->FrameCount());
    
    pst.pDlg->SetStatusText(tsStatusText);
    pst.pDlg->SetProgress(n);

    if (!pst.pDlg->_VerifyImage(f.tsFileName)) {

      e++;

    }
    
  }

  if (0U == e) {

    g_MainDlg.Output(FURR_OT_INFORMATION, _T("%d image file(s) successfully verified and ready to shoot."),
                     pst.pRS->FrameCount());

    TSTRING tsNewTitle;

    sprintf(tsNewTitle, _T("%s%s"), SPD_TITLE, SPD_FINISHED);

    pst.pDlg->SetWindowText(tsNewTitle);

    MessageBeep(MB_ICONEXCLAMATION);

    FLASHWINFO fwi = {0};

    fwi.cbSize    = sizeof(FLASHWINFO);
    fwi.hwnd      = pst.pDlg->m_hWnd;
    fwi.uCount    = 10;
    fwi.dwTimeout = 0;
    fwi.dwFlags   = FLASHW_CAPTION;

    FlashWindowEx(&fwi);

    _endthreadex(0U);

  } else {

    g_MainDlg.Output(FURR_OT_ERROR, _T("%d image file(s) failed to be validated;")
                                    _T(" check previous error(s) and try again."),
                                    e);

    pst.pDlg->Die(FAILED);
    MessageBeep(MB_ICONHAND);
    _endthreadex(1U);

  }

  return 0U;
}

bool SessionPrepareDlg::_BrowseForFolder(JobStatus &Status) {

  bool r = false;

  Status = SUCCEEDED;

  BROWSEINFO bi           = {0};
  TCHAR szPath[MAX_PATH]  = {0};

  bi.hwndOwner      = g_MainDlg.m_hWnd;
  bi.pszDisplayName = szPath;
  bi.lpszTitle      = _T("Choose the directory that contains the frames to record.");
  bi.ulFlags        = BIF_NEWDIALOGSTYLE | BIF_SHAREABLE | BIF_NONEWFOLDERBUTTON | BIF_UAHINT;
  bi.lpfn           = _BrowseCallback;

  LPITEMIDLIST pil = SHBrowseForFolder(&bi);

  if (NULL != pil) {

    IMalloc *pMalloc = NULL;

    if (SUCCEEDED(SHGetMalloc(&pMalloc))) {

      if (FALSE != SHGetPathFromIDList(pil, szPath)) {

        WIN32_FIND_DATA wfd = {0};
        TSTRING tmp         = szPath;
        TSTRING pth         = szPath;

        if (tmp.end() != _T("\\")) {

          tmp += _T('\\');
          pth += _T('\\');

        }
        
        tmp += _T("*.*");

        m_pRS->Rewind();
        m_pRS->tsDirectory = szPath;

        RegistryKey Key;

        if (!Key.Open(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
            !Key.WriteString(FURR_LASTFRAMESDIRVALUE, szPath))
        {

          odsf(_T("SessionPrepareDlg::_BrowseForFolder() :")
               _T(" unable to save last directory path! Win32 Error = %d\n"),
               GetLastError());
        }

        Key.Close();

        HANDLE hFindFile = FindFirstFile(tmp.c_str(), &wfd);

        if (INVALID_HANDLE_VALUE != hFindFile) {

          do {

            if (wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {

              if ((0 != _tcscmp(wfd.cFileName, _T("."))) && (0 != _tcscmp(wfd.cFileName, _T("..")))) {

                FRAME f;

                f.tsFileName = pth + wfd.cFileName;
                m_pRS->lstFrames.push_back(f);

              }

            }

          } while (FindNextFile(hFindFile, &wfd));

          if (0 < m_pRS->lstFrames.size()) {

            m_pRS->_it  = m_pRS->lstFrames.begin();
            r           = true;

          } else {

            g_MainDlg.Output(FURR_OT_WARNING, _T("The directory '%s' contains no files. Session aborted."),
                              m_pRS->tsDirectory.c_str());
            Status = FAILED;

          }

          FindClose(hFindFile);

        }

      } 

      pMalloc->Free(pil);
      pMalloc->Release();

    }

  } else {

    Status = CANCELLED;
  
  }

  return r;
}

int CALLBACK SessionPrepareDlg::_BrowseCallback(HWND hWnd, UINT uiMsg, LPARAM lParam, LPARAM lpData) {

  switch (uiMsg) {

    case BFFM_INITIALIZED:
    {

      TSTRING tsLastDir;
      RegistryKey Key;

      if (!Key.Open(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
          !Key.ReadString(FURR_LASTFRAMESDIRVALUE, tsLastDir))
      {

        odsf(_T("SessionPrepareDlg::_BrowseCallback() :")
             _T(" Failed to read last directory from registry!\n"));
        break;

      }

      if (0 < tsLastDir.size()) {

        wchar_t *pwszPath = NULL;

#ifdef UNICODE

        pswszPath = tsLastDir.c_str();

#else

        wchar_t pWidePath[MAX_PATH] = {0};

        if (-1 == mbstowcs(pWidePath, tsLastDir.c_str(), MAX_PATH)) {

          odsf(_T("SessionPrepareDlg::_BrowseCallback() :")
              _T(" Failed to convert path to wide characters!\n"));
          break;

        }

        pwszPath = pWidePath;

#endif // !UNICODE

        SendMessage(hWnd, BFFM_SETEXPANDED, TRUE, reinterpret_cast<LPARAM>(pwszPath));

      }

    }
    break;


  }

  return 0;
}

bool SessionPrepareDlg::_VerifyImage(CTSTRING &tsImg) {

  bool r          = false;
  FURRImage *pImg = NULL;

  validus_t vs = {0};

  const char *szFileName = NULL;

#ifdef UNICODE

  char szAnsiFileName[MAX_PATH] = {0};

  if (-1 == wcstombs(szAnsiFileName, tsImg.c_str(), MAX_PATH)) {

    odsf(_T("SessionPrepareDlg::_VerifyImage() :")
          _T(" wcstombs() failed.\n"));
    
  } else {

    szFileName = szAnsiFileName;

  }

#else

  szFileName = tsImg.c_str();

#endif // !UNICODE

  if (NULL != szFileName) {

    if (0 != Validus_HashFile(&vs, szFileName)) {

      odsf(_T("SessionPrepareDlg::_VerifyImage() :")
            _T(" Validus_HashFile('%s') failed.\n"),
            szFileName);

      RtlZeroMemory(&vs, sizeof(vs));

    }

  }

  char szFingerprint[49] = {0};

  Validus_FingerprintToString(&vs, szFingerprint);

  TSTRING tsShortFile;

  GetShortFileName(tsImg, tsShortFile);

  if (g_LoaderMap.LoadImage(tsImg, &pImg)) {

    if (pImg->IsValid()) {

      if (pImg->Width() != ImgRes2NumericX(m_pRS->AP.Global.ImgRes) || pImg->Height() != ImgRes2NumericY(m_pRS->AP.Global.ImgRes)) {

        g_MainDlg.Output(FURR_OT_WARNING, _T("Image '%s' {%s} has invalid dimensions; {%dx%d} not ")
                                          _T("{%dx%d}."),
                                          tsShortFile.c_str(),
                                          szFingerprint,
                                          pImg->Width(),
                                          pImg->Height(),
                                          ImgRes2NumericX(m_pRS->AP.Global.ImgRes),
                                          ImgRes2NumericY(m_pRS->AP.Global.ImgRes));

        SAFE_DELETE(pImg);
        return false;

      } else {

        g_MainDlg.Output(FURR_OT_INFORMATION, _T("Image '%s' {%s} OK for shooting."),
                        tsShortFile.c_str(),
                        szFingerprint);

        SAFE_DELETE(pImg);
      }

    } else {

      g_MainDlg.Output(FURR_OT_WARNING, _T("Image '%s' {%s} is not a valid image file."), 
                      tsShortFile.c_str(),
                      szFingerprint);

      SAFE_DELETE(pImg);
      return false;
    }

  } else {

    g_MainDlg.Output(FURR_OT_WARNING, _T("File '%s' {%s} is not of a recognized file type!"), 
                    tsShortFile.c_str(),
                    szFingerprint);

    SAFE_DELETE(pImg);
    return false;

  }

  return true;
}
