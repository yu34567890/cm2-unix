#!/bin/env python3

import struct

SECTOR_SIZE = 256
DATA_START = 512
FS_INAME_LEN = 10

FILE_ENTRY_STRUCT = struct.Struct(f"{FS_INAME_LEN}s B B H H")
FILE_ENTRY_SIZE = FILE_ENTRY_STRUCT.size
DIR_ENTRIES_PER_CLUSTER = SECTOR_SIZE // FILE_ENTRY_SIZE


def read_directory_cluster(f, cluster_index, depth=0):
    indent = "  " * depth

    # Calculate cluster offset
    offset = DATA_START + cluster_index * SECTOR_SIZE

    # Save current position
    saved_pos = f.tell()

    f.seek(offset)
    print(f"{indent}Directory at cluster {cluster_index} (offset {offset}):")

    for i in range(DIR_ENTRIES_PER_CLUSTER):
        raw = f.read(FILE_ENTRY_SIZE)
        if raw == b"\x00" * FILE_ENTRY_SIZE:
            continue  # empty entry

        name, fat_index, mode, size, reserved = FILE_ENTRY_STRUCT.unpack(raw)
        name = name.rstrip(b"\x00").decode(errors="ignore")

        print(f"{indent}Entry {i}:")
        print(f"{indent}  Name      : {name}")
        print(f"{indent}  FAT index : {fat_index}")
        print(f"{indent}  Mode      : {mode} ({'DIR' if (mode & 2)==0 else 'FILE'})")
        print(f"{indent}  Size      : {size}")
        print()

        # If directory, recurse
        if (mode & 2) == 0:
            read_directory_cluster(f, fat_index, depth + 1)

    # Restore file position
    f.seek(saved_pos)


with open(input("file> "), "rb") as f:
    read_directory_cluster(f, 0)






