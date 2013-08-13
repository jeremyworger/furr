#ifndef _FURRSESSIONCOMM_H
#define _FURRSESSIONCOMM_H

#include "furramscomm.h"
#include "furrcappingshutter.h"

/*
 * FURRSessionComm Class
 *
 * Derives from both FURRAMSComm
 * and FURRCappingShutter to provide
 * a wrapped interface to control
 * camera motors, color wheel devices,
 * and any other peripheral devices
 * used by the FURRSession object.
 */
class FURRSessionComm : public FURRAMSComm, 
                        public FURRCappingShutter
{

public:
  FURRSessionComm() { }
  ~FURRSessionComm() { }

};

#endif // !_FURRSESSIONCOMM_H
