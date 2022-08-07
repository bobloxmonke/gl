#ifndef __LCD_H__
#define __LCD_H__

#include <hardware/gpio.h>
#include <hardware/spi.h>

#include "lcdconf.h"

// TODO: make use of the overpowered pi pico PIO

static void lcd_cmd(uint8_t cmd)
{
    gpio_put(LCD_DC_PIN, 0);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(LCD_CS_PIN, 1);
}

static void lcd_data_8bit(uint8_t data)
{
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, &data, 1);
    gpio_put(LCD_CS_PIN, 1);
}

static void lcd_data_16bit(uint16_t data)
{
    lcd_data_8bit((uint8_t)(data >> 8));
	lcd_data_8bit((uint8_t)(data));
}

static void lcd_reset()
{
    gpio_put(LCD_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(LCD_RST_PIN, 1);
    sleep_ms(100);
}

static void lcd_init_reg()
{
    for (int i = 0; i < (sizeof(init_sequence)/sizeof(uint16_t)); i++)
    {
        uint16_t value = init_sequence[i];

        if (value & CMD_FLAG)
        {
            lcd_cmd(((uint8_t)value) & 0xff);
        }
        else
        {
            lcd_data_8bit(((uint8_t)value) & 0xff);
        }

        sleep_us(100);
    }
}

void lcd_init()
{ 
    gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_DIN_PIN, GPIO_FUNC_SPI);

    gpio_init(LCD_RST_PIN);
    gpio_set_dir(LCD_RST_PIN, 1);
    gpio_init(LCD_DC_PIN);
    gpio_set_dir(LCD_DC_PIN, 1);
    gpio_init(LCD_CS_PIN);
    gpio_set_dir(LCD_CS_PIN, 1);

    gpio_put(LCD_CS_PIN, 1);
    gpio_put(LCD_DC_PIN, 0);

    lcd_reset();
    lcd_init_reg();
}

static void lcd_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	lcd_cmd(LCD_CASET);
	lcd_data_16bit(x + LCD_OFFSET_X);
	lcd_data_16bit(x + width - 1 + LCD_OFFSET_X);
 
	lcd_cmd(LCD_RASET);
	lcd_data_16bit(y + LCD_OFFSET_Y);
	lcd_data_16bit(y + height - 1 + LCD_OFFSET_Y);
}

void lcd_flush(uint16_t* buffer, size_t size, uint16_t width, uint16_t height)
{
    lcd_set_window(0, 0, width, height);
	lcd_cmd(LCD_RAMWR);

    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    
    spi_write_blocking(SPI_PORT, (uint8_t*)buffer, size);
    
    gpio_put(LCD_CS_PIN, 1);
}

#endif