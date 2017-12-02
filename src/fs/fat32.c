#include <fs/fat32.h>
#include <kernel/ata_pio.h>
#include <kernel/byte_converter.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER 10000
#define EOF_FAT32  0xFFFFFF8
#define END_MARK 0x0FFFFFF8

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
    read(READ, 0, bpb, 512, 1);

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

    fat.dirs_per_sector = fat.sectors_per_cluster*fat.bytes_per_sector/sizeof(dir_entry_t);

    free(bpb);
}

void set_cluster_hi_lo(dir_entry_t *entry, size_t n) {
    entry->fat_cluster_lo = n & 0x0000FFFF;
    entry->fat_cluster_hi = n >> 16;
}

size_t get_dir_cluster_num(dir_entry_t *entry) {
    return (entry->fat_cluster_hi << 16) + entry->fat_cluster_lo;
}

unsigned find_char_in_str(char *str, char c) {
    int len = strlen(str);
    int i;
    for (i = 0; i < len; i++) {
        if (str[i] == c) {
            return i;
        }
    }
    return len;
}

void split_name_ext(char *name, char *ext, char *base_name) {
    int i = 0, j = 0;
    int len = strlen(base_name);
    while (base_name[i] != '.') {
        name[j++] = base_name[i++];
    }
    j = 0;
    i++;
    while (base_name[i] != '\0') {
        ext[j++] = base_name[i++];
    }
}

void locate_file_in_dir(char *base_name, dir_entry_t *cwd, dir_entry_t *value, size_t *value_addr) {
    char name[9];
    char ext[4];
    char *dot;
    int n = fat.dirs_per_sector;
    int i, j;
    size_t cwd_cluster;
    size_t base_sector;
    dir_entry_t *entry;

    uint8_t *buffer = (uint8_t *)malloc(MAX_BUFFER);    

    *value_addr = 0;

    memset(name, ' ', sizeof(name));
    name[9] = '\0';
    memset(ext,  ' ', sizeof(ext));
    ext[4]  = '\0';

    cwd_cluster = get_dir_cluster_num(cwd);

    split_name_ext(name, ext, base_name);

    do {
        base_sector = fat.data_sector_count+fat.sectors_per_cluster*(cwd_cluster-2);
        printf("cluster: %d\n", cwd_cluster);
        read(READ_SECTOR, base_sector, buffer, 512, fat.sectors_per_cluster);
        for (j = 0; j < n; j++) {
            
        }
    } while (cwd_cluster < END_MARK);
    free(buffer);
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
        ata_pio_rw(READ_SECTOR, root_dir_sector_num, i*512, buffer, NULL, fat.sectors_per_cluster);
    }
    printf("root dir sec n: %d\n", root_dir_sector_num);

    for (i = 0; i < 10; i++) {
        memcpy(&dir_entry, buffer+i*sizeof(dir_entry_t), sizeof(dir_entry_t));
        printf("%d: %s(first char = %x)\n", dir_entry.dir_name, dir_entry.dir_name[0]);
        if (!strncmp((char *)dir_entry.dir_name+1, "B      TXT", 10)) {
            printf("the file 'ab.txt' is found\n");
            printf("first cluster: %d\n", dir_entry.fat_cluster_lo);
            printf("file size: %d\n", dir_entry.file_size);

            //read fat on disk into temp fat
            read(READ_SECTOR, fat_sector_num[0], (unsigned char *)temp_fat, fat.fatsz32*512, fat.fatsz32);
            
            if (temp_fat[dir_entry.fat_cluster_lo] == 0) {
                printf("file is deleted.\n");
            }
        }
    }
}