#ifndef _FURRAMSCOMM_H
#define _FURRAMSCOMM_H

#include "amscomm.h"

/*
 * Constants
 */
#define FAMSC_CAMERAAXIS  'C'       /* Name of camera axis */
#define FAMSC_CWAXIS      'W'       /* Name of color wheel axis */
#define FAMSC_ISMOVING    "^"       /* Is moving query character */
#define FAMSC_GOHOME      "F1"      /* Axis to home character */
#define FAMSC_AXISMOVING  '1'       /* Axis is moving response character */
#define FAMSC_ZEROSTEPS   "O"       /* Zero step counter */
#define FAMSC_GETSTEPS    "Z"       /* Query step accumulator */
#define FAMSC_BADSTEP     9000000   /* Invalid step count (_GetAxisAccumulator) */

/*
 * Camera initialization strings
 */
#define FAMSC_CI_POWER    "Y5 80"   /* Power level */
#define FAMSC_CI_STEPW    "D3"      /* Step width */
#define FAMSC_CI_MAXVELO  "V3000"   /* Maximum velocity */
#define FAMSC_CI_RAMPSPD  "K20 20"  /* Ramp speed */
#define FAMSC_CI_FIXDRES  "H0"      /* Fixed resolution */
#define FAMSC_CI_SETTLE   "E50"     /* Settling time */
#define FAMSC_CI_INIVELO  "I400"    /* Initial velocity */

/*
 * Color wheel initialization strings
 */
#define FAMSC_WI_POWER    "Y0 10"   /* Power level */
#define FAMSC_WI_STEPW    "D3"      /* Step width */
#define FAMSC_WI_MAXVELO  "V2000"   /* Maximum velocity */
#define FAMSC_WI_RAMPSPD  "K50 50"  /* Ramp speed */
#define FAMSC_WI_FIXDRES  "H0"      /* Fixed resolution */
#define FAMSC_WI_SETTLE   "E50"     /* Settling time */
#define FAMSC_WI_INIVELO  "I400"    /* Initial velocity */

/*
 * Miscellaneous camera constants
 */
#define FAMSC_C_WINDFWD   "+"       /* Wind + value */
#define FAMSC_C_WINDREV   "-"       /* Wind reverse + value */
#define FAMSC_C_MECHSTEP  800       /* Steps needed to toggle mechanical shutter */

/*
 * Miscellaneous color wheel constants
 */
#define FAMSC_W_ADV       "R"       /* Advance relative to origin + value */
#define FAMSC_W_RED       "-1600"   /* Red filter location relative to origin */
#define FAMSC_W_GRN       "-2450"   /* Green filter location relative to origin */
#define FAMSC_W_BLU       "-3200"   /* Blue filter location relative to origin */
#define FAMSC_W_CLR       "0"       /* Clear filter location relative to origin */

/*
 * Type defines
 */

/*
 * Callback function typedef
 *
 * Used by FURRAMSComm to report
 * communication problems when
 * talking to the AMS driver.
 */
typedef void (*AMSCOMMCALLBACK)(CTSTRING &, DWORD_PTR);

/*
 * CWFilter Enumeration
 *
 * Contains possible stops
 * for the color wheel mechanism.
 */
enum CWFilter {

  RED = 0,
  GREEN,
  BLUE,
  CLEAR

};

/*
 * AxisQuery Enumeration
 *
 * Contains possible values
 * for querying the state of
 * an axis.
 */
enum AxisQuery {

  AXISMOVING = 0,
  AXISIDLE,
  AXISERROR

};

/*
 * FURRAMSComm Class
 *
 * Derives from AMSComm
 * to provide high-level
 * abstraction for AMS driver
 * controlling of FURR's camera
 * and color wheel motors.
 */
class FURRAMSComm : public AMSComm {

public:
  FURRAMSComm() {

    m_pFn         = NULL;
    m_dwUserData  = 0UL;

  }
  ~FURRAMSComm() {

  }

 /*
  * Sets the error callback function
  * pointer for this object that
  * will be called in the event of
  * a communications error.
  */
  void SetCallback(AMSCOMMCALLBACK pFn, DWORD_PTR dwUserData) {
    
    m_pFn         = pFn;
    m_dwUserData  = dwUserData;
  
  }

