#ifndef DEMO_SOD_CONFIG_H
#define DEMO_SOD_CONFIG_H
#include "EPLStarget.h"
#include "EPLScfg.h"
#include "EPLStypes.h"
#define SAPL_k_SCM_SN1_SADR 1U
#define SAPL_k_SN2_SADR 2U
#define SAPL_k_SN2_ADD_SADR_1 4U
#define SAPL_k_SN2_UDID {0x00U,0x60U,0x65U,0x00U,0x00U,0x00U}
#ifdef DEMO_SOD_CONSUMER
#define DEMO_SPDO_RX_SOURCE_SADR SAPL_k_SCM_SN1_SADR
#define DEMO_SPDO_TX_SOURCE_SADR SAPL_k_SN2_ADD_SADR_1
#else
#define DEMO_SPDO_RX_SOURCE_SADR SAPL_k_SN2_ADD_SADR_1
#define DEMO_SPDO_TX_SOURCE_SADR SAPL_k_SCM_SN1_SADR
#endif
#include "demo_sod_spdo_config.h"
#endif

