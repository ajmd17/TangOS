//TODO: heavy cleanup of code and get rid of unused functions.

#include <kernel/sys_asm.h>
#include <kernel/terminal.h>
#include <kernel/mbr.h>
#include <kernel/os.h>
#include <kernel/byte_converter.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

int round_up(float num) {
    float post_dec = num-(int)num;
    if (post_dec > 0 && post_dec < 1)
        return (num-post_dec)+1;
}

static mbr_t mbr;

void mbr_init() {
    mbr.bootstrap = (uint8_t *)malloc(sizeof(uint8_t)*446);
    int i;
    for (i = 0; i < 4; i++) {
        mbr.partitions[i].error = NO_MBR_ERROR;
        mbr.partitions[i].lba_first_sector = 0;
        mbr.partitions[i].sector_count = 0;
        mbr.partitions[i].lba_end_sector = 0;
    }
}

unsigned char parse_partition(int part_n, uint8_t *mbr_dat, size_t loc) {
    static char block_partitions = 0;
    
    partition_t *part = &mbr.partitions[part_n];
    char sys_id = mbr_dat[loc+4];

    if (sys_id == 0) {
        printf("sys_id(part: %d) is not set\n", part_n+1);
        mbr.partitions[part_n].error = EMPTY_PART;
        //empty partition
        //TODO: add partition manager
        // return 1;
    }

    if (sys_id == 0x5 || sys_id == 0xF || sys_id == 0x85) {
        part->error = EBR_PART;
        return 1; //EBR partition(extended boot record) is not supported
    }
    else {
        part->type = sys_id;
        part->lba_first_sector = to_uint32(mbr_dat, loc+8);
        part->sector_count = to_uint32(mbr_dat, loc+12);
        // if (part->error == EMPTY_PART) {
        //     if (block_partitions) {
        //         part->error = NO_DATA_PART;
        //         return 1;
        //     }
        //     part->lba_first_sector = mbr.partitions[part_n-1].lba_end_sector;
        //     block_partitions = 1;
        // }
        part->lba_end_sector = part->lba_first_sector+part->sector_count;


        part->bootable = mbr_dat[loc] == 0x81;
        return 0;
    }
}

void set_sysid(enum PARTITION_N part_n) {
    unsigned part_pos = PARTITION_ENTRY_1+(part_n*10);
    uint8_t sysid = 0x0C; //FAT32 LBA addressing
    ata_pio_rw(WRITE, part_pos+4, 0, &sysid, 1, 1);
}

void read_partitions_into_memory() {
    uint8_t *mbr_dat = (uint8_t *)malloc(512);
    ata_pio_rw(READ, 0, 0, mbr_dat, 512, 1);

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
    parse_partition(0, mbr_dat, PARTITION_ENTRY_1);
    //second partition
    parse_partition(1, mbr_dat, PARTITION_ENTRY_2);
    //third partition
    parse_partition(2, mbr_dat, PARTITION_ENTRY_3);
    //fourth partition
    parse_partition(3, mbr_dat, PARTITION_ENTRY_4);

    free(mbr_dat);
}

static enum PARTITION_N def_part = NONE;

void set_def_partition(enum PARTITION_N part_n) {
    if (def_part == NONE) def_part = part_n;
}
void write_to_partition(enum PARTITION_N part_n, enum ATA_RW mode, size_t rel_lba, uint8_t *data, int data_len, int sector_len) {
    if (mode == READ || mode == READ_SECTOR) {
        printf("write_to_partition: reading is not for use in writing function!\n");
        return;
    }
    partition_t *part;

    part = &mbr.partitions[part_n];
    ata_pio_rw(mode, part->lba_first_sector+rel_lba,
               0, data, data_len, sector_len);
}

void read_from_partition(enum PARTITION_N part_n, enum ATA_RW mode, size_t rel_lba, unsigned sec_amt, uint8_t *buf) {
    if (mode == WRITE || mode == WRITE_SECTOR) {
        printf("read_from_partition: writing is not for use in reading function!\n");
        return;
    }
    partition_t *part;
    part = &mbr.partitions[part_n];
    printf("reading from %d\n", part->lba_first_sector+rel_lba);
    ata_pio_rw(mode, part->lba_first_sector+rel_lba, 0, buf, NULL, sec_amt);
}

void read(enum ATA_RW mode, size_t rel_lba, uint8_t *buf, size_t buf_size, int sector_amt) {
    if (def_part == NONE) {
        printf("read(): partition set to NONE; skipping\n");
        return;
    }
    read_from_partition(def_part, mode, rel_lba, sector_amt, buf);
}

void write(enum ATA_RW mode, size_t rel_lba, uint8_t *dat, size_t dat_len, int sector_amt) {
    if (def_part == NONE) {
        printf("write(): partition set to NONE; skipping\n");
        return;
    }
    write_to_partition(def_part, mode, rel_lba, dat, dat_len, sector_amt);
}

mbr_t *get_mbr() {
    return &mbr;
}

void mbr_destroy() {
    free(mbr.bootstrap);
}