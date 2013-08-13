#ifndef _SERIALCOMM_H
#define _SERIALCOMM_H

#include "communicator.h"

/*
 * Constants
 */
#define _COMM_INBUFFER  256UL
#define _COMM_OUTBUFFER 256UL

/*
 * SerialComm Class
 *
 * Derives from Communicator
 * to provide serial port I/O
 */
class SerialComm : public Communicator {

public:
  SerialComm() {}
  virtual ~SerialComm() {}

  virtual bool Open(CTSTRING &tsOpen) {

    bool r = false;

    if (Communicator::Open(tsOpen)) {

      if (0 != SetupComm(m_h, _COMM_INBUFFER, _COMM_OUTBUFFER)) {
            
        DCB dcb = {0};

        if (TRUE == GetCommState(m_h, &dcb)) {

          dcb.BaudRate = CBR_9600;
          dcb.Parity   = NOPARITY;
          dcb.ByteSize = 8;
          dcb.StopBits = ONESTOPBIT;
          dcb.fBinary  = TRUE;
          dcb.fParity  = FALSE;

          if (TRUE == SetCommState(m_h, &dcb)) {

            COMMTIMEOUTS cto = {0};

            if (TRUE == GetCommTimeouts(m_h, &cto)) {

              cto.ReadIntervalTimeout         = 0UL;
              cto.ReadTotalTimeoutConstant    = 1000UL;
              cto.ReadTotalTimeoutMultiplier  = 0UL;
              cto.WriteTotalTimeoutConstant   = 1000UL;
              cto.WriteTotalTimeoutMultiplier = 0UL;

              if (TRUE == SetCommTimeouts(m_h, &cto)) {

                r = true;

              }

            }

          }

        }

      }

    }

    return r;
  }

};

#endif // !_SERIALCOMM_H
