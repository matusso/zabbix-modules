#!/bin/bash
# dm-raid status monitor

iter=0;
raids=($(dmraid -s | awk -F':' '/name/ {gsub(/^[ \t]+/, "", $NF); print $NF}')) # array of raids

display_dmraid_json() {
        echo -e "{\n\t\"data\":["
        while [ $iter -ne ${#raids[@]} ]; do
                iter=$[$iter+1]

                if [ $(($iter)) -ne ${#raids[@]} ]; then echo -e "\t\t{ \"{#FSNAME}\":\"${raids[$(($iter-1))]}\",\t\"{#FSTYPE}\":\"dm-raid\"},"
                else echo -e "\t\t{ \"{#FSNAME}\":\"${raids[$(($iter-1))]}\",\t\"{#FSTYPE}\":\"dm-raid\"}"; fi

        done
        echo -e "\t]\n}"
}


display_dmraid_status() {
        status=$(dmraid -s | grep "$1" -A5 | awk -F':' '/status/ {gsub(/^[ \t]+/, "", $NF); print $2}')

        if [ "$status" == "ok" ]; then echo 0
        else echo 1; fi
}

if [ $# -eq 0 ]; then display_dmraid_json
elif [ $# -eq 1 ]; then
        dmraid=$(dmraid -s | awk -F':' "/^name   : $1$/ {print \$NF}")
        if [ -z "$dmraid" ]; then echo "$1 doesn't exists"
        else display_dmraid_status $dmraid; fi
else echo -e "I don't know what do you want!"
fi
