#include <stdio.h>
#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SSCapi.h"
#include "SODapi.h"
#include "SPDOapi.h"
#include "SPDO.h"
#include "loopback_shnf.h"

void demo_sod_producer_set_value(UINT8 value);
UINT8 demo_sod_consumer_get_value(void);

static BOOLEAN activate_rx(UINT8 instance)
{
    BOOLEAN busy;
    do {
        busy = FALSE;
        if (!SPDO_ActivateRxSpdoMapping(instance, &busy)) return FALSE;
    } while (busy);
    return TRUE;
}

static BOOLEAN activate_tx(UINT8 instance)
{
    BOOLEAN busy;
    do {
        busy = FALSE;
        if (!SPDO_ActivateTxSpdoMapping(instance, &busy)) return FALSE;
    } while (busy);
    return TRUE;
}

static BOOLEAN exchange_one_tick(UINT8 source, UINT8 destination, UINT32 ct)
{
    UINT8 free_frames = 1u;
    UINT8 frame[600];
    UINT16 frame_length = 0u;

    SPDO_BuildTxSpdo(source, ct, &free_frames);
    if (free_frames != 0u) return TRUE;
    if (!loopback_take_tx(source, frame, &frame_length) ||
        !loopback_deliver(destination, frame, frame_length) ||
        !loopback_take_rx(destination, frame, &frame_length)) return FALSE;

    SPDO_ProcessRxSpdo(destination, ct, frame, frame_length);
    SPDO_CheckRxTimeout(destination, ct);
    return TRUE;
}

int main(void)
{
    const UINT8 producer = 0u;
    const UINT8 consumer = 1u;
    const UINT8 value_sent = 0x5Au;
    UINT16 ticks = 0u;
    UINT16 delta_ct = 0u;
    UINT16 propagation_delay = 0u;
    UINT32 data_age = 0u;
    BOOLEAN rx_valid;

    if (!SSC_InitAll()) {
        fputs("SSC_InitAll failed; see the openSAFETY stack error above.\n", stderr);
        return 1;
    }

    SPDO_ActivateStart(producer);
    SPDO_ActivateStart(consumer);
    if (!activate_tx(producer) || !activate_tx(consumer) ||
        !activate_rx(producer) || !activate_rx(consumer)) {
        fputs("The stack rejected an SPDO communication or mapping parameter.\n", stderr);
        return 2;
    }

    SPDO_SetToOp(producer, 0u);
    SPDO_SetToOp(consumer, 0u);
    demo_sod_producer_set_value(value_sent);
    SPDO_TxDataChanged(producer, 1u);
    for (ticks = 1u; ticks <= 100u; ++ticks) {
        UINT32 ct = (UINT32)ticks * 100u;
        if (!exchange_one_tick(producer, consumer, ct) ||
            !exchange_one_tick(consumer, producer, ct + 1u)) {
            fputs("Loopback transport failed while exchanging SPDO frames.\n", stderr);
            return 3;
        }
        if (demo_sod_consumer_get_value() == value_sent) break;
    }
    rx_valid = SPDO_GetRxSpdoStatus(consumer, 0u, (UINT32)ticks * 100u,
                                    &delta_ct, &data_age, &propagation_delay);
    if (demo_sod_consumer_get_value() != value_sent || !rx_valid) {
        fprintf(stderr, "Mapped value mismatch: sent 0x%02X, received 0x%02X.\n",
                value_sent, demo_sod_consumer_get_value());
        return 4;
    }

    printf("openSAFETY SPDO producer/consumer exchange succeeded\n");
    printf("  producer SADR 1 -> consumer instance %u (Rx SPDO from SADR 1)\n",
           consumer);
    printf("  exchange completed in %u cyclic opportunities\n", ticks);
    printf("  SOD 0x6000/1 received 0x%02X; Rx connection status is valid\n",
           demo_sod_consumer_get_value());
    printf("  stack APIs exercised: SPDO_BuildTxSpdo, SPDO_ProcessRxSpdo, SPDO_CheckRxTimeout\n");
    return 0;
}
