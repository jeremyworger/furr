#ifndef _FURRCAPPINGSHUTTER_H
#define _FURRCAPPINGSHUTTER_H

#include "parallelcomm.h"

/*
 * Constants 
 */
#define CS_UNCAP  0x1U
#define CS_CAP    0x0U

/*
 * FURRCappingShutter Class
 *
 * Derives from ParallelComm
 * to provide a C++ interface
 * to our electronic capping
 * shutter mechanism via parallel
 * port.
 */
class FURRCappingShutter : public ParallelComm {

public:
  FURRCappingShutter() { }
  ~FURRCappingShutter() { }

  void Uncap(void) { return Write(CS_UNCAP); }
  void Cap(void) { return Write(CS_CAP); }

};

#endif // !_FURRCAPPINGSHUTTER_H
