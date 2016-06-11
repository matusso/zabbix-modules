#!/usr/bin/env python2.7

import sys
import pymongo
import ConfigParser
import getopt

def usage():
    print "./mongodb.py -c|--config=<file> -d|--database=<dbname>"

class Config:
    config_file = "conf/mongodb.conf"

    def cmdline_arg(self, argv):
        try:
            opts, args = getopt.getopt(argv, "hc:d:", ["help", "config=", "database="])
        except getopt.GetoptError:
            usage()
            sys.exit(1)

        for opt, arg in opts:
            if opt in ("-h", "--help"):
                usage()
                sys.exit(0)

            elif opt in ("-c", "--config"):
                config = arg

            elif opt in ("-d", "--database"):
                database = arg

    def config_parser(file, ):
        print "config"

def init_mongo():
    client = pymongo.MongoClient("mongodb://127.0.0.1:27017") # have to define ConnectionString
    # replicaClient = pymongo.MongoReplicaSetClient
    print client.address
    db = client['local'] # have to define database
    print db.command('dbstats')

if __name__ == '__main__':
    c = Config()
    c.cmdline_arg(sys.argv[1:])


    init_mongo()
