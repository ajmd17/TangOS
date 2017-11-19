#include <kernel/sys_asm.h>
#include <kernel/terminal.h>
#include <stdio.h>
#include <kernel/ata_pio.h>
#include <kernel/mbr.h>
#include <stdlib.h>

#include <_math.h>

static mbr_t mbr;

void mbr_init() {
    mbr.bootstrap = (uint8_t *)malloc(sizeof(uint8_t)*446);
    int i;
    for (i = 0; i < 4; i++) {
        mbr.partitions[i].error  = 0;
    }
}

unsigned int to_uint32(uint8_t *n, unsigned pos) {
    return (unsigned)n[pos+3] << 24 | (unsigned)n[pos+2] << 16 |
           (unsigned)n[pos+1] << 8  | n[pos];
}

unsigned char parse_partition(partition_t *part, uint8_t *mbr_dat, size_t loc) {
    char sys_id = mbr_dat[loc+4];
    if (sys_id == 0) {
        part->error = 1;
        return 1;
    }
    if (sys_id == 0x5 || sys_id == 0xF || sys_id == 0x85) {
        part->error = 1;
        return 1; //EBR partition(extended boot record) is not supported
    }
    else {
        part->lba_first_sector = to_uint32(mbr_dat, loc+8);
        part->sector_count = to_uint32(mbr_dat, loc+12);
        part->bootable = mbr_dat[loc] == 0x81;
        return 0;
    }
}

void read_partitions_into_memory() {
    uint8_t *mbr_dat = read(0, 1);

    mbr.signature[0] = mbr_dat[510];
    mbr.signature[1] = mbr_dat[511];

    if (mbr.signature[0] != 0x55 || mbr.signature[1] != 0xAA) {
        printf("This is not a valid MBR drive!\n");
        PAUSE;
    }

    int i;
    for (i = 0; i < 446; i++) mbr.bootstrap[i] = mbr_dat[i];
    
    //try and read partitions
    //first partition
    parse_partition(&mbr.partitions[0], mbr_dat, __PARTITION_ENTRY_1);
    //second partition
    parse_partition(&mbr.partitions[1], mbr_dat, __PARTITION_ENTRY_2);
    //third partition
    parse_partition(&mbr.partitions[2], mbr_dat, __PARTITION_ENTRY_3);
    //fourth partition
    parse_partition(&mbr.partitions[3], mbr_dat, __PARTITION_ENTRY_4);

    free(mbr_dat);
}

void write_to_partition(enum __PARTITION_N part_n, size_t rel_lba, uint8_t *data, int data_len) {
    partition_t *part;
    float sector_amt = data_len/512;

    ROUND_UP(sector_amt);

    printf("sec_amt = %d\n", data[0]);
    part = &mbr.partitions[part_n];
    write(part->lba_first_sector+rel_lba, 
          data, data_len, 1);
}

uint8_t *read_from_partition(enum __PARTITION_N part_n, size_t rel_lba, unsigned sec_amt) {
    partition_t *part;
    part = &mbr.partitions[part_n];
    return read(part->lba_first_sector+rel_lba, sec_amt);
}

mbr_t *get_mbr() {
    return &mbr;
}

void mbr_destroy() {
    free(mbr.bootstrap);
}