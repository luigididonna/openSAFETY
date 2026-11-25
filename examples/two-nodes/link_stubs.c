#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SODapi.h"
#include "SERRapi.h"
#include <stdio.h>

/* Minimal application hooks needed by the initialized demonstration stack. */
void SAPL_SERR_SignalErrorClbk(BYTE_B_INSTNUM_ UINT16 error, UINT32 info)
{
    (void)b_instNum;
    fprintf(stderr, "openSAFETY stack error 0x%04X (info 0x%08lX)\n",
            error, (unsigned long)info);
}

BOOLEAN SAPL_SOD_DefaultValueSetClbk(BYTE_B_INSTNUM)
{
    (void)b_instNum;
    return TRUE;
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

BOOLEAN SAPL_SOD_ParameterSet_CLBK(BYTE_B_INSTNUM_ SOD_t_SERVICE service, const SOD_t_OBJECT *object, const void *data, UINT32 offset, UINT32 size, SOD_t_ABORT_CODES *error)
{
    (void)b_instNum; (void)service; (void)object; (void)data; (void)offset; (void)size;
    *error = SOD_ABT_UNSUPPORTED_ACCESS;
    return TRUE;
}



#include "SCMapi.h"
#include "SNMTSapi.h"
void SAPL_ScmUdidMismatchClbk(UINT16 sadr, const UINT8 *udid, UINT16 handle) {(void)sadr;(void)udid;(void)handle;}
void SAPL_ScmSnFailClbk(UINT16 sadr, UINT8 group, UINT8 code, UINT16 handle) {(void)sadr;(void)group;(void)code;(void)handle;}
void SAPL_ScmNodeStatusChangedClbk(UINT16 sadr, UINT8 old_status, UINT8 new_status) {(void)sadr;(void)old_status;(void)new_status;}
BOOLEAN SAPL_ScmRevisionNumberClbk(UINT16 sadr, UINT32 expected, UINT32 received) {(void)sadr;(void)expected;return expected==received;}
BOOLEAN SAPL_ScmProcessSn(UINT16 sadr) {(void)sadr;return TRUE;}
void SAPL_SNMTS_CalcParamChkSumClbk(BYTE_B_INSTNUM) {(void)b_instNum;}
void SAPL_SNMTS_SwitchToOpReqClbk(BYTE_B_INSTNUM) {(void)b_instNum;}
void SAPL_SNMTS_ErrorAckClbk(BYTE_B_INSTNUM_ UINT8 group, UINT8 code) {(void)b_instNum;(void)group;(void)code;}
BOOLEAN SAPL_SNMTS_ParameterSetProcessed(BYTE_B_INSTNUM) {(void)b_instNum;return TRUE;}
