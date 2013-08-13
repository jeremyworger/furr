#include "stdafx.h"
#include "previewwnd.h"
#include "loadermap.h"

extern LoaderMap g_LoaderMap;

/*
 * PreviewWnd : Public
 */

HWND PreviewWnd::Create(INT_PTR x, INT_PTR y, INT_PTR cx, INT_PTR cy, HWND hWndParent) {

  HWND r         = NULL;
  WNDCLASSEX wnd = {0};

  wnd.cbSize        = sizeof(WNDCLASSEX);
  wnd.hbrBackground = reinterpret_cast<HBRUSH>(NULL_BRUSH);
  wnd.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wnd.hInstance     = GetModuleHandle(NULL);
  wnd.lpfnWndProc   = Window::WindowProc;
  wnd.lpszClassName = PW_CLASSNAME;
  
  RegisterClassEx(&wnd);

  HDC hdcScreen = ::GetDC(NULL);

  m_hDCMem = CreateCompatibleDC(hdcScreen);

  ReleaseDC(NULL, hdcScreen);

  if (NULL != m_hDCMem) {

    r = Window::Create(WS_EX_TOOLWINDOW, PW_CLASSNAME, PW_CAPTION, WS_VISIBLE, x, y, cx, cy, hWndParent, 0);

  }

  return r;
}

LRESULT PreviewWnd::OnPaint(WPARAM wParam, LPARAM lParam) {

  PAINTSTRUCT ps = {0};
  HDC dc         = BeginPaint(m_hWnd, &ps);
  RECT rc        = {0};

  GetClientRect(&rc);

  BitBlt(dc, 0, 0, rc.right, rc.bottom, m_hDCMem, 0, 0, SRCCOPY);

  EndPaint(m_hWnd, &ps);

  return 0;
}

bool PreviewWnd::SetImage(CTSTRING &tsImgFile) {

  bool r          = false;
  FURRImage *pImg = NULL;

  if (g_LoaderMap.LoadImage(tsImgFile, &pImg)) {
    
    BITMAPINFO bi = {0};
    BYTE *pPixels = NULL;
    
    bi.bmiHeader.biSize         = sizeof(bi);
    bi.bmiHeader.biBitCount     = 32;
    bi.bmiHeader.biCompression  = BI_RGB;
    bi.bmiHeader.biPlanes       = 1;
    bi.bmiHeader.biWidth        = pImg->Width();
    bi.bmiHeader.biHeight       = pImg->Height();

    m_hBitmap = CreateDIBSection(m_hDCMem, &bi, DIB_RGB_COLORS, reinterpret_cast<void **>(&pPixels), NULL, 0);

    if (NULL != m_hBitmap) {

      CopyMemory(pPixels, pImg->Pixels(), pImg->Width() * pImg->Height() * 4);
      DeleteObject(SelectObject(m_hDCMem, m_hBitmap));

      InvalidateRect(NULL, true);
      UpdateWindow();
      r = true;

    }

    delete pImg;
  }

  return r;
}