#include <string.h>

#include "loopback_shnf.h"

#define LOOPBACK_MAX_FRAME 600u

static UINT8 tx_buffers[EPLS_cfg_MAX_INSTANCES][LOOPBACK_MAX_FRAME];
static UINT16 tx_lengths[EPLS_cfg_MAX_INSTANCES];
static BOOLEAN tx_ready[EPLS_cfg_MAX_INSTANCES];
static UINT8 rx_buffers[EPLS_cfg_MAX_INSTANCES][LOOPBACK_MAX_FRAME];
static UINT16 rx_lengths[EPLS_cfg_MAX_INSTANCES];
static BOOLEAN rx_ready[EPLS_cfg_MAX_INSTANCES];

UINT32 SHNF_aaulConnValidBit[EPLS_cfg_MAX_INSTANCES]
    [(SPDO_cfg_MAX_NO_RX_SPDO + 31) / 32];
UINT32 SHNF_aaulConnValidStatistic[EPLS_cfg_MAX_INSTANCES]
    [SPDO_cfg_MAX_NO_RX_SPDO];

UINT8 *SHNF_GetTxMemBlock(BYTE_B_INSTNUM_ UINT16 size,
                          SHNF_t_TEL_TYPE type, UINT16 spdo_number)
{
    (void)type;
    (void)spdo_number;
    if (size > LOOPBACK_MAX_FRAME || tx_ready[B_INSTNUM])
        return NULL;
    tx_lengths[B_INSTNUM] = size;
    memset(tx_buffers[B_INSTNUM], 0, size);
    return tx_buffers[B_INSTNUM];
}

BOOLEAN SHNF_MarkTxMemBlock(BYTE_B_INSTNUM_ const UINT8 *block)
{
    if (block != tx_buffers[B_INSTNUM] || tx_lengths[B_INSTNUM] == 0u)
        return FALSE;
    tx_ready[B_INSTNUM] = TRUE;
    return TRUE;
}

UINT8 HNFiff_Crc8CalcSwp(UINT8 init, INT32 length, const void *data)
{
    const UINT8 *bytes = (const UINT8 *)data;
    UINT8 crc = init;
    INT32 i;
    UINT8 bit;
    for (i = 0; i < length; ++i)
    {
        crc ^= bytes[i];
        for (bit = 0; bit < 8u; ++bit)
            crc = (UINT8)((crc & 0x80u) ? ((crc << 1u) ^ 0x2Fu) : (crc << 1u));
    }
    return crc;
}

static UINT16 crc16(UINT16 init, INT32 length, const void *data, UINT16 polynomial)
{
    const UINT8 *bytes = (const UINT8 *)data;
    UINT16 crc = init;
    INT32 i;
    UINT8 bit;
    for (i = 0; i < length; ++i)
    {
        crc ^= (UINT16)((UINT16)bytes[i] << 8u);
        for (bit = 0; bit < 8u; ++bit)
            crc = (UINT16)((crc & 0x8000u) ? ((crc << 1u) ^ polynomial) : (crc << 1u));
    }
    return crc;
}

UINT16 HNFiff_Crc16CalcSwp(UINT16 init, INT32 length, const void *data)
{
    return crc16(init, length, data, 0xAC9Au);
}

UINT16 HNFiff_Crc16_755B_CalcSwp(UINT16 init, INT32 length, const void *data)
{
    return crc16(init, length, data, 0x755Bu);
}

BOOLEAN loopback_take_tx(UINT8 source_instance, UINT8 *frame, UINT16 *length)
{
    UINT16 crc_length;
    UINT16 payload_length;
    UINT16 subframe_one_length;
    UINT16 subframe_two_length;
    if (source_instance >= EPLS_cfg_MAX_INSTANCES || !tx_ready[source_instance] ||
        frame == NULL || length == NULL)
        return FALSE;
    *length = tx_lengths[source_instance];
    /* SFS_FrmSerialize hands SHNF the subframes in reversed memory order.
     * Normalize to the receive order expected by SFS_FrmDeSerialize. */
    crc_length = (*length <= 27u) ? 1u : 2u;
    if (*length < (UINT16)(9u + 2u * crc_length) ||
        ((*length - 9u - 2u * crc_length) & 1u) != 0u)
        return FALSE;
    payload_length = (UINT16)((*length - 9u - 2u * crc_length) / 2u);
    subframe_one_length = (UINT16)(4u + payload_length + crc_length);
    subframe_two_length = (UINT16)(5u + payload_length + crc_length);
    memcpy(frame, tx_buffers[source_instance] + subframe_two_length,
           subframe_one_length);
    memcpy(frame + subframe_one_length, tx_buffers[source_instance],
           subframe_two_length);
    tx_ready[source_instance] = FALSE;
    tx_lengths[source_instance] = 0u;
    return TRUE;
}

BOOLEAN loopback_deliver(UINT8 destination_instance, const UINT8 *frame,
                         UINT16 length)
{
    if (destination_instance >= EPLS_cfg_MAX_INSTANCES || frame == NULL ||
        length == 0u || length > LOOPBACK_MAX_FRAME || rx_ready[destination_instance])
        return FALSE;
    memcpy(rx_buffers[destination_instance], frame, length);
    rx_lengths[destination_instance] = length;
    rx_ready[destination_instance] = TRUE;
    return TRUE;
}

BOOLEAN loopback_take_rx(UINT8 destination_instance, UINT8 *frame,
                         UINT16 *length)
{
    if (destination_instance >= EPLS_cfg_MAX_INSTANCES || !rx_ready[destination_instance] ||
        frame == NULL || length == NULL)
        return FALSE;
    *length = rx_lengths[destination_instance];
    memcpy(frame, rx_buffers[destination_instance], *length);
    rx_lengths[destination_instance] = 0u;
    rx_ready[destination_instance] = FALSE;
    return TRUE;
}
