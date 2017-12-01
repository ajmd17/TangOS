#ifndef FAT32_H
#define FAT32_H

#include <stdint.h>
#include <stddef.h>

#include <kernel/mbr.h>

typedef struct {
    unsigned bytes_per_cluster;
    unsigned bytes_per_sector;
    unsigned cluster_count;
    size_t   data_sector;
    unsigned reserved_sectors_count;
    unsigned data_sector_count;
    unsigned fat_sector_count;
    unsigned num_of_fats;
    unsigned root_cluster;
    unsigned root_entry_count;
    unsigned sectors_per_cluster;
    unsigned total_sector_count;
    //no idea what this does, lets just roll with it
    unsigned fatsz32;
} fat_t;

void fat32_startup(enum PARTITION_N part_n);
void fat_init();

#endif