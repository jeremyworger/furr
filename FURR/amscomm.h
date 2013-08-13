#ifndef _AMSCOMM_H
#define _AMSCOMM_H

#include "serialcomm.h"

/*
 * Driver constants
 */
#define _AMSC_PORT    _T("COM1")  /* Serial port name */
#define _AMSC_BUSY    '$'         /* Busy character */
#define _AMSC_TERM    '\n'        /* Terminator character */

/*
 * CommCode enumeration
 *
 * Contains possible result codes
 * for communication with the driver.
 */
enum CommCode {

  RX_OK = 0,
  RX_TIMEOUT,
  RX_DIFF,
  TX_TIMEOUT

};

/*
 * AMSCode enumeration
 *
 * Contains possible results
 * for Write()
 */
enum AMSCode {

  AMS_OK = 0,
  AMS_TXTIMEOUT,
  AMS_RXTIMEOUT,
  AMS_ERROR

};

/*
 * AMSComm Class
 *
 * Provides an interface for AMS driver
 * communication.
 */
class AMSComm : public SerialComm {

public:
  AMSComm() { }
  ~AMSComm() { }

public:

  /* Opens communications line with driver */
  bool Open(void) { return SerialComm::Open(_AMSC_PORT); }
  bool Open(CTSTRING &tsPort) { return SerialComm::Open(tsPort); }

 /*
  * Writes a command string to the driver.
  */
  AMSCode Write(char cAxis, CANSISTRING &strCmd, ANSISTRING &strResult, int nRxRetry = 5) {

   /* 
    * Verify that we've been given a valid axis name,
    * and command string.
    */
    if ((0 != cAxis) && (0 < strCmd.size())) {

     /*
      * Send axis name, and wait for response.
      */
      int nRetry  = 0;

lblStart:

      char cRx    = 0;

      switch (_TxRxByte(cAxis, &cRx)) {

        case TX_TIMEOUT: 
        {
         /* 
          * We have a problem. Transmit should
          * never time-out; error!
          */
          return AMS_TXTIMEOUT;
        }
        case RX_TIMEOUT:
        {
         /*
          * Response not recieved in nominal time.
          * Issue LF and try again if nRxRetry has not
          * been reached.
          */
          if (nRetry <= nRxRetry) {

            if (Communicator::Write(&m_lf, 1UL)) {

              nRetry++;
              goto lblStart;

            } else {

              return AMS_TXTIMEOUT;

            }

          } else {

           /*
            * We've retried enough times;
            * giving up.
            */
            return AMS_RXTIMEOUT;

          }

          break;
        }
        case RX_DIFF:
        {
         /*
          * The driver has echoed a character that is
          * not the axis name. If the driver is busy,
          * try again. Otherwise failure.
          */
          if (cRx == _AMSC_BUSY) {

           /*
            * Just busy, issue LF and try again if
            * nRxRetry has not been reached.
            */
            if (nRetry <= nRxRetry) {

              if (Communicator::Write(&m_lf, 1UL)) {

                nRetry++;
                goto lblStart;

              } else {

               /*
                * Unable to transmit; error!
                */
                return AMS_TXTIMEOUT;

              }

            } else {

             /*
              * We've retried enough times;
              * giving up.
              */
              return AMS_RXTIMEOUT;
            
            }

          } else {

            /*
             * Driver crashed or communications line
             * inoperable.
             */
             return AMS_ERROR;
          }

          break;
        }
        case RX_OK:
        {
         /*
          * Character transmitted OK;
          * begin issuing command-string.
          */
          CANSISTRINGIT it = strCmd.begin();

          strResult.clear();

          for (; it != strCmd.end(); it++) {

            switch (_TxRxByte((*it), &cRx)) {

              case TX_TIMEOUT:
              {
               /*
                * Unable to transmit byte to driver;
                * failure!
                */
                return AMS_TXTIMEOUT;
              }
              case RX_TIMEOUT:
              {
               /*
                * Did not recieve response from
                * driver in nominal time; issue
                * LF and start over, if nRxRetry
                * has not been reached.
                */
                if (nRetry <= nRxRetry) {

                  if (Communicator::Write(&m_lf, 1UL)) {

                    nRetry++;
                    goto lblStart;

                  } else {

                   /*
                    * Unable to transmit; error!
                    */
                    return AMS_TXTIMEOUT;

                  }

                } else {

                 /*
                  * We've retried enough,
                  * giving up.
                  */
                  return AMS_RXTIMEOUT;

                }

                break;
              }
              case RX_DIFF:
              {
               /*
                * Echoed byte was different than transmitted byte.
                * This is a horrible error
                */
                return AMS_ERROR;
              }
              case RX_OK:
              {
               /*
                * This is what we want to see every time.
                */
                break;
              }

            }

          }

         /*
          * Last character in command string has
          * been issued; issue LF and then recieve
          * n bytes from driver until LF is recieved.
          */
          if (!Communicator::Write(&m_lf, 1UL)) {

           /*
            * Unable to transmit; error!
            */
            return AMS_TXTIMEOUT;

          } else {

lblRecvResult:
            cRx = 0;

            if (Communicator::Read(reinterpret_cast<BYTE *>(&cRx), 1UL)) {

             /*
              * Read byte. If LF, we're done!
              * Otherwise this is a result byte.
              */
              if (m_lf == cRx) {

                return AMS_OK;

              } else {

                strResult += cRx;
                goto lblRecvResult;

              }

            } else {

             /*
              * Driver did not respond in nominal time;
              * issue LF and start over, if nRxRetry has
              * not been reached.
              */
              if (nRetry <= nRxRetry) {

                if (Communicator::Write(&m_lf, 1UL)) {

                  nRetry++;
                  goto lblStart;

                } else {

                 /*
                  * Unable to transmit; error!
                  */
                  return AMS_TXTIMEOUT;
                }

              } else {

               /*
                * We've retried enough,
                * giving up.
                */
                return AMS_RXTIMEOUT;

              }

            }

          }

          break;
        }

      }

    }
  
    return AMS_ERROR;
  }

/*
 * Private methods
 */
private:

  CommCode _TxRxByte(char cTx, char *pcRx) {

    CommCode r = TX_TIMEOUT;

    if (NULL != pcRx) {

      /* Attempt to write byte on serial port */
      if (Communicator::Write(reinterpret_cast<const BYTE *>(&cTx), 1UL)) {

        /* Read echoed byte from driver on serial port */
        if (Communicator::Read(reinterpret_cast<BYTE *>(pcRx), 1UL)) {

         /* 
          * Read byte successfully, check against
          * transmitted byte. If identical, consider
          * this byte successfully tx'd/rx'd; return RX_OK.
          * If byte differs, return RX_DIFF.
          */
          r = (*pcRx == cTx) ? RX_OK : RX_DIFF;

        } else {

          /* Failed to read (timeout); set result to null byte */
          *pcRx = 0;
          r     = RX_TIMEOUT;

        }

      }

    }

    return r;
  }

/*
 * Private members
 */
private:
  
  static const BYTE m_lf = '\n';

};

#endif // !_AMSCOMM_H
