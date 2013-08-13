#include "stdafx.h"
#include "adjustparamsdlg.h"
#include "resource.h"
#include "previewwnd.h"

extern PreviewWnd g_DDPreviewWnd;

/*
 * APGlobalPage : Public
 */
BOOL APGlobalPage::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_Resolution.Attach(GetDlgItem(IDC_RESOLUTION));
  m_Resolution.InsertString(AP_FOURK);
  m_Resolution.InsertString(AP_TWOK);
  m_Resolution.InsertString(AP_TESTRES);

  m_Bitdepth.Attach(GetDlgItem(IDC_BITDEPTH));
  m_Bitdepth.InsertString(AP_24BPP);
  m_Bitdepth.InsertString(AP_32BPP);

  m_LUTFile.Attach(GetDlgItem(IDC_LUTFILE));

  return TRUE;
}

BOOL APGlobalPage::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDC_CHOOSELUT:
    {
      if (!_BrowseForLUTFile()) {

        MessageBox(m_hWnd, _T("Failed to create common dialog for file browsing!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }
    }
    break;
    case IDC_CLEARLUT: SetLUTFile(_T("")); break;

  }

  return FALSE;
}

bool APGlobalPage::SetLUTFile(CTSTRING &tsLUTFile) {

  bool r = true;
  TSTRING tsShortFile;

  m_tsLUTFile = tsLUTFile;

  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
      !Key.WriteString(FURR_CURLUTFILEVALUE, m_tsLUTFile))
  {
    MessageBox(m_hWnd, _T("Failed to save current LUT file path to registry!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);
  }

  if (_T("") != m_tsLUTFile) {

    GetShortFileName(m_tsLUTFile, tsShortFile);

  } else {

    tsShortFile = AP_FILENONE;

  }

  TSTRING tsDisplay;

  sprintf(tsDisplay, _T("LUT File: %s"), tsShortFile.c_str());

  m_LUTFile.SetWindowText(tsDisplay);

  return r;
}

bool APGlobalPage::InitFields(void) {

  bool r = true;

  // Resolution Combo Box
  TSTRING tsRes;

  ImgRes2StrRes(m_pAPGlobal->ImgRes, tsRes);
  m_Resolution.SelectString(tsRes);

  // Bitdepth Combo Box
  TSTRING tsBD;

  BitDepth2String(m_pAPGlobal->Depth, tsBD);
  m_Bitdepth.SelectString(tsBD);

  // LUT File
  SetLUTFile(m_pAPGlobal->szLUTFile);

  // Mode
  switch (m_pAPGlobal->Mode) {

    case CHANNELSPLIT:
      CheckDlgButton(m_hWnd, IDC_CHANNELSPLIT, BST_CHECKED);
      break;
    case COLORWHEEL:
      CheckDlgButton(m_hWnd, IDC_COLORWHEEL, BST_CHECKED);
      break;
    case FULLIMAGE:
      CheckDlgButton(m_hWnd, IDC_FULLIMAGE, BST_CHECKED);
      break;

  }

  return r;
}

bool APGlobalPage::SaveFields(void) {

  bool r = true;

  // Resolution Combo Box
  TSTRING tsRes;

  m_Resolution.GetItemText(m_Resolution.GetCurSel(), tsRes);
  m_pAPGlobal->ImgRes = StrRes2ImgRes(tsRes);

  // Bitdepth Combo Box
  TSTRING tsBD;

  m_Bitdepth.GetItemText(m_Bitdepth.GetCurSel(), tsBD);
  m_pAPGlobal->Depth  = String2BitDepth(tsBD);

  // LUT File
  ZeroMemory(m_pAPGlobal->szLUTFile, MAX_PATH);

  if (_T("") != m_tsLUTFile) {
    CopyMemory(m_pAPGlobal->szLUTFile, m_tsLUTFile.c_str(), m_tsLUTFile.size() * sizeof(TCHAR));
  }

  // Mode
  if (BST_CHECKED == IsDlgButtonChecked(m_hWnd, IDC_CHANNELSPLIT)) {

    m_pAPGlobal->Mode = CHANNELSPLIT;

  } else if (BST_CHECKED == IsDlgButtonChecked(m_hWnd, IDC_COLORWHEEL)) {

    m_pAPGlobal->Mode = COLORWHEEL;

  } else if (BST_CHECKED == IsDlgButtonChecked(m_hWnd, IDC_FULLIMAGE)) {

    m_pAPGlobal->Mode = FULLIMAGE;

  }

  return r;
}

/*
 * APGlobalPage : Private
 */
bool APGlobalPage::_BrowseForLUTFile(void) {

  bool r = false;
  FILEDIALOG fd;
  FileDialog dlg;

  fd.dwFlags    = OFN_HIDEREADONLY;
  fd.tsDlgTitle = AP_LUTDLG;
  fd.hInst      = GetModuleHandle(NULL);
  fd.hWndParent = m_hWnd;
  fd.tszFilter  = AP_LUTFILTER;

  INT_PTR iRes = FD_ERROR;

  if ((iRes = dlg.GetOpenFileName(&fd)) == FD_SUCCESS) {

    r = SetLUTFile(fd.tsFile);

  } else if (iRes == FD_CANCELLED) {

    r = true;

  }

  return r;
}

/*
 * APChannelPage : Public
 */
BOOL APChannelPage::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_Brightness.Attach(GetDlgItem(IDC_BRIGHTNESS));
  m_Brightness.SetRangeMin(-100);
  m_Brightness.SetRangeMax(100);
  m_Contrast.Attach(GetDlgItem(IDC_CONTRAST));
  m_Contrast.SetRangeMin(-100);
  m_Contrast.SetRangeMax(100);

  m_Gamma.Attach(GetDlgItem(IDC_GAMMA));
  m_ET.Attach(GetDlgItem(IDC_ET));
  m_ED.Attach(GetDlgItem(IDC_ED));

  return TRUE;
}

BOOL APChannelPage::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    default:
      return FALSE;
  }

  return FALSE;
}

