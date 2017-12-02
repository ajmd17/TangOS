#ifndef MBR_H
#define MBR_H

#include <stddef.h>
#include <stdint.h>

#include <kernel/ata_pio.h>

#define PARTITION_ENTRY_1 0x1BE
#define PARTITION_ENTRY_2 0x1CE
#define PARTITION_ENTRY_3 0x1DE
#define PARTITION_ENTRY_4 0x1EE

enum PARTITION_N {
    NONE,
    PARTIIION_1,
    PARTITION_2,
    PARTITION_3,
    PARTITION_4
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
void set_sysid(enum PARTITION_N part_n);
void set_def_partition(enum PARTITION_N part_n);
void write_to_partition(enum PARTITION_N part_n, enum ATA_RW mode, size_t rel_lba, uint8_t *data, int data_len, int sector_len);
void read_from_partition(enum PARTITION_N part_n, enum ATA_RW mode, size_t rel_lba, unsigned sec_amt, uint8_t *buf);
void read(enum ATA_RW mode, size_t rel_lba, uint8_t *buf, size_t buf_size, int sector_amt);
void write(enum ATA_RW mode, size_t rel_lba, uint8_t *dat, size_t dat_len, int sector_amt);
mbr_t *get_mbr();
void mbr_destroy();

#endif