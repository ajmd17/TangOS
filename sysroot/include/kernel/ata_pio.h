#ifndef ATA_PIO_H
#define ATA_PIO_H

#include <stddef.h>
#include <stdint.h>

#define SECTOR_SIZE 512

enum ATA_RW {
    READ,
    WRITE,
    READ_SECTOR,
    WRITE_SECTOR,
};

void cache_flush();
void ata_pio_install();
void ata_pio_read(size_t lba, uint8_t *buffer, size_t count);
void ata_pio_write(size_t lba, uint8_t *data, unsigned data_len, size_t n_sectors);
uint8_t *ata_pio_rw(enum ATA_RW mode, size_t pos, uint8_t *rw_dat, unsigned data_len, size_t n_sectors);
size_t lba_max_size();

#endif