#include <fs/fat32.h>
#include <ata_pio.h>
#include <mbr.h>

typedef struct {
    char dir_name[11];
    char dir_attr;
    /*-------------------------*/
    uint8_t  creation_time_tenth;
    uint16_t creation_time;
    uint16_t creation_date;
    /*-------------------------*/
    uint16_t last_access_date;
    /*-------------------------*/
    uint16_t fat_cluster_hi;
    uint16_t fat_cluster_lo;
    /*-------------------------*/
    uint16_t write_time;
    uint16_t write_date;
    /*-------------------------*/
    size_t file_size;
} dir_entry_t;

void fat_init() {
    dir_entry_t dir_entry;
}