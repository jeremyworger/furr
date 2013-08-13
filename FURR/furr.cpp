#include "stdafx.h"
#include "furr.h"

/*
 * Main dialog
 */
FURRMainDlg g_MainDlg;

/*
 * Image loader map
 */
LoaderMap g_LoaderMap;

/*
 * DirectDraw Display Window
 */
DirectDrawWnd g_DDDisplayWnd;

/*
 * DirectDraw Preview Window
 */
PreviewWnd g_DDPreviewWnd;

/*
 * Frame counter
 */
DWORD g_dwFrameCount;

/*
 * Entry point
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpCmdLine, int) {
 
  INITCOMMONCONTROLSEX iccs = {0};

  iccs.dwSize = sizeof(iccs);
  iccs.dwICC  = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS;

  if (FALSE == InitCommonControlsEx(&iccs)) {

    MessageBox(GetForegroundWindow(), _T("Failed to initialize common control libraries!"), 
               _T("FURR - Error"),
               MB_OK | MB_ICONSTOP);
    return 1;

  }

  if (FAILED(CoInitialize(NULL))) {

    MessageBox(GetForegroundWindow(), _T("Failed to initialize COM libraries!"),
               _T("FURR - Error"),
               MB_OK | MB_ICONSTOP);
    return 1;

  }

  MSG msg = {0};

  if (NULL == g_MainDlg.Create(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_FURRMAINDLG), NULL)) {

    MessageBox(GetForegroundWindow(), _T("Failed to create main window!"),
               _T("FURR - Error"),
               MB_OK | MB_ICONSTOP);
    return 1;

  }

  g_MainDlg.SetMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_FURRMAIN)));
  g_MainDlg.Show();
  g_LoaderMap.InitKnownFormats();

  HACCEL hAccel = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINACCELERATOR));

  while (GetMessage(&msg, NULL, 0, 0)) {
    
    //if (!IsDialogMessage(GetForegroundWindow(), &msg)) {

      if (!TranslateAccelerator(g_MainDlg.m_hWnd, hAccel, &msg)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);

      }

   // }

  }

 /*
  * We're outta here.
  */
  CoUninitialize();
  DestroyAcceleratorTable(hAccel);
  DestroyMenu(g_MainDlg.GetMenu());

  return msg.wParam;
}

bool FURR_RunSession(RECORDSESSION &rs) {

  bool r = false;

  SessionPrepareDlg spDlg;

  spDlg.SetRecordSessionData(&rs);

  if (spDlg.DoModal(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_SESSIONPREPARE), g_MainDlg.m_hWnd)) {

    switch (spDlg.GetStatus()) {

      case FAILED:
      {

        break;
      }
      case CANCELLED:
      {
        g_MainDlg.Output(FURR_OT_WARNING, _T("Image verification process halted by user!"));
        break;
      }
      case SUCCEEDED:
      {

        RunSessionDlg rsDlg;

        rsDlg.SetRecordSessionData(&rs);

        if (rsDlg.DoModal(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_RUNSESSION), g_MainDlg.m_hWnd)) {

          switch (rsDlg.GetStatus()) {

            case FAILED:
            {
              g_MainDlg.Output(FURR_OT_ERROR, _T("An error occurred during the recording session!"));
              break;
            }
            case CANCELLED:
            {
              g_MainDlg.Output(FURR_OT_WARNING, _T("Recording session cancelled by user!"));
              break;
            }
            case SUCCEEDED:
            {
              // TODO: Session succeeded
              break;
            }

          }

        }

        break;
      }

    }

  }

  return r;
}

