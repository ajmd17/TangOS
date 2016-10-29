#ifndef EXT2_H
#define EXT2_H

#include <util/tos_types.h>
#include <fs/fs.h>

#define EXT2_SUPER_MAGIC 0xEF53

#define EXT2_DIRECT_BLOCKS 12

/* Super block struct. */
struct ext2_superblock {
  uint inodes_count;
  uint blocks_count;
  uint r_blocks_count;
  uint free_blocks_count;
  uint free_inodes_count;
  uint first_data_block;
  uint log_block_size;
  uint log_frag_size;
  uint blocks_per_group;
  uint frags_per_group;
  uint inodes_per_group;
  uint mtime;
  uint wtime;

  ushort mnt_count;
  ushort max_mnt_count;
  ushort magic;
  ushort state;
  ushort errors;
  ushort minor_rev_level;

  uint lastcheck;
  uint checkinterval;
  uint creator_os;
  uint rev_level;

  ushort def_resuid;
  ushort def_resgid;

  /* EXT2_DYNAMIC_REV */
  uint first_ino;
  ushort inode_size;
  ushort block_group_nr;
  uint feature_compat;
  uint feature_incompat;
  uint feature_ro_compat;

  uchar uuid[16];
  uchar volume_name[16];

  uchar last_mounted[64];

  uint algo_bitmap;

  /* Performance Hints */
  uchar prealloc_blocks;
  uchar prealloc_dir_blocks;
  ushort _padding;

  /* Journaling Support */
  uchar journal_uuid[16];
  uint journal_inum;
  uint jounral_dev;
  uint last_orphan;

  /* Directory Indexing Support */
  uint hash_seed[4];
  uchar def_hash_version;
  ushort _padding_a;
  uchar _padding_b;

  /* Other Options */
  uint default_mount_options;
  uint first_meta_bg;
  uchar _unused[760];

} __attribute__ ((packed));

typedef struct ext2_superblock ext2_superblock_t;

/* Block group descriptor. */
struct ext2_bgdescriptor {
  uint block_bitmap;
  uint inode_bitmap;    // block no. of inode bitmap
  uint inode_table;
  ushort free_blocks_count;
  ushort free_inodes_count;
  ushort used_dirs_count;
  ushort pad;
  uchar reserved[12];
} __attribute__ ((packed));

typedef struct ext2_bgdescriptor ext2_bgdescriptor_t;

/* File Types */
#define EXT2_S_IFSOCK 0xC000
#define EXT2_S_IFLNK  0xA000
#define EXT2_S_IFREG  0x8000
#define EXT2_S_IFBLK  0x6000
#define EXT2_S_IFDIR  0x4000
#define EXT2_S_IFCHR  0x2000
#define EXT2_S_IFIFO  0x1000

/* setuid, etc. */
#define EXT2_S_ISUID  0x0800
#define EXT2_S_ISGID  0x0400
#define EXT2_S_ISVTX  0x0200

/* rights */
#define EXT2_S_IRUSR  0x0100
#define EXT2_S_IWUSR  0x0080
#define EXT2_S_IXUSR  0x0040
#define EXT2_S_IRGRP  0x0020
#define EXT2_S_IWGRP  0x0010
#define EXT2_S_IXGRP  0x0008
#define EXT2_S_IROTH  0x0004
#define EXT2_S_IWOTH  0x0002
#define EXT2_S_IXOTH  0x0001

/* This is not actually the inode table.
 * It represents an inode in an inode table on disk. */
struct ext2_inodetable {
  ushort mode;
  ushort uid;
  uint size;      // file length in byte.
  uint atime;
  uint ctime;
  uint mtime;
  uint dtime;
  ushort gid;
  ushort links_count;
  uint blocks;
  uint flags;
  uint osd1;
  uint block[15];
  uint generation;
  uint file_acl;
  uint dir_acl;
  uint faddr;
  uchar osd2[12];
} __attribute__ ((packed));

typedef struct ext2_inodetable ext2_inodetable_t;

/* Represents directory entry on disk. */
struct ext2_dir {
  uint inode;
  ushort rec_len;
  uchar name_len;
  uchar file_type;
  char name[];    /* Actually a set of characters, at most 255 bytes */
} __attribute__ ((packed));

typedef struct ext2_dir ext2_dir_t;

typedef struct {
  uint block_no;
  uint last_use;
  uchar  dirty;
  uchar *block;
} ext2_disk_cache_entry_t;

typedef int (*ext2_block_io_t) (void *, uint, uchar *);

fs_node_t * ext2_fs_mount(char * device, char * mount_path);

#endif