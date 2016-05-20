//
// Created by burso on 6/26/15.
//

#ifndef ZFS_ZABBIX_MEMLIST_H
#define ZFS_ZABBIX_MEMLIST_H

#include <libzfs.h>

typedef struct devlist devlist_t;
int add_to_devlist(devlist_t * d, const char * device, const char * state, const char * message, const char * pool);
void init_devlist(devlist_t * d);
void free_devlist(devlist_t * d);
int find_state_in_devlist(devlist_t * d, const char * search);
void print_devlist_text(devlist_t * d);
void print_devlist_json(devlist_t * d);

struct devlist {
    char * device;
    char * state;
    const char * message;
    char pool[ZPOOL_MAXNAMELEN];

    devlist_t * next;
};

#endif //ZFS_ZABBIX_MEMLIST_H
