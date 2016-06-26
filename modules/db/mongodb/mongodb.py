#!/usr/bin/env python2.7

import sys
import pymongo
import getopt
import json
from bson import json_util

version = 0.1

def usage():
    print "mongodb.py\n\n" \
          "database-parameters:\n" \
          "\t-g|--get-databases\t\t- show databases\n" \
          "\t-s|--get-stats <database>\t- show database stats\n" \
          "\t-a|--get-stats-all\t\t- show all databases stats\n" \
          "\t-b|--database-ok <database>\t- show state of database\n" \
          "\t-t|--storage-size <database>\t- show used space\n" \
          "\t-v|--avgobj-size <database>\t- show average object size\n" \
          "\t-i|--indexes <database>\t\t- show count of indexes\n" \
          "\t-o|--objects <database>\t\t- show count of objects\n" \
          "\t-c|--collections <database>\t- show count of collections\n" \
          "\t-z|--file-size <database>\t-\n" \
          "\t-m|--ns-size <database>\t\t-\n" \
          "\t-x|--index-size <database>\t-\n" \
          "\t-e|--data-size <database>\t-\n" \
          "\t-n|--num-extents <database>\t-\n" \
          "\nreplica-parameters:\n" \
          "\t-r|--get-replica\t\t- show replica informations\n" \
          "\t-k|--replica-ok\t\t\t- show replica state\n" \
          "\t-w|--members\t\t\t- show replica members\n" \
          "\t-p|--replset\t\t\t- show replica name\n" \
          "\t-y|--my-state\t\t\t-\n" \
          "\t-H|--health <replSet_member>\t- show health state of replica member\n" \
          "\t-T|--stateStr <replSet_member>\t- show state str of replica member\n" \
          "\t-P|--ping <replSet_member>\t- show ping in miliseconds\n" \
          "\t-B|--lastHeartbeat <replSet_member> - show last heartbeat\n" \
          "\nmongodb.py version {}".format(version)

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
    get_health          = 19
    get_state_str       = 20
    get_ping_ms         = 21
    get_lastHeartbeat   = 22

class Config:
# mongo connection parameters
    database = "local"
    replSet_member = None
    connection_string = "mongodb://172.16.134.129:27017,172.16.134.135:27017/test?replicaSet=test"
    option = None

    def cmdline_arg(self, argv):
        try:
            opts, args = getopt.getopt(argv, "hgs:arkb:t:v:i:o:c:z:n:e:x:m:ypwH:T:P:B:", [
                                                         "help",
                                                         "get-databases",
                                                         "get-stats=",
                                                         "get-stats-all",
                                                         "get-replica",
                                                         "replica-ok",
                                                         "database-ok=",
                                                         "storage-size=",
                                                         "avgobj-size=",
                                                         "indexes=",
                                                         "objects=",
                                                         "collections=",
                                                         "file-size=",
                                                         "num-extents=",
                                                         "data-size=",
                                                         "index-size=",
                                                         "ns-size=",
                                                         "my-state",
                                                         "replset",
                                                         "members",
                                                         "health=",
                                                         "stateStr=",
                                                         "ping=",
                                                         "lastHeartbeat="
                                                         ])

        except getopt.GetoptError:
            usage()
            sys.exit(1)

        for opt, arg in opts:
            if opt in ("-h", "--help"):
                usage()
                sys.exit(0)

            elif opt in ("-g", "--get-databases"):
                self.option = ConfigOptions.get_databases

            elif opt in ("-s", "--get-stats"):
                self.option = ConfigOptions.get_stats
                self.database = arg

            elif opt in ("-r", "--get-replica"):
                self.option = ConfigOptions.get_replica

            elif opt in ("-a", "--get-stats-all"):
                self.option = ConfigOptions.get_stats_all

            elif opt in ("-k", "--replica-ok"):
                self.option = ConfigOptions.is_replica_ok

            elif opt in ("-b", "--database-ok"):
                self.option = ConfigOptions.is_database_ok
                self.database = arg

            elif opt in ("-t", "--storage-size"):
                self.option = ConfigOptions.get_storage_size
                self.database = arg

            elif opt in ("-v", "--avgobj-size"):
                self.option = ConfigOptions.get_avg_object_size
                self.database = arg

            elif opt in ("-i", "--indexes"):
                self.option = ConfigOptions.get_indexes
                self.database = arg

            elif opt in ("-o", "--objects"):
                self.option = ConfigOptions.get_objects
                self.database = arg

            elif opt in ("-c", "--collections"):
                self.option = ConfigOptions.get_collections
                self.database = arg

            elif opt in ("-z", "--file-size"):
                self.option = ConfigOptions.get_file_size
                self.database = arg

            elif opt in ("-n", "--num-extents"):
                self.option = ConfigOptions.get_num_extents
                self.database = arg

            elif opt in ("-e", "--data-size"):
                self.option = ConfigOptions.get_data_size
                self.database = arg

            elif opt in ("-x", "--index-size"):
                self.option = ConfigOptions.get_index_size
                self.database = arg

            elif opt in ("-m", "--ns-size"):
                self.option = ConfigOptions.get_ns_size_mb
                self.database = arg

            elif opt in ("-y", "--my-state"):
                self.option = ConfigOptions.get_mystate

            elif opt in ("-p", "--replset"):
                self.option = ConfigOptions.get_replset

            elif opt in ("-w", "--members"):
                self.option = ConfigOptions.get_members

            elif opt in ("-H", "--health"):
                self.option = ConfigOptions.get_health
                self.replSet_member = arg

            elif opt in ("-T", "--stateStr"):
                self.option = ConfigOptions.get_state_str
                self.replSet_member = arg

            elif opt in ("-P", "--ping"):
                self.option = ConfigOptions.get_ping_ms
                self.replSet_member = arg

            elif opt in ("-B", "--lastHeartbeat"):
                self.option = ConfigOptions.get_lastHeartbeat
                self.replSet_member = arg

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

    def get_health(self, replSet_member):
        members = self.get_members()
        for member in members:
            if member['name'] == replSet_member:
                return member['health']

    def get_state_str(self, replSet_member):
        members = self.get_members()
        for member in members:
            if member['name'] == replSet_member:
                return member['stateStr']

    def get_ping_ms(self, replSet_member):
        members = self.get_members()
        for member in members:
            if member['name'] == replSet_member and member['stateStr'] != 'PRIMARY':
                return member['pingMs']

    def get_lastHeartbeat(self, replSet_member):
        members = self.get_members()
        for member in members:
            if member['name'] == replSet_member and member['stateStr'] != 'PRIMARY':
                return json.loads(json_util.dumps(member['lastHeartbeat']))['$date']

    def get_members(self):
        db = self.client['admin']
        members = json.loads(json_util.dumps(db.command('replSetGetStatus')))
        return json.loads(json_util.dumps(members['members']))

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
        members = m.get_members()
        for member in members:
            print member['name']

    elif (c.option == ConfigOptions.get_health):
        print m.get_health(c.replSet_member)

    elif (c.option == ConfigOptions.get_state_str):
        print m.get_state_str(c.replSet_member)

    elif (c.option == ConfigOptions.get_ping_ms):
        print m.get_ping_ms(c.replSet_member)

    elif (c.option == ConfigOptions.get_lastHeartbeat):
        print m.get_lastHeartbeat(c.replSet_member)