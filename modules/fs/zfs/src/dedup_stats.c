//
// Created by burso on 7/8/15.
//

#include <sys/types.h>
#include <stdio.h>
#include "dedup_stats.h"


long long unsigned int
get_dedup_stats(nvlist_t * config) {
    ddt_object_t * ddo;
    unsigned int c;

    if (nvlist_lookup_uint64_array(config, ZPOOL_CONFIG_DDT_OBJ_STATS, (uint64_t **)&ddo, &c) != 0)
        return 0;

    if (ddo->ddo_count == 0) {
        return 0;
    }

    return (u_longlong_t)ddo->ddo_count * (u_longlong_t)ddo->ddo_mspace;
}