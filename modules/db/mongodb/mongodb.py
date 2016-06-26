#!/usr/bin/env python2.7

import sys
import pymongo
import getopt
import json
from bson import json_util

version = 0.1

def usage():
    print "./mongodb.py -d|--database=<dbname>\n\n" \
          "database-parameters:\n" \
          "\t-g|--get-databases \n" \
          "\t-s|--get-stats \n" \
          "\t-a|--get-stats-all \n" \
          "\t-b|--database-ok \n" \
          "\t-t|--storage-size \n" \
          "\t-v|--avgobj-size \n" \
          "\t-i|--indexes \n" \
          "\t-o|--objects \n" \
          "\t-c|--collections \n" \
          "\t-z|--file-size \n" \
          "\t-m|--ns-size \n" \
          "\t-x|--index-size \n" \
          "\t-e|--data-size \n" \
          "\t-n|--num-extents \n" \
          "\nreplica-parameters:\n" \
          "\t-r|--get-replica \n" \
          "\t-k|--replica-ok \n" \
          "\t-w|--members \n" \
          "\t-p|--replset \n" \
          "\t-y|--my-state \n" \
          "\nversion {}".format(version)

# options
class ConfigOptions:
    get_databases       = 0
    get_stats           = 1
    get_replica         = 2
    get_stats_all       = 3
    is_replica_ok       = 4
    is_database_ok      = 5
    get_storage_size    = 6
    get_avg_object_size = 7
    get_indexes         = 8
    get_objects         = 9
    get_collections     = 10
    get_file_size       = 11
    get_num_extents     = 12
    get_data_size       = 13
    get_index_size      = 14
    get_ns_size_mb      = 15
    get_mystate         = 16
    get_replset         = 17
    get_members         = 18

class Config:
# mongo connection parameters
    database = "local"
    connection_string = "mongodb://172.16.134.129:27017,172.16.134.135:27017/test?replicaSet=test"
    option = None

    def cmdline_arg(self, argv):
        try:
            opts, args = getopt.getopt(argv, "hd:gsarkbtviocznexmypw", [ "help", "database=",
                                                         "get-databases",
                                                         "get-stats",
                                                         "get-stats-all",
                                                         "get-replica",
                                                         "replica-ok",
                                                         "database-ok",
                                                         "storage-size",
                                                         "avgobj-size",
                                                         "indexes",
                                                         "objects",
                                                         "collections",
                                                         "file-size",
                                                         "num-extents",
                                                         "data-size",
                                                         "index-size",
                                                         "ns-size",
                                                         "my-state",
                                                         "replset",
                                                         "members"
                                                         ])

        except getopt.GetoptError:
            usage()
            sys.exit(1)

        for opt, arg in opts:
            if opt in ("-h", "--help"):
                usage()
                sys.exit(0)

            elif opt in ("-d", "--database"):
                self.database = arg

            elif opt in ("-g", "--get-databases"):
                self.option = ConfigOptions.get_databases

            elif opt in ("-s", "--get-stats"):
                self.option = ConfigOptions.get_stats

            elif opt in ("-r", "--get-replica"):
                self.option = ConfigOptions.get_replica

            elif opt in ("-a", "--get-stats-all"):
                self.option = ConfigOptions.get_stats_all

            elif opt in ("-k", "--replica-ok"):
                self.option = ConfigOptions.is_replica_ok

            elif opt in ("-b", "--database-ok"):
                self.option = ConfigOptions.is_database_ok

            elif opt in ("-t", "--storage-size"):
                self.option = ConfigOptions.get_storage_size

            elif opt in ("-v", "--avgobj-size"):
                self.option = ConfigOptions.get_avg_object_size

            elif opt in ("-i", "--indexes"):
                self.option = ConfigOptions.get_indexes

            elif opt in ("-o", "--objects"):
                self.option = ConfigOptions.get_objects

            elif opt in ("-c", "--collections"):
                self.option = ConfigOptions.get_collections

            elif opt in ("-z", "--file-size"):
                self.option = ConfigOptions.get_file_size

            elif opt in ("-n", "--num-extents"):
                self.option = ConfigOptions.get_num_extents

            elif opt in ("-e", "--data-size"):
                self.option = ConfigOptions.get_data_size

            elif opt in ("-x", "--index-size"):
                self.option = ConfigOptions.get_index_size

            elif opt in ("-m", "--ns-size"):
                self.option = ConfigOptions.get_ns_size_mb

            elif opt in ("-y", "--my-state"):
                self.option = ConfigOptions.get_mystate

            elif opt in ("-p", "--replset"):
                self.option = ConfigOptions.get_replset

            elif opt in ("-w", "--members"):
                self.option = ConfigOptions.get_members

    def __init__(self, argv):
        self.cmdline_arg(argv)