 /*
  * Sends initialization parameters
  * to the AMS driver for the camera
  * axis.
  */
  bool InitializeCameraAxis(void) {

    bool r                = false;
    ANSISTRING strResult  = _T("");

    if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_POWER, strResult)) {

      if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_STEPW, strResult)) {

        if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_MAXVELO, strResult)) {

          if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_RAMPSPD, strResult)) {

            if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_FIXDRES, strResult)) {

              if (_Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_SETTLE, strResult)) {

                return _Dispatch(FAMSC_CAMERAAXIS, FAMSC_CI_INIVELO, strResult);

              }

            }

          }

        }

      }

    }

    return r;
  }

 /*
  * Sends initialization parameters
  * to the AMS driver for the color wheel
  * axis.
  */
  bool InitializeColorWheelAxis(void) {

    bool r                = false;
    ANSISTRING strResult  = _T("");

    if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_POWER, strResult)) {

      if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_STEPW, strResult)) {

        if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_MAXVELO, strResult)) {

          if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_RAMPSPD, strResult)) {

            if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_FIXDRES, strResult)) {

              if (_Dispatch(FAMSC_CWAXIS, FAMSC_WI_SETTLE, strResult)) {

                return _Dispatch(FAMSC_CWAXIS, FAMSC_WI_INIVELO, strResult);

              }

            }

          }

        }

      }

    }

    return r;
  }

 /*
  * Determines the state of an axis.
  * 
  * Can return the following values:
  *
  * AXISMOVING if the axis is moving
  * AXISIDLE   if the axis is not moving
  * AXISERROR  if there was a communication error
  */
  AxisQuery IsAxisMoving(char cAxis) {

    ANSISTRING strQuery   = FAMSC_ISMOVING;
    ANSISTRING strResult  = _T("");

    if (!_Dispatch(cAxis, strQuery, strResult)) {

      return AXISERROR;

    } else {

     /*
      * If the response contains a '1',
      * then the axis is moving; otherwise
      * it should contain a '0', and the axis
      * is idle.
      */
      if (ANSISTRING::npos != strResult.find(FAMSC_AXISMOVING)) {

        return AXISMOVING;

      } else {

        return AXISIDLE;

      }

    }

  }

 /*
  * Winds camera axis to home sensor
  */
  bool Camera_WindHome(void) { return _GoHome(FAMSC_CAMERAAXIS); }

 /*
  * Toggles the state of the mechanical
  * camera shutter (800 steps = 180 degrees)
  */
  bool Camera_ToggleMechShutter(void) { return Camera_Wind(FAMSC_C_MECHSTEP); }

 /*
  * Winds the camera axis in either direction,
  * at a set amount of steps
  */
  bool Camera_Wind(int iSteps, bool bForward = true) {

    ANSISTRING strWind;
    ANSISTRING strResult;

    sprintf(strWind, _T("%s%d"), bForward ? FAMSC_C_WINDFWD : FAMSC_C_WINDREV, iSteps);

    return _Dispatch(FAMSC_CAMERAAXIS, strWind, strResult);
  }

 /*
  * Queries camera axis for current position
  */
  int Camera_GetStepCount(void) {

    return _GetAxisAccumulator(FAMSC_CAMERAAXIS);
  }

 /*
  * Resets camera axis step accumulator
  */
  bool Camera_ResetAccumulator(void) {
    
    return _ResetStepAccumulator(FAMSC_CAMERAAXIS);
  }

 /*
  * Winds color wheel axis to home sensor
  */
  bool ColorWheel_WindHome(void) { return _GoHome(FAMSC_CWAXIS); }

 /*
  * Winds color wheel to RED filter
  *
  * Note: Assumes that axis has been homed
  * to CLEAR filter previous to call.
  */
  bool ColorWheel_WindRed(void) { return _ColorWheel_ToFilter(RED); }

 /*
  * Winds color wheel to GREEN filter
  *
  * Note: Assumes that axis has been homed
  * to CLEAR filter previous to call.
  */
  bool ColorWheel_WindGreen(void) { return _ColorWheel_ToFilter(GREEN); }

 /*
  * Winds color wheel to BLUE filter
  *
  * Note: Assumes that axis has been homed
  * to CLEAR filter previous to call.
  */
  bool ColorWheel_WindBlue(void) { return _ColorWheel_ToFilter(BLUE); }

 /*
  * Winds color wheel to CLEAR filter
  *
  * Note: Assumes that axis has been homed
  * to CLEAR filter previous to call.
  */
  bool ColorWheel_WindClear(void) { return _ColorWheel_ToFilter(CLEAR); }

 /*
  * Queries camera axis for current position
  */
  int ColorWheel_GetStepCount(void) {

    return _GetAxisAccumulator(FAMSC_CWAXIS);
  }

 /*
  * Resets camera axis step accumulator
  */
  bool ColorWheel_ResetAccumulator(void) {
    
    return _ResetStepAccumulator(FAMSC_CWAXIS);
  }


 /*
  * Private methods
  */


