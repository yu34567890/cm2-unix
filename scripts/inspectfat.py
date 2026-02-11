#!/bin/env python3

import struct

SECTOR_SIZE = 256
DATA_START = 512
FS_INAME_LEN = 10

FILE_ENTRY_STRUCT = struct.Struct(f"{FS_INAME_LEN}s B B H H")
FILE_ENTRY_SIZE = FILE_ENTRY_STRUCT.size
DIR_ENTRIES_PER_CLUSTER = SECTOR_SIZE // FILE_ENTRY_SIZE

def read_directory_cluster(filename, cluster_index):
    with open(filename, "rb") as f:
        # Calculate cluster offset
        offset = DATA_START + cluster_index * SECTOR_SIZE
        f.seek(offset)

        print(f"Directory at cluster {cluster_index} (offset {offset}):\n")

        for i in range(DIR_ENTRIES_PER_CLUSTER):
            raw = f.read(FILE_ENTRY_SIZE)
            if raw == b"\x00" * FILE_ENTRY_SIZE:
                continue  # empty entry

            name, fat_index, mode, size, reserved = FILE_ENTRY_STRUCT.unpack(raw)
            name = name.rstrip(b"\x00").decode(errors="ignore")

            print(f"Entry {i}:")
            print(f"  Name      : {name}")
            print(f"  FAT index : {fat_index}")
            print(f"  Mode      : {mode} ({'DIR' if (mode & 2)==0 else 'FILE'})")
            print(f"  Size      : {size}")
            print()










read_directory_cluster(input("name> "), int(input("cluster> ")))






