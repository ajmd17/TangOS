#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/sys_asm.h>
#include <kernel/ata_pio.h>
#include <kernel/os.h>

unsigned char primary_detected = 0;
unsigned short disk_info[256];
unsigned char pio_48_supported = 0;
unsigned int sector_count = 0;

void cache_flush() {
	outb(0x1F7, 0xE7); //send cache flush command
	while ((inb(0x1F7) & (0x80 | 0x40)) != 0x40); //wait for DRIVE READY and BUSY bit to clear	
}

void ata_pio_install() {
	char detect;
	unsigned int i, tmp2;
	
	//identify and select drive
	outb(0x1F6, 0xA0);

	//set sector count, lbalo, lbamid and lbahi IO ports to 0
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);

	//send identify to command port
	outb(0x1F7, 0xEC);

	//see if drive exists
	detect = inb(0x1F7);

	if (detect != 0) {
		primary_detected = 1;
		#if DEBUG_PRINT
		puts("drive detected\n");
		#endif
	} else {
		puts("Drive does not exist.\n");
		return;
	}

	detect = inb(0x1F4);
	detect += inb(0x1F5);

	if (detect > 0) {
		puts("Disk is not PATA compatible.\n");
		return;
	}

	//read data from data port(256 words)
	for (i = 0; i < 256; i++) {
		disk_info[i] = inw(0x1F0);
	}

	if ((disk_info[83] & 0x10) == 0x10) {
		pio_48_supported = 1;
		#if DEBUG_PRINT
		puts("PIO 48 read supported\n");
		#endif
	} else {
		tmp2 = disk_info[83];
		#if DEBUG_PRINT
		puts("PIO read 48 not supported");
		printf("word 83 = %x\n", tmp2);
		#endif
	}

	tmp2 = disk_info[60]; //word size
	tmp2 = tmp2 << 16;
	tmp2 += disk_info[61];
	#if DEBUG_PRINT
	printf("disk supports %d total lba addressable sectors\n", tmp2);
	#endif
	sector_count = tmp2;
}

void ata_pio_read(size_t lba, size_t buf_s_pos, uint8_t *buff, size_t count) {
	while ((inb(0x1F7) & (0x80 | 0x40)) != 0x40); //wait for DRIVE READY and BUSY bit to clear	

	size_t bytes_read;
	volatile unsigned short word;
	volatile int tmp;
	//primary bus
	unsigned short port = 0x1F0;
	unsigned short slavebit = 0;

	if (!count) {
		printf("nothing to read\n");
		return;
	}

	//0xE0 for master, 0xF0 for slave
	outb(0x1F6, 0xE0 | (slavebit << 4) | ((lba >> 24) & 0x0F));

	outb(0x1F2, 0x01); //Read one sector

	//Send the low 8 bits of the LBA to port 0x1F3:
	outb(0x1F3, (unsigned char) lba);
	//Send the next 8 bits of the LBA to port 0x1F4:
	outb(0x1F4, (unsigned char)(lba >> 8));
	//Send the next 8 bits of the LBA to port 0x1F5:
	outb(0x1F5, (unsigned char)(lba >> 16));

	outb(0x1F7, 0x20); //send read sectors command

	for (tmp = 0; tmp < 3000; tmp++); //wait for IRQ or poll
	while ((inb(0x1F7) & (0x80 | 0x40)) != 0x40); //wait for DRIVE READY and BUSY bit to clear

	bytes_read = 0;

	while (bytes_read < 256) {
		word = 0;
		word = inw(0x1F0);

		buff[buf_s_pos+(bytes_read * 2)] = word & 0xFF;
		buff[buf_s_pos+((bytes_read * 2) + 1)] = word >> 8;

		bytes_read++;
	}

	//printf("reading at %d, read %d bytes\n", lba, bytes_read);
}

void ata_pio_write(size_t lba, uint8_t *data, unsigned data_len, size_t n_sectors) {
	outb(0x1F2, n_sectors); //set number of sectors
	//Send the low 8 bits of the LBA to port 0x1F3:
	outb(0x1F3, (unsigned char) lba);
	//Send the next 8 bits of the LBA to port 0x1F4:
	outb(0x1F4, (unsigned char)(lba >> 8));
	//Send the next 8 bits of the LBA to port 0x1F5:
	outb(0x1F5, (unsigned char)(lba >> 16));

	outb(0x1F7, 0x30); //initialize write command

	int i;
	for (i = 0; i < data_len; i++) {
		while ((inb(0x1F7) & (0x80 | 0x40)) != 0x40); //wait for DRIVE READY and BUSY bit to clear
		outb(0x1F0, data[i]); //write to io port
		cache_flush();
	}

	uint8_t *temp_buf = (uint8_t *)malloc(512);
	ata_pio_read(lba, 0, temp_buf, 1);
	free(temp_buf);
}

uint8_t *ata_pio_rw(enum ATA_RW mode, size_t pos, size_t buf_s_pos, uint8_t *rw_dat, unsigned data_len, size_t n_sectors) {
	switch(mode) {
		case READ:
			ata_pio_read(pos, buf_s_pos, rw_dat, n_sectors);
			break;
		case WRITE:
			ata_pio_write(pos, rw_dat, data_len, n_sectors);
			break;
		case READ_SECTOR:
			ata_pio_read(pos*512, buf_s_pos, rw_dat, n_sectors);
			break;
		case WRITE_SECTOR:
			ata_pio_write(pos*512, rw_dat, data_len, n_sectors);
			break;
	};
}

size_t lba_max_size() {
	size_t max_lba = 0;
	outb(0x1F7, 0x27); //read native max address
	while ((inb(0x1F7) & (0x80 | 0x40)) != 0x40); //wait for DRIVE READY and BUSY bit to clear
	max_lba =  (unsigned long long )inb(0x1F7+3);
	max_lba += (unsigned long long )inb(0x1F7+4) << 8;
	max_lba += (unsigned long long )inb(0x1F7+5) << 16;
	outb(0x3F4, 0x80); //set HOB
	max_lba += (unsigned long long )inb(0x1F7+3) << 24;
	max_lba += (unsigned long long )inb(0x1F7+4) << 32;
	max_lba += (unsigned long long )inb(0x1F7+5) << 40;
	return max_lba;
}