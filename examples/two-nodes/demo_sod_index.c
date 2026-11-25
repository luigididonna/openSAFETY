#include "EPLScfg.h"
#include "EPLStarget.h"
#include "EPLStypes.h"
#include "SODapi.h"

extern SOD_t_OBJECT demo_sod_producer_od[];
extern SOD_t_OBJECT demo_sod_consumer_od[];

const SOD_t_OBJECT * const SAPL_ps_OD[EPLS_cfg_MAX_INSTANCES] = {
    demo_sod_producer_od,
#if (EPLS_cfg_MAX_INSTANCES > 1)
    demo_sod_consumer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 2)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 3)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 4)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 5)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 6)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 7)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 8)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 9)
    demo_sod_producer_od,
#endif
#if (EPLS_cfg_MAX_INSTANCES > 10)
    demo_sod_producer_od
#endif
};
