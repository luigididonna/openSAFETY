#include <stdio.h>
#include <string.h>

#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SHNF.h"
#include "SFS.h"
#include "SFSint.h"
#include "SNMT.h"
#include "SNMTS.h"
#include "loopback_shnf.h"

int main(void)
{
    const UINT8 sender_node = 1u;
    const UINT8 receiver_node = 2u;
    UINT8 wire_frame[600];
    UINT8 received_frame[600];
    UINT16 wire_length = 0u;
    UINT16 received_length = 0u;
    UINT8 *tx;
    const UINT8 *payload;
    EPLS_t_FRM_HDR tx_header;
    EPLS_t_FRM_HDR rx_header;

    memset(&tx_header, 0, sizeof(tx_header));
    tx_header.w_adr = sender_node;
    tx_header.b_id = (UINT8)(SNMT_k_FRM_TYPE | SNMT_k_EXT_SER_REQ);
    tx_header.w_sdn = 1u;
    tx_header.b_le = 1u;
    tx_header.w_ct = 7u;
    tx_header.w_tadr = 1u;

    tx = SFS_GetMemBlock(sender_node, SHNF_k_SNMT, 0u, tx_header.b_le);
    if (tx == NULL)
    {
        fputs("Node A could not allocate an openSAFETY frame.\n", stderr);
        return 1;
    }
    tx[SFS_k_FRM_DATA_OFFSET] = SNMT_k_SN_SET_TO_PREOP;
    if (!SFS_FrmSerialize(sender_node, &tx_header, tx))
    {
        fputs("Node A failed to serialize the openSAFETY frame.\n", stderr);
        return 2;
    }
    if (!loopback_take_tx(sender_node, wire_frame, &wire_length) ||
        !loopback_deliver(receiver_node, wire_frame, wire_length) ||
        !loopback_take_rx(receiver_node, received_frame, &received_length))
    {
        fputs("The loopback transport failed to deliver the frame.\n", stderr);
        return 3;
    }

    memset(&rx_header, 0, sizeof(rx_header));
    payload = SFS_FrmDeSerialize(receiver_node, received_frame, received_length,
                                 &rx_header);
    if (payload == NULL)
    {
        fputs("Node B rejected the openSAFETY frame.\n", stderr);
        return 4;
    }

    printf("SCM A (SADR %u) -> Safety Node B (instance %u): valid openSAFETY SNMT request\n",
           tx_header.w_adr, receiver_node);
    printf("  frame length: %u bytes\n", received_length);
    printf("  frame ID: 0x%02X, SDN: %u, CT: %u\n",
           rx_header.b_id, rx_header.w_sdn, rx_header.w_ct);
    printf("  SNMT command: %u (set to PRE-OPERATIONAL)\n", payload[0]);

    /* The SN answers with the SNMT extended service response (frame ID 45). */
    memset(&tx_header, 0, sizeof(tx_header));
    tx_header.w_adr = receiver_node;
    tx_header.b_id = (UINT8)(SNMT_k_FRM_TYPE + SNMT_k_EXT_SER_REQ + 1u);
    tx_header.w_sdn = 1u;
    tx_header.b_le = 1u;
    tx_header.w_ct = 8u;
    tx_header.w_tadr = 1u;
    tx = SFS_GetMemBlock(receiver_node, SHNF_k_SNMT, 0u, tx_header.b_le);
    if (tx == NULL)
    {
        fputs("Node B could not allocate its SNMT response frame.\n", stderr);
        return 5;
    }
    tx[SFS_k_FRM_DATA_OFFSET] = SNMTS_k_SN_STS_PREOP;
    if (!SFS_FrmSerialize(receiver_node, &tx_header, tx) ||
        !loopback_take_tx(receiver_node, wire_frame, &wire_length) ||
        !loopback_deliver(sender_node, wire_frame, wire_length) ||
        !loopback_take_rx(sender_node, received_frame, &received_length))
    {
        fputs("Node B failed to send its SNMT response.\n", stderr);
        return 6;
    }
    memset(&rx_header, 0, sizeof(rx_header));
    payload = SFS_FrmDeSerialize(sender_node, received_frame, received_length,
                                 &rx_header);
    if (payload == NULL ||
        rx_header.b_id != (UINT8)(SNMT_k_FRM_TYPE + SNMT_k_EXT_SER_REQ + 1u) ||
        payload[0] != SNMTS_k_SN_STS_PREOP)
    {
        fputs("Node A rejected Node B's SNMT response.\n", stderr);
        return 7;
    }
    printf("Safety Node B (SADR %u) -> SCM A (instance %u): valid SNMT response\n",
           rx_header.w_adr, sender_node);
    printf("  frame length: %u bytes, state response: %u (PRE-OPERATIONAL)\n",
           received_length, payload[0]);
    return 0;
}