bool APChannelPage::InitFields(void) {

  bool r = true;

  // Brightness slider
  m_Brightness.SetPos(m_pAPChannel->iBrightness);

  // Contrast slider
  m_Contrast.SetPos(m_pAPChannel->iContrast);

  // Gamma edit box
  SetFloatValue(m_pAPChannel->dGamma, m_Gamma.m_hWnd);

  // Exposure Time edit box
  SetDWORDValue(m_pAPChannel->dwET, m_ET.m_hWnd);

  return r;
}

bool APChannelPage::SaveFields(void) {

  bool r = true;

  // Brightness slider
  m_pAPChannel->iBrightness = m_Brightness.GetPos();

  // Contrast slider
  m_pAPChannel->iContrast   = m_Contrast.GetPos();

  // Gamma edit box
  m_pAPChannel->dGamma      = GetFloatValue(m_Gamma);

  // Exposure Time edit box
  m_pAPChannel->dwET        = GetDWORDValue(m_ET);

  return r;
}

/*
 * APFullImagePage : Public
*/
BOOL APFullImagePage::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_ET.Attach(GetDlgItem(IDC_ET));

  return TRUE;
}

BOOL APFullImagePage::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    default: return FALSE;
  }

  return FALSE;
}

bool APFullImagePage::InitFields(void) {

  bool r = true;

  // Exposure time edit box
  SetDWORDValue(m_pAPChannel->dwET, m_ET.m_hWnd);

  return r;
}

bool APFullImagePage::SaveFields(void) {

  bool r = true;

  m_pAPChannel->dwET = GetDWORDValue(m_ET);

  return r;
}

/*
 * APWedgePage : Public
 */
BOOL APWedgePage::OnCreate(WPARAM wParam, LPARAM lParam) {

  m_WedgeFile.Attach(GetDlgItem(IDC_WEDGEFILE));
  m_Steps.Attach(GetDlgItem(IDC_STEPS));
  m_StartMsec.Attach(GetDlgItem(IDC_STARTEXPOSURETIME));
  m_StepMsec.Attach(GetDlgItem(IDC_STEPEXPOSURETIME));
  m_EndMsec.Attach(GetDlgItem(IDC_ENDEXPOSURETIME));

  return TRUE;
}

BOOL APWedgePage::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDC_BROWSE:
    {
      if (!_BrowseForWedgeFile()) {

        PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONSTOP, _T("Failed to create file browsing dialog!")
                    _T(" Win32 Error: %d"), GetLastError());

      }

    }
    break;

    default:
      return FALSE;
  }

  return FALSE;
}

