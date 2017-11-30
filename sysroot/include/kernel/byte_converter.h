#ifndef BYTE_CONVERTER_H
#define BYTE_CONVERTER_H

#include <stdint.h>

uint16_t to_uint16(uint8_t *n, unsigned pos);
uint32_t to_uint32(uint8_t *n, unsigned pos);

#endif