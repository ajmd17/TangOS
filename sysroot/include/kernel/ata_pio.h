#ifndef ATA_PIO_H
#define ATA_PIO_H

#include <stddef.h>
#include <stdint.h>

void cache_flush();
void ata_pio_install();
void ata_pio_read(size_t lba, size_t buf_s_pos, uint8_t *buffer, size_t count);
void ata_pio_write(size_t lba, uint8_t *data, unsigned data_len, size_t n_sectors);
void ata_pio_write_seq(size_t lba, uint8_t *data, int amt_dat);
// uint8_t *ata_pio_rw(enum ATA_RW mode, size_t pos, size_t buf_s_pos, uint8_t *rw_dat, unsigned data_len, size_t n_sectors);
size_t lba_max_size();

#endif