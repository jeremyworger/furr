#ifndef _CAMERACONTROLSDLG_H
#define _CAMERACONTROLSDLG_H

#include "stdafx.h"
#include "cameracomm.h"

class CameraControlsDlg : public Dialog {

public:
  CameraControlsDlg() { }
  ~CameraControlsDlg() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  BOOL OnClose(WPARAM wParam, LPARAM lParam);

private:
  Window m_FrameCount;
  Window m_FastCount;
  Window m_ManualString;
  CameraComm m_Camera;
};

#endif // !_CAMERACONTROLSDLG_H