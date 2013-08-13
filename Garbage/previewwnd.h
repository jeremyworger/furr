#ifndef _PREVIEWWND_H
#define _PREVIEWWND_H

#include "stdafx.h"

/*
 * Constants
 */
#define PW_CLASSNAME  _T("FURR_PreviewWnd")
#define PW_CAPTION    _T("FURR - Image Adjust Preview")

/*
 * PreviewWnd Class
 *
 * Provides a preview image
 * used while adjusting image
 * parameters for a session
 *
 */
class PreviewWnd : public Window {

public:
  PreviewWnd() {}
  ~PreviewWnd() {
  
    DeleteObject(m_hBitmap);
    DeleteDC(m_hDCMem);

  }

  HWND Create(INT_PTR x, INT_PTR y, INT_PTR cx, INT_PTR cy, HWND hWndParent);
  LRESULT OnPaint(WPARAM wParam, LPARAM lParam);

  bool SetImage(CTSTRING &tsImgFile);

private:
  HBITMAP m_hBitmap;
  HDC m_hDCMem;

};

#endif // !_PREVIEWWND_H
