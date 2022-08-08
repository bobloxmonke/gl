#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct font_t
{
    const uint8_t* data;

    uint16_t width;
    uint16_t height;
};

#endif