#!/usr/bin/env python2.7

import sys
import pymongo
import ConfigParser
import getopt
import json

def usage():
    print "./mongodb.py -c|--config=<file> -d|--database=<dbname>"

class Config:
    config_file = "conf/mongodb.conf"

# mongo connection parameters
    database = "local"
    connection_string = "mongodb://127.0.0.1:27017"
    replica = False

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
                self.config_file = arg

            elif opt in ("-d", "--database"):
                self.database = arg

    def config_parser(self, config_file):
        settings = ConfigParser.ConfigParser()
        settings.read(config_file)
        self.connection_string = settings.get('global', 'connection_string')
        self.replica = settings.getboolean('global', 'replica')

    def __init__(self, argv):
        self.cmdline_arg(argv)
        self.config_parser(self.config_file)

class Mongo:
    client = None

    def __init__(self, connection_string, replica):
        if replica == True:
            pymongo.MongoReplicaSetClient

        self.client = pymongo.MongoClient(connection_string)

    def get_databases(self):
        for i in self.client.database_names():
            print i

    def get_stats(self, database):
        db = self.client[database]
        print db.command('dbstats')

if __name__ == '__main__':
    c = Config(sys.argv[1:])
    m = Mongo(c.connection_string,
              c.replica)

    m.get_databases()
    m.get_stats("local")