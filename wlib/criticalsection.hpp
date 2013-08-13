#ifndef _CRITICALSECTION_H
#define _CRITICALSECTION_H

#include "stdafx.h"

/*
 * CriticalSection Class
 *
 * Simple wrapper for Win32
 * critical sections.
 */
class CriticalSection {

public:
  CriticalSection() {

    Reset();
    
  }
  virtual ~CriticalSection() {

    Reset();
  
  }

 /*
  * Initializes the object
  */
  bool Create(DWORD dwSpinCount = 0UL) {

#if (_WIN32_WINNT >= 0x0403)

    return (0 != InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount));

#else
 
    __try {

      InitializeCriticalSection(&m_cs);
      return true;

    } __except(EXCEPTION_EXECUTE_HANDLER) {

      return false;

    }

#endif

  }

 /*
  * Tries to enter the critical section
  * and will block until any other threads
  * have relinquished control.
  *
  * Note: Can return false if an exception
  * was thrown by EnterCriticalSection().
  */
  bool Enter(void) {

    __try {

      EnterCriticalSection(&m_cs);
      return true;

    } __except(EXCEPTION_EXECUTE_HANDLER) {

      return false;

    }

  }

#if (_WIN32_WINNT >= 0x0400)

 /*
  * Checks to see if the critical section
  * can be entered, but will not block the
  * calling thread. If the call is successful,
  * the calling thread owns the critical section.
  */
  bool TryEnter(void) {

    return (0 != TryEnterCriticalSection(&m_cs));
  }

#endif

 /*
  * Relinquishes control of the critical
  * section to any other threads.
  */
  void Leave(void) { LeaveCriticalSection(&m_cs); }

#if (_WIN32_WINNT >= 0x0403)

 /*
  * Sets the spin count for multi-processor systems;
  * returns the previous spin count.
  */
  DWORD SetSpinCount(DWORD dwSpinCount) {
    
    return SetCriticalSectionSpinCount(dwSpinCount);
  }

#endif

 /*
  * Resets the object
  * to a virgin state.
  */
  void Reset(void) {

    DeleteCriticalSection(&m_cs);
    RtlZeroMemory(&m_cs, sizeof(m_cs));

  }

protected:
  CRITICAL_SECTION m_cs;

};

#endif // !_CRITICALSECTION_H
