#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdarg.h>

#include "config.h"

void usage(char * app_name);

void get_config(int argc, char ** argv, config_t * cnf) {
	int c = 0;

	while (1) {
		static struct option long_options[] = {
            	/* These options don't set a flag.
                	We distinguish them by their indices. */
                	{"version",         no_argument,        0, 'v'},
                	{"help",            no_argument,        0, 'h'},
                	{"show",            required_argument,  0, 's'},
					{"zpool",	    	required_argument,  0, 'z'},
                	{"format",          required_argument,  0, 'f'},
					{"device",			required_argument,  0, 'd'},
            		{0, 0, 0, 0}
		};
	
		/* getopt_long stores the option index here. */
        	int option_index = 0;
        	c = getopt_long (argc, argv, "hvs:f:z:d:", long_options, &option_index);
	
		/* Detect the end of the options. */
        	if (c == -1)
               		break;

		switch (c) {
			case 0:
                	/* If this option set a flag, do nothing else now. */
                        	if (long_options[option_index].flag != 0)
                                	break;

                        	printf ("option %s", long_options[option_index].name);
                        	if (optarg)
                                	fprintf(stdout, " with arg %s", optarg);

                        	printf ("\n");
                        	break;

			case 's':
				if 	(!strcmp("all", optarg)) 	cnf->sw = SW_ALL;
				else if (!strcmp("read_ops", optarg)) 	cnf->sw = SW_READ_OPS;
				else if (!strcmp("write_ops", optarg))	cnf->sw = SW_WRITE_OPS;
				else if (!strcmp("read_bts", optarg))	cnf->sw = SW_READ_BTS;
				else if (!strcmp("write_bts", optarg))	cnf->sw = SW_WRITE_BTS;
				else if (!strcmp("health", optarg))	cnf->sw = SW_HEALTH;
				else if (!strcmp("logical", optarg))	cnf->sw = SW_LOGICAL;
				else if (!strcmp("compress", optarg))	cnf->sw = SW_COMPRESS;
				else if (!strcmp("used", optarg))	cnf->sw = SW_USED;
				else if (!strcmp("real", optarg))	cnf->sw = SW_REAL_USED;
				else if (!strcmp("available", optarg))	cnf->sw = SW_AVAILABLE;
				else if (!strcmp("pools", optarg))	cnf->sw = SW_POOLS;
				else if (!strcmp("devices", optarg)) cnf->sw = SW_DEVICES;
				else if (!strcmp("device-state", optarg)) cnf->sw = SW_DEVSTATE;
				else if (!strcmp("dedupratio", optarg)) cnf->sw = SW_DEDUPRATIO;
				else if (!strcmp("ddt-memory", optarg)) cnf->sw = SW_DDT;
				else if (!strcmp("status", optarg)) cnf->sw = SW_ERR_MESSAGE;
				else {
					fprintf(stderr, "unknown show type: %s\n", optarg);
					exit(1);
				}
				break;

			case 'f':
				if (!strcmp("text", optarg)) cnf->ft = TP_TEXT;
				else if (!strcmp("json", optarg)) cnf->ft = TP_JSON;
				else { 
					fprintf(stderr, "unknown format type: %s\n", optarg);
					exit(1);
				}
				break;

			case 'z':
				strncpy(cnf->zname, optarg, sizeof(cnf->zname));
				break;

			case 'd':
				strncpy(cnf->vdev, optarg, sizeof(cnf->vdev));
				break;
			
			case 'h':
				usage(argv[0]);
				exit(0);

			case 'v':
				fprintf(stdout, "%s version %.1lf\n", argv[0], ZIO_VERSION);
				exit(0);

			case '?':
				/* getopt_long already printed an error message. */
                		fprintf(stderr, "%s -h | --help for show help\n", argv[0]);
                		exit(1);

			default:
				abort();	

		}	
	}

	/* Print any remaining command line arguments (not options). */
        if (optind < argc) {
                printf ("non-option ARGV-elements: ");
                while (optind < argc)
                        printf("%s ", argv[optind++]);

                putchar('\n');
                exit(1);
        }


	return;
}

void
init_config(config_t * cnf) {
	cnf->sw = SW_UNDEF;
	cnf->ft = TP_UNDEF;			

	/* init zpool parameters */
	cnf->zpool.read_ops = 0;
	cnf->zpool.write_ops = 0;
	cnf->zpool.read_bts = 0;
	cnf->zpool.write_bts = 0;
	cnf->zpool.alloc = 0;
	cnf->zpool.free = 0;
	cnf->zpool.health = NULL;
	cnf->zpool.dedupratio = 0;
	cnf->zpool.ddt_memory = 0;
	cnf->zpool.name = NULL;
	
	/* init zfs parameters */
	cnf->zfs.used = 0;
	cnf->zfs.compressratio = 0;
	cnf->zfs.available = 0;
	cnf->zfs.logical = 0;	
	cnf->zfs.name = NULL;
	
	return;
}

void
usage(char * app_name) {

	fprintf(stdout, "%s: [-s <show>][-f <format>][-z <zpool>][-d <device>]\n\n", app_name);
	fprintf(stdout, " -z | --zpool\t\tset zpool\n");
	fprintf(stdout, " -s | --show\t\tshow zpools name\n");
	fprintf(stdout, " -d | --device\t\tset device\n");
	fprintf(stdout, " -f | --format\t\tset type of format [text, json]\n");
	fprintf(stdout, " -h | --help\t\tshow this help menu\n");
	fprintf(stdout, " -v | --version\t\tshow version\n");

	fprintf(stdout, "\nparameters for -s:\n"
			"\tall\t\t- print all statistics\n"
			"\tread_ops\t- print read io operations\n"
			"\twrite_ops\t- print write io operations\n"
			"\tread_bts\t- print read bytes per seconds\n"
			"\twrite_bts\t- print write bytes per seconds\n"
			"\thealth\t\t- print health of zpool\n"
			"\tlogical\t\t- print logical used space\n"
			"\tcompress\t- print space saved by compress\n"
			"\tdedupratio\t- print dedupratio of zpool\n"
			"\tused\t\t- print used space\n"
			"\treal\t\t- print real used space after dedup\n"
			"\tavailable\t- print available space\n\n"
			"\tpools\t\t- print pools\n"
	        "\tdevices\t\t- print devices in zpool\n"
			"\tdevice-state\t- print state of device\n"
			"\tddt-memory\t- print size of deduplication table in memory\n"
			"\tstatus\t\t- print status message of pool\n");
	return;
}
