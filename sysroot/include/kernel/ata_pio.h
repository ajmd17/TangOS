#ifndef ATA_PIO_H
#define ATA_PIO_H

#include <stddef.h>
#include <stdint.h>

#define SECTOR_SIZE 512

void cache_flush();
void ata_pio_install();
void ata_pio_read(size_t lba, uint8_t *buffer, size_t count);
void ata_pio_write(size_t lba, uint8_t *data, unsigned data_len, size_t n_sectors);
void write(size_t lba, uint8_t *data, unsigned data_len, size_t n_sectors);
uint8_t *read(size_t lba, size_t sector_count);
size_t lba_max_size();

#endif