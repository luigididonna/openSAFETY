#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SODapi.h"
#include "SERRapi.h"
#include <stdio.h>

/* The frame-codec demonstration does not initialize or access an SOD. These
 * link-time application hooks make that limited scope explicit. */
const SOD_t_OBJECT * const SAPL_ps_OD[EPLS_cfg_MAX_INSTANCES] = { NULL };

void SAPL_SERR_SignalErrorClbk(BYTE_B_INSTNUM_ UINT16 error, UINT32 info)
{
    (void)b_instNum;
    fprintf(stderr, "openSAFETY stack error 0x%04X (info 0x%08lX)\n",
            error, (unsigned long)info);
}

BOOLEAN SAPL_SOD_DefaultValueSetClbk(BYTE_B_INSTNUM)
{
    (void)b_instNum;
    return FALSE;
}

const SOD_t_ATTR *SAPL_SOD_AttrGetClbk(BYTE_B_INSTNUM_ UINT16 index,
                                       UINT8 subindex, UINT32 *handle,
                                       SOD_t_ABORT_CODES *error)
{
    (void)b_instNum;
    (void)index;
    (void)subindex;
    (void)handle;
    *error = SOD_ABT_OBJ_DOES_NOT_EXIST;
    return NULL;
}

void *SAPL_SOD_ReadClbk(BYTE_B_INSTNUM_ UINT32 handle, UINT32 offset,
                        UINT32 size, SOD_t_ABORT_CODES *error)
{
    (void)b_instNum;
    (void)handle;
    (void)offset;
    (void)size;
    *error = SOD_ABT_OBJ_DOES_NOT_EXIST;
    return NULL;
}

SOD_t_ABORT_CODES SAPL_SOD_WriteClbk(BYTE_B_INSTNUM_ UINT32 handle,
                                     const void *data, BOOLEAN overwrite,
                                     UINT32 offset, UINT32 size)
{
    (void)b_instNum;
    (void)handle;
    (void)data;
    (void)overwrite;
    (void)offset;
    (void)size;
    return SOD_ABT_OBJ_DOES_NOT_EXIST;
}

BOOLEAN SAPL_SOD_LockClbk(BYTE_B_INSTNUM_ UINT32 handle)
{
    (void)b_instNum;
    (void)handle;
    return TRUE;
}

BOOLEAN SAPL_SOD_UnlockClbk(BYTE_B_INSTNUM_ UINT32 handle)
{
    (void)b_instNum;
    (void)handle;
    return TRUE;
}

SOD_t_ABORT_CODES SAPL_SOD_ActualLenSet(BYTE_B_INSTNUM_ UINT32 handle,
                                        UINT32 length)
{
    (void)b_instNum;
    (void)handle;
    (void)length;
    return SOD_ABT_OBJ_DOES_NOT_EXIST;
}

SOD_t_ABORT_CODES SAPL_SOD_ActualLenGet(BYTE_B_INSTNUM_ UINT32 handle,
                                        UINT32 *length)
{
    (void)b_instNum;
    (void)handle;
    (void)length;
    return SOD_ABT_OBJ_DOES_NOT_EXIST;
}
