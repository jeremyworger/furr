#ifndef _COMMUNICATOR_H
#define _COMMUNICATOR_H

#include "stdafx.h"

/*
 * Communicator class
 *
 * Provides a wrapped interface
 * for serial port/file/console communcation.
 */
class Communicator {

public:

  Communicator() {
  
    _Reset();

  }

 virtual ~Communicator() { Close(); }

 virtual bool Open(CTSTRING &tsOpen) {

    bool r = false;

    m_h = CreateFile(tsOpen.c_str(),
                     GENERIC_READ | GENERIC_WRITE,
                     0UL,
                     NULL,
                     OPEN_EXISTING,
                     0UL,
                     NULL);

    if (INVALID_HANDLE_VALUE != m_h) {

      m_bOpen = true;
      r       = true;

    } else {

      _Reset();

    }

    return r;
  }

  virtual bool Close(void) {

    if (m_bOpen) {

      if (TRUE == CloseHandle(m_h)) {

        _Reset();
        return true;
        
      } else {

        _Reset();
        return false;

      }
    
    } else {

      return false;

    }

  }

  virtual bool Read(BYTE *pBuffer, DWORD_PTR dwSizeBytes) {

    bool r = false;

    if ((NULL != pBuffer) && (0UL < dwSizeBytes)) {

      DWORD dwBytesRead = 0UL;

      if (TRUE == ReadFile(m_h, pBuffer, dwSizeBytes, &dwBytesRead, NULL) 
          && (dwBytesRead == dwSizeBytes))
      {

        r = true;

      }

    }

    return r;
  }

  virtual bool Read(TSTRING &tsRead, DWORD_PTR dwSizeBytes) {

    bool r = false;

    if (0 < dwSizeBytes) {

      BYTE *pBuf = new BYTE[dwSizeBytes];

      if (NULL != pBuf) {

        ZeroMemory(pBuf, dwSizeBytes);

        if (Read(pBuf, dwSizeBytes)) {

          tsRead  = reinterpret_cast<TCHAR *>(pBuf);
          r       = true;

        }

        delete pBuf;
      }

    }

    return r;
  }

  virtual bool Write(const BYTE *pWrite, DWORD dwBytes) {

    bool r = false;

    if ((NULL != pWrite) && (0 != *pWrite) && (0 < dwBytes)) {

      DWORD dwBytesWritten = 0UL;

      if (TRUE == WriteFile(m_h, pWrite, dwBytes, &dwBytesWritten, NULL)
          && (dwBytesWritten == dwBytes))
      {

        r = true;

      }

    }

    return r;
  }

  virtual bool Write(CTSTRING &tsWrite) {

    return Write(reinterpret_cast<const BYTE *>(tsWrite.c_str()), 
                 tsWrite.size() * sizeof(TCHAR));
  }

private:

  void _Reset(void) {

    m_h     = NULL;
    m_bOpen = false;

  }

protected:
  HANDLE m_h;
  bool   m_bOpen;

};

#endif // !_COMMUNICATOR_H