class Mongo:
    client = None

    def __init__(self, connection_string):
        self.client = pymongo.MongoClient(connection_string)

    def get_databases(self):
        return self.client.database_names()

    def get_stats(self, database):
        db = self.client[database]
        return db.command('dbstats')

    def get_replica_stats(self):
        db = self.client['admin']
        return db.command('replSetGetStatus')

    ## replica stats ##
    def is_replica_ok(self):
        db = self.client['admin']
        replStatus = json.loads(json_util.dumps(db.command('replSetGetStatus')))
        return replStatus['ok']

    def get_myState(self):
        db = self.client['admin']
        myState = json.loads(json_util.dumps(db.command('replSetGetStatus')))
        return myState['myState']

    def get_replSet(self):
        db = self.client['admin']
        replSet = json.loads(json_util.dumps(db.command('replSetGetStatus')))
        return replSet['set']

    def get_members(self):
        db = self.client['admin']
        members = json.loads(json_util.dumps(db.command('replSetGetStatus')))   
        return members['members']

    ## database stats ##
    def is_database_ok(self, database):
        db = self.client[database]
        is_database_ok = json.loads(json_util.dumps(db.command('dbstats')))
        return is_database_ok['ok']

    def get_storageSize(self, database):
        db = self.client[database]
        storageSize = json.loads(json_util.dumps(db.command('dbstats')))
        return storageSize['storageSize']

    def get_avgObjSize(self, database):
        db = self.client[database]
        avgObjSize = json.loads(json_util.dumps(db.command('dbstats')))
        return avgObjSize['avgObjSize']

    def get_indexes(self, database):
        db = self.client[database]
        indexes = json.loads(json_util.dumps(db.command('dbstats')))
        return indexes['indexes']

    def get_objects(self, database):
        db = self.client[database]
        objects = json.loads(json_util.dumps(db.command('dbstats')))
        return objects['objects']

    def get_collections(self, database):
        db = self.client[database]
        collections = json.loads(json_util.dumps(db.command('dbstats')))
        return collections['collections']

    def get_fileSize(self, database):
        db = self.client[database]
        fileSize = json.loads(json_util.dumps(db.command('dbstats')))
        return fileSize['fileSize']

    def get_numExtents(self, database):
        db = self.client[database]
        numExtents = json.loads(json_util.dumps(db.command('dbstats')))
        return numExtents['numExtents']

    def get_dataSize(self, database):
        db = self.client[database]
        dataSize = json.loads(json_util.dumps(db.command('dbstats')))
        return dataSize['dataSize']

    def get_indexSize(self, database):
        db = self.client[database]
        indexSize = json.loads(json_util.dumps(db.command('dbstats')))
        return indexSize['indexSize']

    def get_nsSizeMB(self, database):
        db = self.client[database]
        nsSizeMB = json.loads(json_util.dumps(db.command('dbstats')))
        return nsSizeMB['nsSizeMB']

if __name__ == '__main__':
    c = Config(sys.argv[1:])
    m = Mongo(c.connection_string)

    if (c.option == ConfigOptions.get_databases):
        for database in m.get_databases():
            print database

    elif (c.option == ConfigOptions.get_stats):
        print json_util.dumps(m.get_stats(c.database))

    elif (c.option == ConfigOptions.get_replica):
        print json_util.dumps(m.get_replica_stats())

    elif (c.option == ConfigOptions.get_stats_all):
        for database in m.get_databases():
            print json_util.dumps(m.get_stats(database))

    elif (c.option == ConfigOptions.is_replica_ok):
        print m.is_replica_ok()

    elif (c.option == ConfigOptions.is_database_ok):
        print m.is_database_ok(c.database)

    elif (c.option == ConfigOptions.get_storage_size):
        print m.get_storageSize(c.database)

    elif (c.option == ConfigOptions.get_avg_object_size):
        print m.get_avgObjSize(c.database)

    elif (c.option == ConfigOptions.get_indexes):
        print m.get_indexes(c.database)

    elif (c.option == ConfigOptions.get_objects):
        print m.get_objects(c.database)

    elif (c.option == ConfigOptions.get_collections):
        print m.get_collections(c.database)

    elif (c.option == ConfigOptions.get_file_size):
        print m.get_fileSize(c.database)

    elif (c.option == ConfigOptions.get_num_extents):
        print m.get_numExtents(c.database)

    elif (c.option == ConfigOptions.get_data_size):
        print m.get_dataSize(c.database)

    elif (c.option == ConfigOptions.get_index_size):
        print m.get_indexSize(c.database)

    elif (c.option == ConfigOptions.get_ns_size_mb):
        print m.get_nsSizeMB(c.database)

    elif (c.option == ConfigOptions.get_mystate):
        print m.get_myState()

    elif (c.option == ConfigOptions.get_replset):
        print m.get_replSet()

    elif (c.option == ConfigOptions.get_members):
        members = json.loads(json_util.dumps(m.get_members()))
        for member in members:
            print member['name']