bool APWedgePage::InitFields(void) {

  bool r = true;

  // Wedge image file path
  _SetCurrentWedgeFile(m_pWedge->szWedgeFile);

  // # of steps
  SetDWORDValue(m_pWedge->dwSteps, m_Steps.m_hWnd);

  // Start exposure time
  SetDWORDValue(m_pWedge->dwStartMsec, m_StartMsec.m_hWnd);

  // Step exposure time
  SetDWORDValue(m_pWedge->dwStepMsec, m_StepMsec.m_hWnd);

  // End exposure time
  SetDWORDValue(m_pWedge->dwEndMsec, m_EndMsec.m_hWnd);

  return r;
}

bool APWedgePage::SaveFields(void) {

  bool r = true;

  // Wedge image file path is saved
  // when _BrowseForWedgeFile() returns

  // # of steps
  m_pWedge->dwSteps     = GetDWORDValue(m_Steps);

  // Start exposure time
  m_pWedge->dwStartMsec = GetDWORDValue(m_StartMsec);

  // Step exposure time
  m_pWedge->dwStepMsec  = GetDWORDValue(m_StepMsec);

  // End exposure time
  m_pWedge->dwEndMsec   = GetDWORDValue(m_EndMsec);

  return r;
}

/*
 * APWedgePage : Private
 */
bool APWedgePage::_BrowseForWedgeFile(void) {

  bool r = false;
  FILEDIALOG fd;
  FileDialog dlg;

  fd.dwFlags    = OFN_HIDEREADONLY;
  fd.tsDlgTitle = AP_WEDGEDLG;
  fd.hInst      = GetModuleHandle(NULL);
  fd.hWndParent = m_hWnd;
  fd.tszFilter  = AP_WDGEFILTER;

  INT_PTR iRes = FD_ERROR;

  if ((iRes = dlg.GetOpenFileName(&fd)) == FD_SUCCESS) {

    _SetCurrentWedgeFile(fd.tsFile);
    r = true;

  } else if (iRes == FD_CANCELLED) {

    r = true;

  }

  return r;
}

void APWedgePage::_SetCurrentWedgeFile(CTSTRING &tsWedgeFile) {

  if (_T("") != tsWedgeFile) {

    ZeroMemory(m_pWedge->szWedgeFile, MAX_PATH);
    CopyMemory(m_pWedge->szWedgeFile, tsWedgeFile.c_str(), tsWedgeFile.size() * sizeof(TCHAR));

  } else {

    ZeroMemory(m_pWedge->szWedgeFile, MAX_PATH);

  }

  TCHAR szCompactPath[MAX_PATH] = {0};

  if (0 < tsWedgeFile.size()) {

    if (!PathCompactPathEx(szCompactPath, m_pWedge->szWedgeFile, AP_NWEDGEPATH, 0UL)) {

      ZeroMemory(szCompactPath, MAX_PATH);
      CopyMemory(szCompactPath, AP_FILENONE, _tcslen(AP_FILENONE) * sizeof(TCHAR));

    }

  } else {

    ZeroMemory(szCompactPath, MAX_PATH);
    CopyMemory(szCompactPath, AP_FILENONE, _tcslen(AP_FILENONE) * sizeof(TCHAR));

  }

  m_WedgeFile.SetWindowText(szCompactPath);

}

/*
 * AdjustParamsDlg : Public
 */
