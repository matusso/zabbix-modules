#include <stdio.h>
#include <string.h>

#include "stdprint.h"

void
print_stats(config_t * cnf) {
	fprintf(stdout,	"zpool:\t\t%s\n" 
			"read_ops:\t%llu\n"
			"write_ops:\t%llu\n"
			"read_bts:\t%llu\n"
			"write_bts:\t%llu\n"
			"alloc:\t\t%llu\n"
			"free:\t\t%llu\n"
			"health:\t\t%s\n"
			"ddt_memory:\t%llu\n"
			"dedupratio:\t%.2f\n\n",
	cnf->zpool.name, cnf->zpool.read_ops, cnf->zpool.write_ops, cnf->zpool.read_bts, 
	cnf->zpool.write_bts, cnf->zpool.alloc, cnf->zpool.free, cnf->zpool.health, cnf->zpool.ddt_memory,
	cnf->zpool.dedupratio);

	fprintf(stdout, "zfs:\t\t%s\n"
			"used:\t\t%llu\n"
			"available:\t%llu\n"
			"logical:\t%llu\n"
			"compressratio:\t%.2f\n",
	cnf->zfs.name, cnf->zfs.used, cnf->zfs.available, cnf->zfs.logical, cnf->zfs.compressratio);
	return; 
}

void
print_stats_read_ops(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.read_ops);
	return;
}

void
print_stats_write_ops(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.write_ops);
	return;
}

void
print_stats_read_bts(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.read_bts);
	return;
}

void
print_stats_write_bts(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.write_bts);
	return;
}

void
print_stats_health_bool(config_t * cnf) {
	if (!strcmp("ONLINE", cnf->zpool.health)) fprintf(stdout, "0\n");
	else fprintf(stdout, "1\n");

	return;
}

void
print_stats_logical(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zfs.logical);
	return;
}

void
print_stats_compress(config_t * cnf) {
        if (cnf->zfs.logical > cnf->zfs.used)
                fprintf(stdout, "%llu\n", cnf->zfs.logical - cnf->zfs.used);
        else
                fprintf(stdout, "0\n");

        return;
}

void
print_stats_used(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zfs.used);
	return;
}

void
print_stats_real_used(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.alloc);
	return;
}

void
print_stats_available(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zfs.available);
	return;
}

void
print_stats_dedupratio(config_t * cnf) {
	fprintf(stdout, "%.2f\n", cnf->zpool.dedupratio);
	return;
}

void
print_stats_ddt_memory(config_t * cnf) {
	fprintf(stdout, "%llu\n", cnf->zpool.ddt_memory);
	return;
}

void
print_status_message(zstatus_t * zstat) {
	fprintf(stdout, "pool: %s\n", zstat->name);
	fprintf(stdout, "%s\n", zstat->err_message);
	return;
}