#include <kernel/pata.h>
#include <kernel/sys_asm.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void wait_400_ns(int bar) {
    inb(STATUS(bar));
    inb(STATUS(bar));
    inb(STATUS(bar));
    inb(STATUS(bar));    
}

uint8_t send_command(uint8_t command, int bar) {
    outb(COMMAND(bar), command);
    printf("command = %d\n\n", (int)command);

    uint8_t status;
    int timeout = 20000000;
    
    do {
        wait_400_ns(bar);
        status = inb(STATUS(bar));
        //printf("status = %d\n", (int)status);
        assert((status & ERROR) == 0, "Error! (E0001)");
    } while ((status & DRQ) == 0 && (status & ERROR) == 0);
    printf("Stat=%d\n", status);
    return status;
}

void read_uint16s(uint16_t *data, int len) {
    int i;
    for (i = 0; i < len; i++) {
        data[i] = inw(BAR_1_PRIMARY);
        //printf("data[%d] = %d", i, data[i]);
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

int drive_init() {
     uint16_t *device_info_buffer = (uint16_t *)malloc(256);
     read_uint16s(device_info_buffer, 256);
     char *serial_number = get_string(device_info_buffer, 10, 20);
     char *firmware_rev = get_string(device_info_buffer, 23, 8);     
     char *model_no = get_string(device_info_buffer, 27, 40);   
     unsigned long block_count = ((uint32_t)device_info_buffer[61] << 16 | device_info_buffer[60])-1;  
     printf("fudge? %s", serial_number);
     //while (1);
     free(device_info_buffer);
     free(serial_number);
     free(firmware_rev);     
     free(model_no);
}

void select_drive(int bar, BusPosition_t bp, char aLbaHigh4, bool setLBA) {
    uint8_t val = DRIVE_SELECT_DEFAULT;

    if (bp == BUS_POS_SLAVE) {
        val |= DRIVE_SELECT_SLAVE;
    }

    if (setLBA) {
        val |= DRIVE_SELECT_LBA;
    }

    val |= aLbaHigh4;
    
    outb(bar, val);
    wait_400_ns(bar);
}

void discover_disks(int bar) {
    BusPosition_t bp = BUS_POS_MASTER;

    select_drive(bar, bp, 0, false);
    outb(CONTROL(bar), 0x02);

    select_drive(bar, bp, 0, false);

    outb(SECTOR_COUNT(bar), 0);
    outb(LBA0(bar), 0);
    outb(LBA1(bar), 0);
    outb(LBA2(bar), 0);
    
    int val = send_command(IDENTIFY, bar);
    printf("val = %d\n", val);

    assert((val & ERROR) == 0, "Not a PATA drive");
    assert(val != 0, "No drive attached.");

    uint8_t status;
    do {
        wait_400_ns(bar);
        status = inb(STATUS(bar));
    } while ((status & DRQ) == 0 && (status & ERROR) == 0);


    printf("STATUS = %d\n", status);

    drive_init();
}