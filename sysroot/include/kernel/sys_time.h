#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <stdint.h>

#include <kernel/sys_asm.h>

uint8_t bcd2bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

uint16_t time_getyear() {
	outb(0x70, 0x09);
	uint8_t time = inb(0x71);
	return bcd2bin(time) + 2000;
}

#endif
