#ifndef _PARALLELCOMM_H
#define _PARALLELCOMM_H

#include "../inpout32/include/inpout32.h"

/*
 * Constants
 */
#define PC_OUTPORT    0x378
#define PC_INPORT     0x379

/*
 * ParallelComm Class
 *
 * Provides a C++ interface
 * to the parallel port pins.
 */
class ParallelComm {

public:
  ParallelComm() { }
  virtual ~ParallelComm() { }

  virtual short Read(void) { return Inp32(PC_INPORT); }
  virtual void Write(short sData) { return Out32(PC_OUTPORT, sData); }

};

#endif // !_PARALLELCOMM_H
