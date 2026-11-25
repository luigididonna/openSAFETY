#include <stdio.h>
#include <string.h>
#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SSCapi.h"
#include "SHNF.h"
#include "SFS.h"
#include "loopback_shnf.h"

int main(void)
{
    const UINT8 producer = 0u;
    const UINT8 consumer = 1u;
    const UINT8 process_value = 0x5Au;
    UINT8 free_frames = 1u;
    UINT8 wire_frame[600];
    UINT8 received_frame[600];
    UINT16 wire_length = 0u;
    UINT16 received_length = 0u;
    UINT8 *tx;
    const UINT8 *payload;
    EPLS_t_FRM_HDR tx_header;
    EPLS_t_FRM_HDR rx_header;

    if (!SSC_InitAll()) {
        fputs("SSC_InitAll failed; see the openSAFETY stack error above.\n", stderr);
        return 1;
    }

    memset(&tx_header, 0, sizeof(tx_header));
    tx_header.w_adr = 1u;
    tx_header.b_id = 0x30u; /* SPDO data only frame (no time request/response). */
    tx_header.w_sdn = 1u;
    tx_header.b_le = 1u;
    tx_header.w_ct = 1u;
    tx_header.w_tadr = 4u;

    tx = SFS_GetMemBlock(producer, SHNF_k_SPDO, 0u, tx_header.b_le);
    if (tx == NULL) {
        fputs("SPDO producer could not allocate a frame.\n", stderr);
        return 2;
    }
    tx[SFS_k_FRM_DATA_OFFSET] = process_value;
    if (!SFS_FrmSerialize(producer, &tx_header, tx) ||
        !loopback_take_tx(producer, wire_frame, &wire_length) ||
        !loopback_deliver(consumer, wire_frame, wire_length) ||
        !loopback_take_rx(consumer, received_frame, &received_length)) {
        fputs("SPDO serialization or loopback delivery failed.\n", stderr);
        return 3;
    }

    memset(&rx_header, 0, sizeof(rx_header));
    payload = SFS_FrmDeSerialize(consumer, received_frame, received_length,
                                 &rx_header);
    if (payload == NULL || rx_header.b_id != 0x30u ||
        rx_header.w_adr != tx_header.w_adr ||
        rx_header.w_tadr != tx_header.w_tadr || payload[0] != process_value) {
        fputs("Consumer rejected the SPDO frame or mapped payload.\n", stderr);
        return 4;
    }

    printf("openSAFETY SPDO frame exchange succeeded\n");
    printf("  producer SADR: %u -> consumer SADR: %u\n",
           rx_header.w_adr, rx_header.w_tadr);
    printf("  frame length: %u bytes; verified process-data byte: 0x%02X\n",
           received_length, payload[0]);
    printf("  upstream SFS serializer and decoder verified both frame subframes and CRC\n");
    return 0;
}
