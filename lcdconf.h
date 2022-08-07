#ifndef __LCD_CONFIG_H__
#define __LCD_CONFIG_H__

#include "st7735.h"

#define SPI_PORT        spi0

#define LCD_WIDTH       160
#define LCD_HEIGHT      128

#define LCD_OFFSET_X    1
#define LCD_OFFSET_Y    2

#define LCD_CS_PIN      17
#define LCD_CLK_PIN     18
#define LCD_DIN_PIN     19
#define LCD_DC_PIN      20
#define LCD_RST_PIN     21

#define LCD_CASET       0x2a
#define LCD_RASET       0x2b
#define LCD_RAMWR       0x2c

#endif