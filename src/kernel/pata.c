#include <kernel/pata.h>
#include <kernel/sys_asm.h>
#include <assert.h>
#include <stdlib.h>

void wait_400_ns(int bar) {
    inb(STATUS(bar));
    inb(STATUS(bar));
    inb(STATUS(bar));
    inb(STATUS(bar));    
}

char send_command(char command, int bar) {
    outb(COMMAND(bar), command);

    char status;

    do {
        wait_400_ns(BAR_0_PRIMARY);
        status = inb(STATUS(bar));
        if (status == ERROR) {
            assert(0, "ERROR!");
        }
        else { 
            return status;
        }
    } while (inb(BUSY));
}

void read_uint16s(uint16_t *data, int len) {
    int i;
    for (i = 0; i < len; i++) {
        data[i] = (uint16_t)0;
    }
}

char *get_string(uint16_t *buffer, int start, int len) {
    char *new_str = (char *)malloc(len);
    int i;
    for (i = 0; i < len/2; i++) {
        uint16_t xchar = buffer[start+i];
        new_str[i*2] = (char)((xchar >> 8) & 0xFF);
        new_str[i*2+1] = (char)(xchar & 0xFF);        
    }
    return new_str;
}

void drive_init() {
     uint16_t *device_info_buffer = (uint16_t *)malloc(256);
     read_uint16s(device_info_buffer, 256);
     char *serial_number = get_string(device_info_buffer, 10, 20);
     char *firmware_rev = get_string(device_info_buffer, 23, 8);     
     char *model_no = get_string(device_info_buffer, 27, 40);   
     unsigned long block_count = ((uint32_t)device_info_buffer[61] << 16 | device_info_buffer[60])-1;  
}

void discover_disks(int bar) {
    outb(SECTOR_COUNT(bar), 0);
    outb(LBA0(bar), 0);
    outb(LBA1(bar), 0);
    outb(LBA2(bar), 0);
    
    int val = send_command(IDENTIFY, bar);
    if (val == ERROR) {
        assert(0, "NOT A PATA DRIVE!");
    }
    else if (val == 0) {
        assert(0, "NO DRIVE ATTACHED!");
    }
    char status;
    do {
        wait_400_ns(bar);
        status = inb(STATUS(bar));
        if (status != DRQ && status != ERROR) drive_init();

    } while (status != DRQ || status != ERROR);
}

void bus_enumeration() {

}