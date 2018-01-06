#ifndef MBR_H
#define MBR_H

#include <stddef.h>
#include <stdint.h>

#include <kernel/ata_pio.h>

#define PARTITION_ENTRY_1 0x1BE
#define PARTITION_ENTRY_2 0x1CE
#define PARTITION_ENTRY_3 0x1DE
#define PARTITION_ENTRY_4 0x1EE

typedef enum {
    NO_MBR_ERROR,
    EMPTY_PART,
    EBR_PART,
    NO_DATA_PART,
} MBR_ERR;

typedef enum {
    NONE,
    PARTITION_1,
    PARTITION_2,
    PARTITION_3,
    PARTITION_4
} PARTITION;

typedef struct {
    MBR_ERR  error;
    uint8_t  bootable;
	uint8_t  type;
	unsigned lba_first_sector;
	unsigned sector_count;
    unsigned lba_end_sector;
} partition_t;

typedef struct {
    uint8_t    *bootstrap;
    partition_t partitions[4];
    uint8_t     signature[2];
} mbr_t;

void mbr_init();
void read_partitions_into_memory();
void set_sysid(PARTITION part_n);
void set_def_partition(PARTITION part_n);
void mbr_write(PARTITION part_n, size_t rel_lba, uint8_t *data, int data_len, int sector_len);
void mbr_read(PARTITION part_n, size_t rel_lba, unsigned sec_amt, uint8_t *buf);
mbr_t *get_mbr();
void mbr_destroy();

#endif