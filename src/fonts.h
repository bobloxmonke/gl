#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "font8x14.h"

struct font_t
{
    const uint8_t* data;

    uint16_t width;
    uint16_t height;
};

font_t font8x14 = { font8x14_data, 8, 14 };

#endif