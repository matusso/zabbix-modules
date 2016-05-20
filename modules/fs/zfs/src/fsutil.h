#ifndef __FSUTIL_H_INCLUDED__
#define __FSUTIL_H_INCLUDED__

#include <libzfs.h>
#include <sys/types.h>

/* zpool functions */
int zpool_get_count(zpool_handle_t * zhp, void * data);
int show_zpools(libzfs_handle_t * g_zfs);
int show_zpools_json(libzfs_handle_t * g_zfs);
float zpool_get_dedupratio(zpool_handle_t * zhp);
const char * zpool_get_health(zpool_handle_t * zhp);
const char * zpool_get_poolname(zpool_handle_t * zhp);

/* zfs functions */
long long unsigned int zfs_get_used(zfs_handle_t * zhf);
long long unsigned int zfs_get_available(zfs_handle_t * zhf);
long long unsigned int zfs_get_logical(zfs_handle_t * zhf);
float zfs_get_compressratio(zfs_handle_t * zhf);
const char * zfs_get_fsname(zfs_handle_t * zhf);


#endif
