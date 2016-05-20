#ifndef __STDPRINT_H_INCLUDED__
#define __STDPRINT_H_INCLUDED__

#include "config.h"
#include "vdev_status.h"

void print_stats(config_t * cnf);
void print_stats_read_ops(config_t * cnf);
void print_stats_write_ops(config_t * cnf);
void print_stats_read_bts(config_t * cnf);
void print_stats_write_bts(config_t * cnf);
void print_stats_health_bool(config_t * cnf);
void print_stats_logical(config_t * cnf);
void print_stats_compress(config_t * cnf);
void print_stats_used(config_t * cnf);
void print_stats_real_used(config_t * cnf);
void print_stats_available(config_t * cnf);
void print_stats_dedupratio(config_t * cnf);
void print_stats_ddt_memory(config_t * cnf);
void print_status_message(zstatus_t * zstat);

#endif
