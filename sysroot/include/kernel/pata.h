#ifndef PATA_H
#define PATA_H

#define PRIMARY 0
#define SECONDARY 1

#define MASTER 0
#define SLAVE 1

#define READ_DMA 0xC8
#define READ_DMA_EXT 0x25
#define WRITE_DMA 0xCA
#define WRITE_DMA_EXT 0x35

#define READ 0xA8
#define EJECT 0x1B
#define CACHE_FLUSH 0xE7
#define CACHE_FLUSH_EXT 0xEA
#define IDENTIFY 0xEC

#define PATA 0x0001

#define BAR_0_PRIMARY 0x01F0
#define BAR_0_SECONDARY 0x0170

#define BAR_1_PRIMARY 0x03F4
#define BAR_1_SECONDARY 0x0374

#define DATA(BAR_0) BAR_0+0
#define SECTOR_COUNT(BAR_0) BAR_0+2
#define LBA0(BAR_0) BAR_0+3
#define LBA1(BAR_0) BAR_0+4
#define DEVICE_SELECT(BAR_0) BAR_0+6
#define LBA2(BAR_0) BAR_0+5
#define COMMAND(BAR_0) BAR_0+7 //write only
#define STATUS(BAR_0) BAR_0+7  //read only
#define CONTROL(BAR_1) BAR_1+2

enum { //Status flags
    NONE       = 0x00,
    BUSY       = 0x80,
    ATA_SR_DRD = 0x40,
    ATA_SR_DF  = 0x20,
    ATA_SR_DSC = 0x10,
    DRQ        = 0x08,
    ATA_SR_COR = 0x04,
    ATA_SR_IDX = 0x02,
    ERROR      = 0x01
};

enum { //Error flags
    ATA_ER_BBK   = 0x80,
    ATA_ER_UNC   = 0x40,
    ATA_ER_MC    = 0x20,
    ATA_ER_IDNF  = 0x10,
    ATA_ER_MCR   = 0x08,
    ATA_ER_ABRT  = 0x04,
    ATA_ER_TK0NF = 0x02,
    ATA_ER_AMNF  = 0x01
};

#endif