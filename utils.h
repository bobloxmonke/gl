#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint16_t rgb_u16(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = 0x0000;

	color = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
	color = ((color & 0x00ff) << 8) | ((color & 0xff00) >> 8); // rev16

	return color;
}

#endif