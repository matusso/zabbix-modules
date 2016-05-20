#ifndef __CONFIG_H_INCLUDED__
#define __CONFIG_H_INCLUDED__

#include <stdbool.h>
#include <sys/types.h>
#include <sys/fs/zfs.h>

#define ZIO_VERSION 0.3
#define VDEV_MAXNAMELEN 256

enum ft_type {
	TP_UNDEF = 0,
	TP_TEXT,
	TP_JSON
};

enum sw_type {
	SW_UNDEF = 0,
	SW_ALL,
	SW_READ_OPS,
	SW_WRITE_OPS,
	SW_READ_BTS,
	SW_WRITE_BTS,
	SW_HEALTH,
	SW_LOGICAL,
	SW_COMPRESS,
	SW_USED,
	SW_REAL_USED,
	SW_AVAILABLE,
	SW_DEDUPRATIO,
	SW_POOLS,
	SW_DEVICES,
	SW_DEVSTATE,
	SW_DDT,
	SW_ERR_MESSAGE,
};

typedef struct zfs_data {
	float compressratio;
	const char * name;
	long long unsigned int used;
	long long unsigned int available;
	long long unsigned int real;
	long long unsigned int logical;
	long long unsigned int compress;
} zfs_data_t;

typedef struct zpool_data {
	float dedupratio;
	const char * name;
	long long unsigned int read_ops;
	long long unsigned int write_ops;
	long long unsigned int read_bts;
	long long unsigned int write_bts;
	long long unsigned int alloc;
	long long unsigned int free;
	long long unsigned int ddt_memory;
	const char * health;
} zpool_data_t;

typedef struct config {
	unsigned int ft;
	unsigned int sw;

	char zname[ZAP_MAXNAMELEN];
	char vdev[VDEV_MAXNAMELEN];

	zpool_data_t zpool;	
	zfs_data_t zfs;
} config_t;

void init_config(config_t * cnf);
void get_config(int argc, char ** argv, config_t * cnf);

#endif
