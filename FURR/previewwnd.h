#ifndef _PREVIEWWND_H
#define _PREVIEWWND_H

#include "stdafx.h"
#include "directdrawwnd.h"

class PreviewWnd : public DirectDrawWnd {

public:
  PreviewWnd() { }
  ~PreviewWnd() { }

  LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);

};

#endif // !_PREVIEWWND_H