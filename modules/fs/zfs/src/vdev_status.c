//
// Created by burso on 6/18/15.
//

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "zio.h"
#include "config.h"
#include "vdev_status.h"
#include "memlist.h"

int
zpool_print_vdev(zpool_handle_t * zhp, void * data) {
    zstatus_t * zstat = (zstatus_t *)data;

    zpool_status_t reason;
    uint_t c;

    char * msgId = NULL;
    nvlist_t * config, * nvroot;
    vdev_stat_t * vs;

#if defined(LINUX)
    zpool_errata_t errata;
    reason = zpool_get_status(zhp, &msgId, &errata);
#else
    reason = zpool_get_status(zhp, &msgId);
#endif

    config = zpool_get_config(zhp, NULL);

    verify(nvlist_lookup_nvlist(config, ZPOOL_CONFIG_VDEV_TREE, &nvroot) == 0);
    verify(nvlist_lookup_uint64_array(nvroot, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vs, &c) == 0);

    if (strcmp(zpool_get_name(zhp), zstat->name) == 0) {
        switch (reason) {
            case ZPOOL_STATUS_MISSING_DEV_R:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices could not "
                        "be opened.  Sufficient replicas exist for\n\tthe pool to "
                        "continue functioning in a degraded state.\n");
                break;

            case ZPOOL_STATUS_MISSING_DEV_NR:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices could not "
                        "be opened.  There are insufficient\n\treplicas for the "
                        "pool to continue functioning.\n");
                break;

            case ZPOOL_STATUS_CORRUPT_LABEL_R:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices could not "
                        "be used because the label is missing or\n\tinvalid.  "
                        "Sufficient replicas exist for the pool to continue\n\t"
                        "functioning in a degraded state.\n"
                        "action: Replace the device using "
                        "'zpool replace'.\n");
                break;

            case ZPOOL_STATUS_CORRUPT_LABEL_NR:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices could not "
                        "be used because the label is missing \n\tor invalid.  "
                        "There are insufficient replicas for the pool to "
                        "continue\n\tfunctioning.\n");
                zpool_explain_recover(zpool_get_handle(zhp), zpool_get_name(zhp), reason, config);
                break;

            case ZPOOL_STATUS_FAILING_DEV:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices has "
                        "experienced an unrecoverable error.  An\n\tattempt was "
                        "made to correct the error.  Applications are "
                        "unaffected.\n"
                        "action: Determine if the device needs "
                        "to be replaced, and clear the errors\n\tusing "
                        "'zpool clear' or replace the device with 'zpool "
                        "replace'.\n");
                break;

            case ZPOOL_STATUS_OFFLINE_DEV:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices has "
                        "been taken offline by the administrator.\n\tSufficient "
                        "replicas exist for the pool to continue functioning in "
                        "a\n\tdegraded state.\n"
                        "action: Online the device using "
                        "'zpool online' or replace the device with\n\t'zpool "
                        "replace'.\n");
                break;

            case ZPOOL_STATUS_REMOVED_DEV:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices has "
                        "been removed by the administrator.\n\tSufficient "
                        "replicas exist for the pool to continue functioning in "
                        "a\n\tdegraded state.\n"
                        "action: Online the device using "
                        "'zpool online' or replace the device with\n\t'zpool "
                        "replace'.\n");

                break;

            case ZPOOL_STATUS_RESILVERING:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices is "
                        "currently being resilvered.  The pool will\n\tcontinue "
                        "to function, possibly in a degraded state.\n"
                        "action: Wait for the resilver to complete.\n");
                break;

            case ZPOOL_STATUS_CORRUPT_DATA:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices has "
                        "experienced an error resulting in data\n\tcorruption.  "
                        "Applications may be affected.\n"
                        "action: Restore the file in question "
                        "if possible.  Otherwise restore the\n\tentire pool from "
                        "backup.\n");
                break;

            case ZPOOL_STATUS_CORRUPT_POOL:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: The pool metadata is corrupted "
                        "and the pool cannot be opened.\n");

                zpool_explain_recover(zpool_get_handle(zhp), zpool_get_name(zhp), reason, config);
                break;

            case ZPOOL_STATUS_VERSION_OLDER:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: The pool is formatted using a "
                        "legacy on-disk format.  The pool can\n\tstill be used, "
                        "but some features are unavailable.\n"
                        "action: Upgrade the pool using 'zpool "
                        "upgrade'.  Once this is done, the\n\tpool will no longer "
                        "be accessible on software that does not support\n\t"
                        "feature flags.\n");
                break;

            case ZPOOL_STATUS_VERSION_NEWER:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: The pool has been upgraded to a "
                        "newer, incompatible on-disk version.\n\tThe pool cannot "
                        "be accessed on this system.\n"
                        "action: Access the pool from a system "
                        "running more recent software, or\n\trestore the pool from "
                        "backup.\n");
                break;

            case ZPOOL_STATUS_FEAT_DISABLED:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: Some supported features are not "
                        "enabled on the pool. The pool can\n\tstill be used, but "
                        "some features are unavailable.\n"
                        "action: Enable all features using "
                        "'zpool upgrade'. Once this is done,\n\tthe pool may no "
                        "longer be accessible by software that does not support\n\t"
                        "the features. See zpool-features(5) for details.\n");
                break;

            case ZPOOL_STATUS_UNSUP_FEAT_READ:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: The pool cannot be accessed on "
                        "this system because it uses the\n\tfollowing feature(s) "
                        "not supported on this system.\n"
                        "action: Access the pool from a system "
                        "that supports the required feature(s),\n\tor restore the "
                        "pool from backup.\n");
                break;

            case ZPOOL_STATUS_UNSUP_FEAT_WRITE:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: The pool can only be accessed "
                        "in read-only mode on this system. It\n\tcannot be "
                        "accessed in read-write mode because it uses the "
                        "following\n\tfeature(s) not supported on this system:\n"
                        "action: The pool cannot be accessed in "
                        "read-write mode. Import the pool with\n"
                        "\t\"-o readonly=on\", access the pool from a system that "
                        "supports the\n\trequired feature(s), or restore the "
                        "pool from backup.\n");
                break;

            case ZPOOL_STATUS_FAULTED_DEV_R:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices are "
                        "faulted in response to persistent errors.\n\tSufficient "
                        "replicas exist for the pool to continue functioning "
                        "in a\n\tdegraded state.\n"
                        "action: Replace the faulted device, "
                        "or use 'zpool clear' to mark the device\n\trepaired.\n");
                break;

            case ZPOOL_STATUS_FAULTED_DEV_NR:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices are "
                        "faulted in response to persistent errors.  There are "
                        "insufficient replicas for the pool to\n\tcontinue "
                        "functioning.\n"
                        "action: Destroy and re-create the pool "
                        "from a backup source.  Manually marking the device\n"
                        "\trepaired using 'zpool clear' may allow some data "
                        "to be recovered.\n");
                break;

            case ZPOOL_STATUS_IO_FAILURE_WAIT:
            case ZPOOL_STATUS_IO_FAILURE_CONTINUE:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: One or more devices are "
                        "faulted in response to IO failures.\n"
                        "action: Make sure the affected devices "
                        "are connected, then run 'zpool clear'.\n");
                break;

            case ZPOOL_STATUS_BAD_LOG:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: An intent log record "
                        "could not be read.\n"
                        "\tWaiting for adminstrator intervention to fix the "
                        "faulted pool.\n"
                        "action: Either restore the affected "
                        "device(s) and run 'zpool online',\n"
                        "\tor ignore the intent log records by running "
                        "'zpool clear'.\n");

            case ZPOOL_STATUS_HOSTID_MISMATCH:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: Mismatch between pool hostid "
                        "and system hostid on imported pool.\n\tThis pool was "
                        "previously imported into a system with a different "
                        "hostid,\n\tand then was verbatim imported into this "
                        "system.\n"
                        "action: Export this pool on all systems "
                        "on which it is imported.\n"
                        "\tThen import it to correct the mismatch.\n");
                break;


            default:
                (void) snprintf(zstat->err_message, ERR_MESSAGE_SIZE, "status: OK\n");
                assert(reason == ZPOOL_STATUS_OK);
        }
    }

    /*
     * todo: have to processed message to variable..
    if (msgId != NULL)
        (void) fprintf(stderr, "   see: http://zfsonlinux.org/msg/%s\n", msgId);
    */

    if (config != NULL) {
        nvlist_t ** spares;
        uint_t nspares;
        pool_scan_stat_t * ps = NULL;

        (void) nvlist_lookup_uint64_array(nvroot, ZPOOL_CONFIG_SCAN_STATS, (uint64_t **)&ps, &c);
            print_status_config(zhp, zpool_get_name(zhp), nvroot, &zstat->d, 0, B_FALSE);

        if (nvlist_lookup_nvlist_array(nvroot, ZPOOL_CONFIG_SPARES, &spares, &nspares) == 0) {
            print_spares(zhp, spares, nspares, &zstat->d);
        }

    }

    zpool_close(zhp);
    return 0;
}

