#include <fs/fat32.h>
#include <kernel/ata_pio.h>
#include <kernel/byte_converter.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER 10000
#define EOF_FAT32  0xffffff8

static mbr_t *mbr;
static fat_t fat;

typedef struct {
    char dir_name[11];
    char dir_attr;
    /*-------------------------*/
    uint8_t  creation_time_tenth;
    uint16_t creation_time;
    uint16_t creation_date;
    /*-------------------------*/
    uint16_t last_access_date;
    /*-------------------------*/
    uint16_t fat_cluster_hi;
    uint16_t fat_cluster_lo;
    /*-------------------------*/
    uint16_t write_time;
    uint16_t write_date;
    /*-------------------------*/
    size_t file_size;
} dir_entry_t;

void fat32_startup(enum PARTITION_N part_n) {
    //loading bios parameter block
    set_def_partition(part_n);

    uint8_t *bpb = (uint8_t *)malloc(512);
    read(READ, 0, bpb, 512);

    if (bpb[510] != 0x55 || bpb[511] != 0xAA) {
        //non valid filesystem
        return;
    }

    //valid fat filesystem

    fat.bytes_per_sector = to_uint16(bpb, 11);
    fat.sectors_per_cluster = bpb[13];
    fat.bytes_per_cluster = fat.bytes_per_sector*fat.sectors_per_cluster;
    fat.reserved_sectors_count = to_uint16(bpb, 14);
    fat.num_of_fats = bpb[16];
    fat.root_entry_count = to_uint16(bpb, 17);
    fat.total_sector_count = to_uint16(bpb, 19);

    if (fat.total_sector_count == 0) {
        fat.total_sector_count = to_uint32(bpb, 32);
    }

    //get sector count(FAT32 only)
    fat.fat_sector_count = to_uint32(bpb, 36);
    fat.data_sector_count = fat.total_sector_count - (
                            fat.reserved_sectors_count+
                            fat.num_of_fats
                            *
                            fat.fat_sector_count);
    fat.cluster_count = fat.data_sector_count/fat.sectors_per_cluster;
    fat.fatsz32 = bpb[0x24+3];

    fat.root_cluster = to_uint32(bpb, 44);
    fat.data_sector = fat.reserved_sectors_count+fat.num_of_fats*fat.fat_sector_count;

    free(bpb);
}

void fat_init() {
    size_t *temp_fat;
    mbr = get_mbr();
    dir_entry_t dir_entry;
    uint8_t *buffer = (uint8_t *)malloc(MAX_BUFFER);
    int data_sector_num = 0;
    size_t root_dir_sector_num = 0;
    int fat_sector_num[2];
    int i;

    fat_sector_num[0] = fat.reserved_sectors_count;
    fat_sector_num[1] = fat.reserved_sectors_count+fat.fatsz32;

    temp_fat = (size_t *)malloc(sizeof(size_t)*fat.fatsz32*fat.bytes_per_sector/4);
    data_sector_num = fat.reserved_sectors_count+2*fat.fatsz32;
    memset(buffer, 0, MAX_BUFFER);
    root_dir_sector_num = data_sector_num+(fat.root_cluster-2)*fat.sectors_per_cluster;
    for (i = 0; i < fat.sectors_per_cluster; i++) {
        ata_pio_rw(READ_SECTOR, root_dir_sector_num*512, i*512, buffer, NULL, fat.sectors_per_cluster);
    }
    printf("root dir sec n: %d\n", -root_dir_sector_num);
}