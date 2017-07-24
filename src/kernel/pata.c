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
        wait_400_ns();
        status = inb(STATUS(bar));
        if (status == ERROR) 
            assert(false);
        else 
            return status;
    } while (inb(BUSY))
}

void drive_init() {
    inw();
}

void bus_enumeration() {

}