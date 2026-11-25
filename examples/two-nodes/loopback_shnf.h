#ifndef LOOPBACK_SHNF_H
#define LOOPBACK_SHNF_H

#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SHNF.h"

BOOLEAN loopback_take_tx(UINT8 source_instance, UINT8 *frame, UINT16 *length);
BOOLEAN loopback_deliver(UINT8 destination_instance, const UINT8 *frame,
                         UINT16 length);
BOOLEAN loopback_take_rx(UINT8 destination_instance, UINT8 *frame,
                         UINT16 *length);

#endif
