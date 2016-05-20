#!/usr/bin/env bash
# zabbix zpool's monitoring 

zio=$(which zio)

if [[ -n "$1" ]]; then
                if [[ "$1" == "devices" ]]; then
                        $zio -s devices -f json 2>/dev/null
                        exit 0
                fi

                if [[ "$1" == "device-state" ]]; then
                        $zio -s device-state -d $2 2>/dev/null
                        exit 0
                fi

                if [[ "$1" == "arccache" ]]; then
                        kstat zfs:0:arcstats:size | grep size | awk '{printf $2"\n"}'
                        exit 0
                fi

                zpool list -Ho name $1 > /dev/null 2>&1
                if [[ `echo $?` == "1" ]]; then
                        echo "pool: '$1' not exists"
                        exit 1
                fi
                zpool="$1"      # set zpool
else
        $zio -s pools -f json 2>/dev/null
        exit 0
fi


dedup() {
        used=$($zio -s used -z $zpool 2>/dev/null)
        dedupratio=$($zio -s dedupratio -z $zpool 2>/dev/null)
        dedup=$(echo $used / $dedupratio | bc)
        echo $used - $dedup | bc
}

total() {
        dedup=$(dedup)
        compress=$($zio -s compress -z $zpool 2>/dev/null)
        echo $compress + $dedup | bc
}


if [[ -n "$2" ]]; then
        if      [[ "$2" == "ioread"     ]]; then $zio -s read_ops -z $zpool 2>/dev/null
        elif    [[ "$2" == "iowrite"    ]]; then $zio -s write_ops -z $zpool 2>/dev/null
        elif    [[ "$2" == "bsread"     ]]; then $zio -s read_bts -z $zpool 2>/dev/null
        elif    [[ "$2" == "bswrite"    ]]; then $zio -s write_bts -z $zpool 2>/dev/null
        elif    [[ "$2" == "free"       ]]; then $zio -s available -z $zpool 2>/dev/null
        elif    [[ "$2" == "real"       ]]; then $zio -s real -z $zpool 2>/dev/null
        elif    [[ "$2" == "compress"   ]]; then $zio -s compress -z $zpool 2>/dev/null
        elif    [[ "$2" == "logical"    ]]; then $zio -s logical -z $zpool 2>/dev/null
        elif    [[ "$2" == "total"      ]]; then total
        elif    [[ "$2" == "dedup"      ]]; then dedup
        elif    [[ "$2" == "health"     ]]; then $zio -s health -z $zpool 2>/dev/null
        elif    [[ "$2" == "ddt"        ]]; then $zio -s ddt-memory -z $zpool 2>/dev/null
        fi
fi
