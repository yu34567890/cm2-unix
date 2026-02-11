#!/usr/bin/env python3
import struct

# ---------------- CONSTANTS ----------------

FAT_SIZE = 256          # number of FAT entries
CLUSTER_SIZE = 256      # bytes per cluster
DATA_START = 512        # offset where cluster 0 begins

FAT_FREE = 0xFE
FAT_END  = 0xFF

FS_INAME_LEN = 10
FS_MODE_DIR  = 0
FS_MODE_FILE = 2

FILE_ENTRY = struct.Struct(f"{FS_INAME_LEN}s B B H H")
ENTRY_SIZE = FILE_ENTRY.size
ENTRIES_PER_DIR = CLUSTER_SIZE // ENTRY_SIZE


# ---------------- SIMPLE FAT8 IMAGE ----------------

# FAT table: 256 bytes
fat = bytearray([FAT_FREE] * FAT_SIZE)
fat[0] = FAT_END   # cluster 0 = root directory

# Data region: 256 clusters * 256 bytes = 65536 bytes
data = bytearray([0] * (16 * CLUSTER_SIZE))

# Root directory entries (cluster 0)
root_dir = [None] * ENTRIES_PER_DIR


# --------- Helper: allocate a cluster ---------

def alloc_cluster():
    for i in range(1, FAT_SIZE):
        if fat[i] == FAT_FREE:
            fat[i] = FAT_END
            return i
    raise RuntimeError("No free clusters")


# --------- Helper: add entry to root directory ---------

def add_root_entry(name, first_cluster, mode, size):
    name_bytes = name.encode().ljust(FS_INAME_LEN, b'\x00')
    entry = FILE_ENTRY.pack(name_bytes, first_cluster, mode, size, 0)

    for i in range(ENTRIES_PER_DIR):
        if root_dir[i] is None:
            root_dir[i] = entry
            # write into cluster 0
            offset = i * ENTRY_SIZE
            data[offset:offset + ENTRY_SIZE] = entry
            return

    raise RuntimeError("Root directory full")


# --------- Create a file ---------

def write_file(name, content):
    # round size up to cluster size
    padded_size = ((len(content) + CLUSTER_SIZE - 1) // CLUSTER_SIZE) * CLUSTER_SIZE

    first = alloc_cluster()
    cluster = first

    offset = 0
    while offset < padded_size:
        start = cluster * CLUSTER_SIZE
        chunk = content[offset:offset + CLUSTER_SIZE]
        data[start:start + len(chunk)] = chunk

        if len(chunk) < CLUSTER_SIZE:
            data[start + len(chunk):start + CLUSTER_SIZE] = b"\x00" * (CLUSTER_SIZE - len(chunk))

        offset += CLUSTER_SIZE

        if offset < padded_size:
            next_cluster = alloc_cluster()
            fat[cluster] = next_cluster
            cluster = next_cluster

    add_root_entry(name, first, FS_MODE_FILE, padded_size)


# --------- Create a directory ---------

def make_dir(name):
    cluster = alloc_cluster()
    # cluster is already zeroed
    add_root_entry(name, cluster, FS_MODE_DIR, CLUSTER_SIZE)


# --------- Write image to disk ---------

def save_image(filename):
    with open(filename, "wb") as f:
        f.seek(0)
        f.write(fat)

        f.seek(DATA_START)
        f.write(data)


# ---------------- Example ----------------

write_file("HELLO.TXT", b"Hello FAT8!")
write_file("README", b"This is a test file.")
make_dir("dev")

save_image("fat8.img")
print("Created fat8.img")

