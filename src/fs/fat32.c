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

    // write(WRITE, 16, 2, 1, 1);

    uint8_t *bpb = (uint8_t *)malloc(512);
    read(READ, 0, bpb, 512, 1);

    if (bpb[510] != 0x55 || bpb[511] != 0xAA) {
        //non valid filesystem
        return;
    }

    //valid fat filesystem

    printf("num of fats: %d\n", bpb[16]);

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

void trim_spaces(char *c) {
    int len = strlen(c);
    int i = 0;
    while (*c != ' ' && i++ < len) {
        c++;
    }
    if (*c == ' ') *c = '\0';
}

void assign_value(uint8_t *n_bpb, uint8_t *value, int offset, int len) {
    int i;
    for (i = 0; i < len; i++) {
        value[i] = n_bpb[i+offset];
    }
}
