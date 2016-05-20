//
// Created by burso on 7/8/15.
//

#ifndef ZFS_ZABBIX_DEDUP_STATS_H
#define ZFS_ZABBIX_DEDUP_STATS_H

#include <sys/fs/zfs.h>
#include <libzfs.h>

long long unsigned int get_dedup_stats(nvlist_t * config);

#endif //ZFS_ZABBIX_DEDUP_STATS_H
