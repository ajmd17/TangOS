#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>
#include <stddef.h>

#include <kernel/mbr.h>

typedef struct {
    unsigned bytes_per_cluster;
    unsigned cluster_count;
    size_t   data_sector;
    unsigned reserved_sectors_count;
    unsigned data_sector_count;
    unsigned fat_sector_count;
    unsigned num_of_fats;
    unsigned root_cluster;
    unsigned root_entry_count;
    unsigned bytes_per_sector;
    unsigned sectors_per_cluster;
    int      dirs_per_sector;
    size_t   root_dir_cluster_num;
    unsigned total_sector_count;
    //no idea what this does, lets just roll with it
    unsigned fatsz32;
} fat_t;

void fat32_startup(PARTITION part_n);
void new_partition(unsigned n_sectors);
void print_data(void);

#endif