BOOL AdjustParamsDlg::OnCreate(WPARAM wParam, LPARAM lParam) {

  // Populate and initialize TreeView
  _SetupControls();

  // Create child page dialogs
  if (!_CreatePages()) {

    MessageBox(m_hWnd, _T("An error occurred while creating a child property page!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

  }

  // Initialize setting structure pointers for each page
  m_GlobalPage.SetStructPointer(&m_Params.Global);
  m_RedPage.SetStructPointer(&m_Params.Red);
  m_GreenPage.SetStructPointer(&m_Params.Green);
  m_BluePage.SetStructPointer(&m_Params.Blue);
  m_FullPage.SetStructPointer(&m_Params.Full);
  m_WedgePage.SetStructPoitner(&m_Params.Wedge);

  // Read current page value from registry
  RegistryKey Key;
  DWORD_PTR dwCurPage;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
      !Key.ReadDWORD(FURR_CURAPSPAGEVALUE, &dwCurPage))
  {
    dwCurPage = IDD_AP_PAGEGLOBAL;
    odsf(_T("AdjustParamsDlg::OnCreate() : Failed to read current page value; defaulting to global\n"));
  }

  _SwitchToPage(static_cast<int>(dwCurPage), true);

  // Read saved settings
  TSTRING tsTemp;

  GetCurrentParamsPath(tsTemp);

  if (!_ReadSettings(tsTemp)) {

    PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONSTOP, _T("Failed to load parameters file [%s]! Reverting to")
                        _T(" defaults."),tsTemp.c_str());

    _LoadDefaultParamsFile();

  }

  // Create and display preview window
  // REMOVED: 1/13/2004 10:11:09 PM
 /* RECORDSESSION rs;
  DDRAWWNDINIT ddwi;

  ddwi.bPrimaryDisplay = true;
  ddwi.bWindowed       = true;
  ddwi.bInitVisible    = false;
  ddwi.rcSize.left     = 0;
  ddwi.rcSize.top      = 0;
  ddwi.rcSize.right    = AP_PREVIEWX;
  ddwi.rcSize.bottom   = AP_PREVIEWY;
  ddwi.tsCaption       = AP_PREVIEWCAP;
  
  if (!g_DDPreviewWnd.Display(ddwi, rs)) {

    PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONSTOP, _T("Failed to create preview window! Error = %d"),
                GetLastError());

  }

  RECT rc = {0};

  GetWindowRect(&rc);

  g_DDPreviewWnd.MoveWindow(rc.right + 10, rc.top, AP_PREVIEWX, AP_PREVIEWY, true);
  g_DDPreviewWnd.Show();*/

  return TRUE;
}

