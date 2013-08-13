#ifndef _CAMERACOMM_H
#define _CAMERACOMM_H

#include "serialcomm.h"

/*
 * Constants
 */
#define CAMERACOMM_FWD    _T("FORWARD\r")
#define CAMERACOMM_REV    _T("REVERSE\r")
#define CAMERACOMM_STEP   _T("A1\r")
#define CAMERACOMM_ADV    _T("A")
#define CAMERACOMM_ID     _T("ID")    // Speed
#define CAMERACOMM_IE     _T("IE")    // Pause
#define CAMERACOMM_SHOOT  _T("S1\r")
#define CAMERACOMM_PORT   _T("COM1")

/*
 * CameraComm Class
 *
 * Derives from SerialComm
 * to provide high-level
 * interface for camera driver
 * communication.
 */
class CameraComm : public SerialComm {

public:
  CameraComm() { }
  ~CameraComm() { }

  bool Open(void) { return SerialComm::Open(CAMERACOMM_PORT); }
  bool Shoot(void) { return Communicator::Write(CAMERACOMM_SHOOT); }
  bool SetDirection(bool bForward = true) {

    if (bForward) {

      return Communicator::Write(CAMERACOMM_FWD);

    } else {

      return Communicator::Write(CAMERACOMM_REV);

    }

  }
  bool Step(void) { return Communicator::Write(CAMERACOMM_STEP); }
  bool Advance(CTSTRING &tsFrames) {

    TSTRING tsAdvance = CAMERACOMM_ADV;
    
    tsAdvance += tsFrames;
    tsAdvance += _T("\r");

    return Communicator::Write(tsAdvance);
  }
  bool SetSpeed(DWORD dwSpeed) {

    TSTRING tsSpeed;

    sprintf(tsSpeed, _T("%s%d\r"), CAMERACOMM_ID, dwSpeed);

    return Communicator::Write(tsSpeed);
  }
  bool SetDelay(float fDelay) {

    TSTRING tsDelay;
    
    sprintf(tsDelay, _T("%s%.02f\r"), CAMERACOMM_IE, fDelay);

    return Communicator::Write(tsDelay);
  }

};

#endif // !_CAMERACOMM_H