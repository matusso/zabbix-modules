# zfs-zabbix
zabbix advanced monitoring of zfs

#### help

<pre>
zio --help
zio: [-s <show>][-f <format>][-z <zpool>][-d <device>]

 -z | --zpool           set zpool
 -s | --show            show zpools name
 -d | --device          set device
 -f | --format          set type of format [text, json]
 -h | --help            show this help menu
 -v | --version         show version

parameters for -s:

        all             - print all statistics
        read_ops        - print read io operations
        write_ops       - print write io operations
        read_bts        - print read bytes per seconds
        write_bts       - print write bytes per seconds
        health          - print health of zpool
        logical         - print logical used space
        compress        - print space saved by compress
        dedupratio      - print dedupratio of zpool
        used            - print used space
        real            - print real used space after dedup
        available       - print available space
        pools           - print pools
        devices         - print devices in zpool
        device-state    - print state of device
        ddt-memory      - print size of deduplication table in memory
</pre>

#### installation

##### omnios:
<pre>
~# gmake CC="path_to_compiler -DSOLARIS"
</pre>

##### openindiana:<br>
<pre>
~# gmake CC="path_to_compiler -DOI"
</pre>

hint: problem with linker
<pre>
ld: fatal: file crt1.o: open failed: No such file or directory
collect2: ld returned 1 exit status
gmake: *** [zio] Error 1
</pre>

solution: install package "lint"
<pre>
~# pkg install pkg://openindiana.org/developer/library/lint
</pre>



##### freebsd:
<pre>
~# gmake -f Makefile
</pre>

##### linux:
<pre>
~# make -f Makefile
</pre>


#### examples

<pre>
~# zpool list
NAME      SIZE  ALLOC   FREE  EXPANDSZ   FRAG    CAP  DEDUP  HEALTH  ALTROOT
data_01  1.98G  56.5K  1.98G         -     0%     0%  1.00x  ONLINE  -
~# zio -s pools -f json
{
        "data":[

                { "{#FSNAME}":"data_01",                "{#FSTYPE}":"zfs" }

        ]
}
~# zio -s devices -f json -z data_01
{
        "data":[

                { "{#DEVNAME}":"sdb",           "{#DEVTYPE}":"disk" },
                { "{#DEVNAME}":"sdc",           "{#DEVTYPE}":"disk" }

        ]
}
~# zio -s health -z data_01
0
~# zio -s device-state -d sdb
0
~# zio -s all -z data_01
zpool:          data_01
read_ops:       0
write_ops:      107
read_bts:       0
write_bts:      97280
alloc:          57856
free:           2130648576
health:         ONLINE
ddt_memory:     0
dedupratio:     1.00

zfs:            data_01
used:           56320
available:      2097095680
logical:        22016
compressratio:  1.00
</pre>