private:

 /*
  * Sends an axis to its home sensor
  */
  bool _GoHome(char cAxis) {

    ANSISTRING strResult;

    return _Dispatch(cAxis, FAMSC_GOHOME, strResult);
  }

 /*
  * Returns the value of the step accumulator
  * for an axis.
  *
  * Note: Can return FAMSC_BADSTEP if there
  * is a communications failure.
  */
  int _GetAxisAccumulator(char cAxis) {

    int r = FAMSC_BADSTEP;

    ANSISTRING strQuery   = FAMSC_GETSTEPS;
    ANSISTRING strResult  = _T("");

    if (_Dispatch(cAxis, strQuery, strResult)) {

     /*
      * Got response from driver; strip any non
      * numeric characters, and return value.
      */
      ANSISTRING strStrip = _T(" \n");

      StrStripChars(strResult, strStrip.c_str());

     /*
      * Convert string result into integer
      */
      r = atoi(strResult.c_str());

    }

    return r;
  }

 /*
  * Resets the step accumulator for an axis
  */
  bool _ResetStepAccumulator(char cAxis) {

    ANSISTRING strResult;

    return _Dispatch(cAxis, FAMSC_ZEROSTEPS, strResult);
  }

 /*
  * Dispatches a command to the driver, and if an error
  * occurs, uses the supplied callback to report said error.
  */
  bool _Dispatch(char cAxis, CANSISTRING &strWrite, ANSISTRING &strResult) {

    bool r = false;

   /*
    * Send command string to driver
    */
    switch (Write(cAxis, strWrite, strResult)) {

      case AMS_OK:
      {
       /*
        * No errors. Success
        */
        r = true;
        break;
      }
      case AMS_TXTIMEOUT:
      {
       /*
        * Transmission error,
        * the driver is probably offline.
        */
        if (NULL != m_pFn) {

          TSTRING tsMsg = _T("");

          sprintf(tsMsg, _T("FURRAMSComm: Unable to transmit data '%s' to axis '%c'!"),
                  strWrite.c_str(),
                  cAxis);

          m_pFn(tsMsg, m_dwUserData);

        }
        break;
      }
      case AMS_RXTIMEOUT:
      {
       /*
        * Recieve error,
        * the driver is probably offline.
        */
        if (NULL != m_pFn) {

          TSTRING tsMsg = _T("");

          sprintf(tsMsg, _T("FURRAMSComm: Timed out waiting for response; data '%s' on axis '%c'!"),
                  strWrite.c_str(),
                  cAxis);

          m_pFn(tsMsg, m_dwUserData);

        }
        break;
      }

    }

    return r;
  }

 /*
  * Moves the color wheel axis to a
  * specified filter location
  *
  * Note: Assumes color wheel has
  * been "homed"
  */
  bool _ColorWheel_ToFilter(CWFilter Filter) {

    ANSISTRING strWind = FAMSC_W_ADV;
    ANSISTRING strResult;

    switch (Filter) {

      case RED:
      {
        strWind += FAMSC_W_RED;
      }
      break;
      case GREEN:
      {
        strWind += FAMSC_W_GRN;
      }
      break;
      case BLUE:
      {
        strWind += FAMSC_W_BLU;
      }
      break;
      default:
      case CLEAR:
      {
        strWind += FAMSC_W_CLR;
      }
      break;

    }

    return _Dispatch(FAMSC_CWAXIS, strWind, strResult);
  }

 /*
  * Private members
  */
private:
  AMSCOMMCALLBACK m_pFn;
  DWORD_PTR       m_dwUserData;

};

#endif // !_FURRAMSCOMM_H
