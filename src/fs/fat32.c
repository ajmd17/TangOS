#include <fs/fat32.h>
#include <kernel/ata_pio.h>
#include <kernel/mbr.h>
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

void fat32_startup(PARTITION part_n) {
    //loading bios parameter block

    // write(WRITE, 16, 2, 1, 1);

    uint8_t *bpb = (uint8_t *)malloc(512);

    mbr_read(part_n, 0, 1, bpb);
    // read(READ, 0, bpb, 512, 1);

    uint8_t amt_fats = 1;

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

void new_partition(unsigned n_sectors) {
    int i;
    partition_t *last_part;

    for (i = 0; i < 4; i++) {
        if (mbr->partitions[i].error != NULL)
            last_part = &(mbr->partitions[i]);
        else
            break;
    }

    i--;

    partition_t new_part;
    new_part.lba_first_sector = last_part->lba_end_sector;
    new_part.sector_count = n_sectors;
    new_part.lba_end_sector = new_part.lba_first_sector + new_part.sector_count;

    //get partition entry from last usable partition
    int part_index = PARTITION_ENTRY_1+((i)*10);

    //write sys_id for FAT32(LBA addressing) for loading later
    uint8_t sys_id = 0x0C;
    ata_pio_write(part_index+4, &sys_id, 1, 1);

    //split ints into four bytes each and write bytes to disk
    uint8_t *split = (uint8_t *)&new_part.lba_first_sector;
    ata_pio_write_seq(part_index+8, split, 4);
    split = (uint8_t *)&new_part.sector_count;
    ata_pio_write_seq(part_index+12, split, 4);

    printf("Created partition %d\n", i);
}


