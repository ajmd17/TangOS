#ifndef MBR_H
#define MBR_H

#include <stdint.h>

#define __PARTITION_ENTRY_1 0x1BE
#define __PARTITION_ENTRY_2 0x1CE
#define __PARTITION_ENTRY_3 0x1DE
#define __PARTITION_ENTRY_4 0x1EE

enum __PARTITION_N {
    __PARTIIION_1,
    __PARTITION_2,
    __PARTITION_3,
    __PARTITION_4
};

typedef struct {
    unsigned char error;
    uint8_t       bootable;
	uint8_t       type;
	uint32_t      lba_first_sector;
	uint32_t      sector_count;
} partition_t;

typedef struct {
    uint8_t    *bootstrap;
    partition_t partitions[4];
    uint8_t     signature[2];
} mbr_t;

void mbr_init();
void read_partitions_into_memory();
void write_to_partition(enum __PARTITION_N part_n, size_t rel_lba, uint8_t *data, int data_len);
uint8_t *read_from_partition(enum __PARTITION_N part_n, size_t rel_lba, unsigned sec_amt);
mbr_t *get_mbr();
void mbr_destroy();

#endif