//
// Created by burso on 6/18/15.
//

#ifndef ZFS_ZABBIX_VDEV_STATUS_H
#define ZFS_ZABBIX_VDEV_STATUS_H
#include <sys/fs/zfs.h>
#include <libzfs.h>
#include "memlist.h"

#define ERR_MESSAGE_SIZE 1024

typedef struct zstatus zstatus_t;
int zpool_print_vdev(zpool_handle_t * zhp, void * data);
void print_spares(zpool_handle_t * zhp, nvlist_t ** spares, uint_t nspares, devlist_t * d);
void print_status_config(zpool_handle_t * zhp, const char * name, nvlist_t * nv,
                         devlist_t * d, int depth, boolean_t isspare);

struct zstatus {
    const char * name;
    char err_message[ERR_MESSAGE_SIZE];
    devlist_t d;
};


#endif //ZFS_ZABBIX_VDEV_STATUS_H
