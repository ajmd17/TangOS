#ifndef ATA_PIO_H
#define ATA_PIO_H

#include <stddef.h>

#define SECTOR_SIZE 512

void ata_pio_install();
void ata_pio_read(size_t lba, char *buffer, size_t count);
void ata_pio_write(size_t lba, char *data, size_t n_sectors);
void write(size_t lba, char *data, size_t n_sectors);
char *read(size_t lba, size_t sector_count);

#endif