BOOL AdjustParamsDlg::OnCommand(WPARAM wParam, LPARAM lParam) {

  switch (LOWORD(wParam)) {

    case IDCANCEL: EndDialog(1); break;
    case IDOK:
    {
      TSTRING tsTemp;

      GetCurrentParamsPath(tsTemp);

      if (!_WriteSettings(tsTemp)) {

        PrintMsgBox(m_hWnd, _T("FURR - Error"), MB_OK | MB_ICONSTOP, _T("Failed to save parameters file [%s]!\n"), tsTemp.c_str());
        break;
      }
      EndDialog(0);
    }
    break;
    case IDC_LOADPARAMS:
    {

      if (!_LoadFile()) {

        MessageBox(m_hWnd, _T("Failed to load parameters file!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }

    }
    break;
    case IDC_SAVEPARAMS:
    {

      if (!_SaveFile()) {

        MessageBox(m_hWnd, _T("Failed to save parameters file!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

      }

    }
    break;
    case IDC_LOADDEFAULTS:
    {

      _LoadDefaultParamsFile();

    }
    break;
    // REMOVED: 1/13/2004 10:13:42 PM
    //case IDC_UPDATEPREVIEW:
    //{
    //  TSTRING tsCurImg;

    //  GetCurrentPreviewImage(tsCurImg);

    //  if (_T("") == tsCurImg) {

    //    MessageBox(m_hWnd, _T("No preview image is currently selected.  Use the 'Choose Preview' button to select one."),
    //                       _T("FURR - Information"), MB_OK | MB_ICONINFORMATION);

    //    break;
    //  }

    //  _WriteCurrentSettings();

    //  if (!_DisplayPreviewImage(tsCurImg)) {

    //    MessageBox(m_hWnd, _T("Failed to load preview image file!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    //  }
    //}
    //break;
    //case IDC_CHOOSEPREVIEW:
    //{
    // 
    //  if (!_LoadPreviewImage()) {

    //    MessageBox(m_hWnd, _T("Failed to load preview image file!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    //  }

    //}
    //break;

  }

  return FALSE;
}

BOOL AdjustParamsDlg::OnNotify(WPARAM wParam, LPARAM lParam) {

  LPNMHEADER lpnmh = reinterpret_cast<LPNMHEADER>(lParam);

  if (NULL != lpnmh) {

    if (IDC_TREEVIEW == wParam && TVN_SELCHANGED == lpnmh->hdr.code) {

      LPNMTREEVIEW lpnmtv = reinterpret_cast<LPNMTREEVIEW>(lpnmh);
      PAPTVDATA paptvd    = reinterpret_cast<PAPTVDATA>(m_TreeView.HandleToData(lpnmtv->itemNew.hItem));

      if (NULL != paptvd) {
        _SwitchToPage(paptvd->Key);
        odsf(_T("AdjustParamsDlg::OnNotify() : TV item selected: %d\n"), paptvd->Key);
      }
    }

  }

  return FALSE;
}

BOOL AdjustParamsDlg::OnClose(WPARAM wParam, LPARAM lParam) {

  EndDialog(0);

  return FALSE;
}

BOOL AdjustParamsDlg::OnDestroy(WPARAM wParam, LPARAM lParam) {

  g_DDPreviewWnd.Destroy();

  return FALSE;
}

/*
 * AdjustParamsDlg : Private
 */
void AdjustParamsDlg::_SetupControls(void) {

  m_TreeView.Attach(GetDlgItem(IDC_TREEVIEW));

  APTVNODE node;

  node.Data.Key   = IDD_AP_PAGEGLOBAL;
  node.fMask      = TVIF_TEXT;
  node.hIns       = TVI_ROOT;
  node.tsText     = _T("Global Settings");

  m_TreeView.AddNode(&node);

  node.Data.Key   = 0;
  node.fMask      |= TVIF_STATE;
  node.fState     = node.fStateMask = TVIS_EXPANDED;
  node.tsText     = _T("Adjustment Settings");

  m_TreeView.AddNode(&node);

  node.Data.Key   = IDD_AP_PAGEFULLIMAGE;
  node.hIns       = TVI_LAST;
  node.hOwner     = m_TreeView.KeyToHandle(0);
  node.tsText     = _T("Full Image");

  m_TreeView.AddNode(&node);

  node.Data.Key   = IDD_AP_PAGERED;
  node.hIns       = TVI_LAST;
  node.hOwner     = m_TreeView.KeyToHandle(0);
  node.tsText     = _T("Red Channel");

  m_TreeView.AddNode(&node);

  node.Data.Key   = IDD_AP_PAGEGREEN;
  node.tsText     = _T("Green Channel");

  m_TreeView.AddNode(&node);

  node.Data.Key   = IDD_AP_PAGEBLUE;
  node.tsText     = _T("Blue Channel");

  m_TreeView.AddNode(&node);

  node.Data.Key   = IDD_AP_PAGEWEDGE;
  node.hIns       = TVI_ROOT;
  node.fMask      = TVIF_TEXT;
  node.fState     = node.fStateMask = 0;
  node.hOwner     = TVI_ROOT;
  node.tsText     = _T("Wedge Settings");

  m_TreeView.AddNode(&node);

}

bool AdjustParamsDlg::_ReadSettings(CTSTRING &tsConfPath) {

  bool r = false;

  if (m_Serializer.Read(tsConfPath.c_str(), &m_Params)) {

    if (m_GlobalPage.InitFields()) {
    
      if (m_RedPage.InitFields()) {

        if (m_GreenPage.InitFields()) {

          if (m_BluePage.InitFields()) {

            if (m_FullPage.InitFields()) {

              if (m_WedgePage.InitFields()) {

                TSTRING tsShortPath;

                GetShortFileName(tsConfPath, tsShortPath);
                SetWindowText(_T("FURR - Film Parameters <") + tsShortPath + _T(">"));

                r = true;

              }

            }

          }

        }

      }

    }

  }

  return r;
}

bool AdjustParamsDlg::_WriteSettings(CTSTRING &tsConfPath) {

  bool r = false;

  if (m_GlobalPage.SaveFields()) {

    if (m_RedPage.SaveFields()) {

      if (m_GreenPage.SaveFields()) {

        if (m_BluePage.SaveFields()) {

          if (m_FullPage.SaveFields()) {

            if (m_WedgePage.SaveFields()) {

              if (m_Serializer.Write(tsConfPath.c_str(), &m_Params)) {

                TSTRING tsShortPath;

                GetShortFileName(tsConfPath, tsShortPath);
                SetWindowText(_T("FURR - Film Parameters <") + tsShortPath + _T(">"));

                r = true;

              }

            }

          }

        }

      }

    }

  }

  return r;
}

void AdjustParamsDlg::_LoadDefaultParamsFile(void) {

  if (!_ReadSettings(FURR_DEFIMAGEPARAMSFILE)) {

    MessageBox(m_hWnd, _T("Failed to load default parameters file! Is it missing?"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);

  } else {

    if (!SetCurrentParamsPath(FURR_DEFIMAGEPARAMSFILE)) {

      MessageBox(m_hWnd, _T("Failed to write default parameters file path to registry.")
                         _T(" Default settings will NOT be available!"),
                         _T("FURR - Error"), MB_OK | MB_ICONSTOP);

    }

  }

}

bool AdjustParamsDlg::_SaveFile(void) {

  bool r = false;
  FILEDIALOG fd;
  FileDialog dlg;

  fd.dwFlags    = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  fd.tsDlgTitle = AP_SAVEDLG;
  fd.hInst      = GetModuleHandle(NULL);
  fd.hWndParent = m_hWnd;
  fd.tszFilter  = AP_FILEFILTER;

  INT_PTR iRes = FD_ERROR;

  if ((iRes = dlg.GetSaveFileName(&fd)) == FD_SUCCESS) {

   if (_WriteSettings(fd.tsFile)) {

     if (SetCurrentParamsPath(fd.tsFile)) {
      
        r = true;

     }

   }


  } else if (iRes == FD_CANCELLED) {

    r = true;

  }

  return r;
}

bool AdjustParamsDlg::_LoadFile(void) {

  bool r = false;
  FILEDIALOG fd;
  FileDialog dlg;

  fd.dwFlags    = OFN_HIDEREADONLY;
  fd.tsDlgTitle = AP_LOADDLG;
  fd.hInst      = GetModuleHandle(NULL);
  fd.hWndParent = m_hWnd;
  fd.tszFilter  = AP_FILEFILTER;

  INT_PTR iRes = FD_ERROR;

  if ((iRes = dlg.GetOpenFileName(&fd)) == FD_SUCCESS) {

    if (_ReadSettings(fd.tsFile)) {

      if (SetCurrentParamsPath(fd.tsFile)) {

        r = true;

      }

    }

  } else if (iRes == FD_CANCELLED) {

    r = true;

  }

  return r;
}

// REMOVED: 1/13/2004 10:16:43 PM
//bool AdjustParamsDlg::_LoadPreviewImage(void) {
//
//  bool r = false;
//  FILEDIALOG fd;
//  FileDialog dlg;
//
//  fd.dwFlags    = OFN_HIDEREADONLY;
//  fd.tsDlgTitle = AP_PREVIEWDLG;
//  fd.hInst      = GetModuleHandle(NULL);
//  fd.hWndParent = m_hWnd;
//  fd.tszFilter  = AP_IMGFILTER;
//
//  INT_PTR iRes = FD_ERROR;
//
//  if ((iRes = dlg.GetOpenFileName(&fd)) == FD_SUCCESS) {
//
//    if (_DisplayPreviewImage(fd.tsFile)) {
//
//      _SetCurrentPreviewImage(fd.tsFile);
//      r = true;
//
//    }
//
//  } else if (iRes == FD_CANCELLED) {
//
//    r = true;
//
//  }
//
//  return r;
//}

// REMOVED: 1/13/2004 10:16:57 PM
//bool AdjustParamsDlg::_DisplayPreviewImage(CTSTRING &tsImgFile) {
//
//  FRAME f;
//
//  f.tsFileName = tsImgFile;
//
//  return g_DDPreviewWnd.SetImage(f);
//}

// REMOVED: 1/13/2004 10:17:49 PM
//void AdjustParamsDlg::_SetCurrentPreviewImage(CTSTRING &tsPath) {
//
//  RegistryKey Key;
//
//  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
//      !Key.WriteString(FURR_CURPREVIEWIMAGEVALUE, tsPath))
//  {
//    MessageBox(m_hWnd, _T("Failed to set current preview image path!"), _T("FURR - Error"), MB_OK | MB_ICONSTOP);
//  }
//
//}

void AdjustParamsDlg::_SetCurrentPageValue(int iPageID) {

  RegistryKey Key;

  if (!Key.Create(FURR_ROOTREGISTRYKEY, FURR_REGISTRYCONFIGSPATH) ||
      !Key.WriteDWORD(FURR_CURAPSPAGEVALUE, static_cast<DWORD>(iPageID)))
  {
    odsf(_T("AdjustParamsDlg::_WriteCurrentSettings() : Failed to save current page value.\n"));
  }

}

bool AdjustParamsDlg::_CreatePages(void) {

  bool r = false;

  if (m_GlobalPage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGEGLOBAL), m_hWnd)) {

    if (m_RedPage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGERED), m_hWnd)) {

      if (m_GreenPage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGEGREEN), m_hWnd)) {

        if (m_BluePage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGEBLUE), m_hWnd)) {

          if (m_FullPage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGEFULLIMAGE), m_hWnd)) {

            if (m_WedgePage.Create(GetInstance(), MAKEINTRESOURCE(IDD_AP_PAGEWEDGE), m_hWnd)) {

              WINDOWING w;
              INT_PTR x;
              INT_PTR y;

              m_TreeView.GetCoords(&w);

              x = w.rcClient.right + 1;
              y = w.rcClient.top;

              m_GlobalPage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
              m_RedPage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
              m_GreenPage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
              m_BluePage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
              m_FullPage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
              m_WedgePage.SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

              r = true;

            }

          }

        }

      }

    }

  }

  return r;
}

