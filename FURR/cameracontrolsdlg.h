#ifndef _CAMERACONTROLSDLG_H
#define _CAMERACONTROLSDLG_H

#include "stdafx.h"
#include "furramscomm.h"
#include "furrcappingshutter.h"

/*
 * CameraControlsDlg Class
 *
 * Provides a graphical interface
 * to manually controlling the camera
 * axis through the AMS driver.
 */
class CameraControlsDlg : public Dialog {

public:
  CameraControlsDlg(class FURRMainDlg *pParent, DWORD *pdwFrameCount) { 
  
    m_pParent       = pParent;
    m_pdwFrameCount = pdwFrameCount;

  }
  ~CameraControlsDlg() { }

  BOOL OnCreate(WPARAM wParam, LPARAM lParam);
  BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  BOOL OnDestroy(WPARAM wParam, LPARAM lParam);

private:

  bool _Wind(int iSteps, bool bForward = true);
  void _DoWind(bool bForward = true);
  void _DoZeroAccumulator(void);
  void _ThrowAMSError(void);

private:
  class FURRMainDlg  *m_pParent;
  DWORD              *m_pdwFrameCount;
  FURRAMSComm         m_AMSComm;
  FURRCappingShutter  m_CappingShutter;
  Window              m_WindFwdFrames;
  Window              m_WindRevFrames;

};

#endif // !_CAMERACONTROLSDLG_H
