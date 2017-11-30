#include <kernel/byte_converter.h>

uint16_t to_uint16(uint8_t *n, unsigned pos) {
    return (uint16_t)(n[pos+1] << 8 | n[pos]);
}

uint32_t to_uint32(uint8_t *n, unsigned pos) {
    return (unsigned)n[pos+3] << 24 | (unsigned)n[pos+2] << 16 |
           (unsigned)n[pos+1] << 8  | n[pos];
}