void AdjustParamsDlg::_SwitchToPage(int iPageID, bool bSelNode) {

  switch (iPageID) {

    case 0:
    case IDD_AP_PAGEGLOBAL:

      m_GlobalPage.Show();
      m_RedPage.Hide();
      m_GreenPage.Hide();
      m_BluePage.Hide();
      m_FullPage.Hide();
      m_WedgePage.Hide();

    break;
    case IDD_AP_PAGERED:

      m_GlobalPage.Hide();
      m_RedPage.Show();
      m_GreenPage.Hide();
      m_BluePage.Hide();
      m_FullPage.Hide();
      m_WedgePage.Hide();

    break;
    case IDD_AP_PAGEGREEN:

      m_GlobalPage.Hide();
      m_RedPage.Hide();
      m_GreenPage.Show();
      m_BluePage.Hide();
      m_FullPage.Hide();
      m_WedgePage.Hide();

    break;
    case IDD_AP_PAGEBLUE:

      m_GlobalPage.Hide();
      m_RedPage.Hide();
      m_GreenPage.Hide();
      m_BluePage.Show();
      m_FullPage.Hide();
      m_WedgePage.Hide();

    break;
    case IDD_AP_PAGEFULLIMAGE:

      m_GlobalPage.Hide();
      m_RedPage.Hide();
      m_GreenPage.Hide();
      m_BluePage.Hide();
      m_FullPage.Show();
      m_WedgePage.Hide();
    break;
    case IDD_AP_PAGEWEDGE:

      m_GlobalPage.Hide();
      m_RedPage.Hide();
      m_GreenPage.Hide();
      m_BluePage.Hide();
      m_FullPage.Hide();
      m_WedgePage.Show();

    break;

  }

  _SetCurrentPageValue(iPageID);

  if (bSelNode) {

    m_TreeView.SetSelectedNode(iPageID, TVGN_CARET);

  }

}

/*
 * Global Functions
 */
double GetFloatValue(Window &Wnd) {

  double r = 0.0;
  TSTRING tsText;

  if (Wnd.GetWindowText(tsText)) {

    r = _tstof(tsText.c_str());

  }

  return r;
}

bool SetFloatValue(double dFloat, HWND hWnd) {

  bool r = false;
  TSTRING tsText;

  sprintf(tsText, _T("%.02f"), dFloat);

  return WND_TRUE(::SetWindowText(hWnd, tsText.c_str()));
}

DWORD GetDWORDValue(Window &Wnd) {

  DWORD r = 0UL;
  TSTRING tsText;

  if (Wnd.GetWindowText(tsText)) {

    r = static_cast<DWORD>(_tstoi(tsText.c_str()));

  }

  return r;
}

bool SetDWORDValue(DWORD dwValue, HWND hWnd) {

  bool r = false;
  TSTRING tsText;

  sprintf(tsText, _T("%d"), dwValue);

  return WND_TRUE(::SetWindowText(hWnd, tsText.c_str()));
}
