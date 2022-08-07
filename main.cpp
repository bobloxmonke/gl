#include <stdio.h>

#include <pico/stdlib.h>

#include "gl.hpp"
#include "lcd.hpp"

int main()
{
    // stdio_init_all();

    spi_init(SPI_PORT, 10000 * 1000); // 10000 KHz

    lcd_init();
    gl_init(LCD_WIDTH, LCD_HEIGHT);

    gl_clear();

    

    // send the buffer to the lcd over spi
    lcd_flush(frame_buffer, frame_buffer_size, frame_width, frame_height);

    return 0;
}