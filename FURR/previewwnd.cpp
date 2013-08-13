#include "stdafx.h"
#include "previewwnd.h"


/*
 * PreviewWnd : Public
 */

LRESULT PreviewWnd::OnMouseMove(WPARAM wParam, LPARAM lParam) {

  if (m_DDWI.bWindowed) {

    HDC hDC = GetDC();

    DWORD dwRGB = GetPixel(hDC, LOWORD(lParam), HIWORD(lParam));

    TSTRING tsCap;

    sprintf(tsCap, _T("%s [%d, %d, %d]"),
            m_DDWI.tsCaption.c_str(),
            GetRValue(dwRGB),
            GetGValue(dwRGB),
            GetBValue(dwRGB));

    SetWindowText(tsCap);
            
    ReleaseDC(m_hWnd, hDC);

  }

  return 0;
}