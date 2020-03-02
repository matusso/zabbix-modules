//
// Created by burso on 6/26/15.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memlist.h"

devlist_t * __getend(devlist_t * d);
unsigned long int __pool_devices_count(devlist_t * d);

void
init_devlist(devlist_t * d) {
    d->device = NULL;
    d->state  = NULL;
    d->message= NULL;
    d->next   = NULL;

    return;
}

int
add_to_devlist(devlist_t * d, const char * device, const char * state, const char * message, const char * pool) {
    while (d->next != NULL) {
        d = d->next;
    }

    d->device = (char *)malloc(strlen(device)+1 * sizeof(char));
    d->state = (char *)malloc(strlen(state)+1 * sizeof(char));
    d->next = (devlist_t *)malloc(sizeof(devlist_t));

    if (d->device == NULL || d->state == NULL || d->next == NULL) {
        fprintf(stderr, "could not allocate memory\n");
        return 1;
    }

    init_devlist(d->next);

    strcpy(d->device, device);
    strcpy(d->state, state);
    strncpy(d->pool, pool, MAXNAMELEN);
    d->message = message;

    return 0;
}

int
find_state_in_devlist(devlist_t * d, const char * search) {
    while (d->next != NULL) {
        if (strcmp(d->device, search) == 0) {
            if (    strcmp(d->state, "ONLINE") == 0 ||
                    strcmp(d->state, "AVAIL")  == 0 ||
                    strcmp(d->state, "INUSE")  == 0 )
                    printf("0\n");

            else
                    printf("1\n");
//            printf("device:\t%s, state:\t%s, error:\t%s\n", d->device, d->state, d->message);
            return 0;           // found
        }

        d = d->next;
    }

    return 1;                   // not found
}

void
print_devlist_text(devlist_t * d) {
    while (d->next != NULL) {
        printf("%s\n", d->device);

        d = d->next;
    }
    return;
}

void
print_devlist_json(devlist_t * d) {
    unsigned long int c = 1, cnt_devices;

    cnt_devices = __pool_devices_count(d);
    printf("{\n\t\"data\":[\n\n");


    while (d->next != NULL) {
        if (c != cnt_devices) {
            printf("\t\t{ \"{#DEVNAME}\":\"%s\",\t\t\"{#DEVTYPE}\":\"disk\" },\n", d->device);
        } else {
            printf("\t\t{ \"{#DEVNAME}\":\"%s\",\t\t\"{#DEVTYPE}\":\"disk\" }\n", d->device);
        }
        c++;

        d = d->next;
    }
    printf("\n\t]\n}\n");
    return;
}

void
free_devlist(devlist_t * d) {
    devlist_t * end = NULL;
    long unsigned int c = 0, cnt_devices = 0;

    cnt_devices = __pool_devices_count(d);

    while(c <= cnt_devices-1) {
        end = __getend(d);

        free(end->device);
        free(end->next);
        free(end->state);
        end->next = NULL;

        c++;
    }

    return;
}

devlist_t *
__getend(devlist_t * d) {
    while (d->next->next != NULL) {
        d = d->next;
    }

    return d;
}

unsigned long int
__pool_devices_count(devlist_t * d) {
    unsigned long int counter = 0;
    while (d->next != NULL) {
        counter++;
        d = d->next;
    }

    return counter;
}
