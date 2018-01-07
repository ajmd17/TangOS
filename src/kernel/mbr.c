//TODO: heavy cleanup of code and get rid of unused functions.

#include <kernel/sys_asm.h>
#include <kernel/terminal.h>
#include <kernel/mbr.h>
#include <kernel/os.h>
#include <kernel/byte_converter.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

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

    printf("part %d is %x\n", part_n, sys_id);
    if (sys_id == 0) {
        // printf("sys_id(part: %d) is not set\n", part_n+1);
        mbr.partitions[part_n].error = EMPTY_PART;
        //empty partition
        //TODO: add partition manager
        // return 1;
    }
    else if (sys_id == 0x05 || sys_id == 0x0F || sys_id == 0x85) {
        part->error = EBR_PART;
        return 1; //EBR partition(extended boot record) is not supported
    }
    else if (sys_id == 0x0C || sys_id == 0x0B) {
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


        part->bootable = (mbr_dat[loc] == 0x81);
        return 0;
    }
    else {
        return 1;
    }
}

void set_sysid(PARTITION part_n) {
    unsigned part_pos = PARTITION_ENTRY_1+(part_n*10);
    uint8_t sysid = 0x0C; //FAT32 LBA addressing
    ata_pio_write(part_pos+4, &sysid, 1, 1);
}

void read_partitions_into_memory() {
    uint8_t *mbr_dat = (uint8_t *)malloc(512);

    ata_pio_read(0, 0, mbr_dat, 1);

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

void mbr_write(PARTITION part_n, size_t rel_lba, uint8_t *data, int data_len, int n_sectors) {
    partition_t *part = &mbr.partitions[part_n];
    ata_pio_write(part->lba_first_sector+rel_lba, data, data_len, n_sectors);
    printf("writing to partition %d(%x)\n", part_n, PARTITION_ENTRY_1+(part_n*10));
}

void mbr_read(PARTITION part_n, size_t rel_lba, unsigned sec_amt, uint8_t *buf) {
    partition_t *part = &mbr.partitions[part_n];
    ata_pio_read(part->lba_first_sector+rel_lba, 0, buf, sec_amt);
    printf("reading from partition %d(%x)\n", part_n, PARTITION_ENTRY_1+(part_n*10));    
}

void mbr_write_seq(PARTITION part_n, size_t rel_lba, uint8_t *data, int n_chars) {
    partition_t *part = &mbr.partitions[part_n];
    ata_pio_write_seq(part->lba_first_sector+rel_lba, data, n_chars);
}

mbr_t *get_mbr() {
    return &mbr;
}

void mbr_destroy() {
    free(mbr.bootstrap);
}

int get_partition_entry(int n) {
    return PARTITION_ENTRY_1+(n*10);
}