void
print_status_config(zpool_handle_t * zhp, const char * name, nvlist_t * nv,
                    devlist_t * d, int depth, boolean_t isspare) {

    nvlist_t ** child;
    uint_t c, children;
    vdev_stat_t * vs;
    uint64_t notpresent;
    char * vname;
    const char * state = NULL;
    const char * err_msg = NULL;
    char * type = NULL;

    if (nvlist_lookup_nvlist_array(nv, ZPOOL_CONFIG_CHILDREN, &child, &children) != 0)
            children = 0;

    verify(nvlist_lookup_uint64_array(nv, ZPOOL_CONFIG_VDEV_STATS, (uint64_t **)&vs, &c) == 0);

    state = zpool_state_to_name(vs->vs_state, vs->vs_aux);
    if (isspare) {
        if (vs->vs_aux == VDEV_AUX_SPARED)
            state = "INUSE";
        else if (vs->vs_state == VDEV_STATE_HEALTHY)
            state = "AVAIL";
    }

    if (nvlist_lookup_uint64(nv, ZPOOL_CONFIG_NOT_PRESENT, &notpresent) == 0) {
        char *path;
        verify(nvlist_lookup_string(nv, ZPOOL_CONFIG_PATH, &path) == 0);
        (void) fprintf(stderr, "  was %s", path);

    } else if (vs->vs_aux != 0) {
        switch (vs->vs_aux) {
            case VDEV_AUX_OPEN_FAILED:
                err_msg = "cannot open";
                break;

            case VDEV_AUX_BAD_GUID_SUM:
                err_msg = "missing device";
                break;

            case VDEV_AUX_NO_REPLICAS:
                err_msg = "insufficient replicas";
                break;

            case VDEV_AUX_VERSION_NEWER:
                err_msg = "newer version";
                break;

            case VDEV_AUX_UNSUP_FEAT:
                err_msg = "unsupported feature(s)";
                break;

            case VDEV_AUX_SPARED:
                err_msg = "currently in use";
                break;

            case VDEV_AUX_ERR_EXCEEDED:
                err_msg = "too many errors";
                break;

            case VDEV_AUX_IO_FAILURE:
                err_msg = "experienced I/O failures";
                break;

            case VDEV_AUX_BAD_LOG:
                err_msg = "bad intent log";
                break;

            case VDEV_AUX_EXTERNAL:
                err_msg = "external device fault";
                break;

            case VDEV_AUX_SPLIT_POOL:
                err_msg = "split into new pool";
                break;

            default:
                err_msg = "corrupted data";
                break;
        }

    }

    for (c = 0; c < children; c++) {
        vname = zpool_vdev_name(g_zfs, zhp, child[c], B_TRUE);
        print_status_config(zhp, vname, child[c], d, depth + 2, isspare);
        free(vname);
    }

    verify(nvlist_lookup_string(nv, ZPOOL_CONFIG_TYPE, &type) == 0);

    if (strcmp(type, VDEV_TYPE_DISK) == 0) {
        add_to_devlist(d, name, state, err_msg, zpool_get_name(zhp));
    }

    return;
}

void
print_spares(zpool_handle_t * zhp, nvlist_t ** spares, uint_t nspares, devlist_t * d) {
    char * name = NULL;
    uint_t i;

    if (nspares == 0)
        return;

    for (i = 0; i < nspares; i++) {
        name = zpool_vdev_name(g_zfs, zhp, spares[i], B_FALSE);
        print_status_config(zhp, name, spares[i], d, 0, B_TRUE);
        free(name);
    }
    return;
}