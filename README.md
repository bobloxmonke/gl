# gl
*very simple graphics library for raspberry pi pico with support for ST7735 and ST7789 LCD controllers*

## features
- header-only
- designed for embedded systems
- 16-bit RGB color support
- no floating point math
- no dependencies
- images, bitmaps, and fonts support
- easy to use

## usage
here is a small snippet of code to get started with

```c
spi_init(SPI_PORT, 10000 * 1000);                                                   // initialize spi communication with 10000 KHz frequency

lcd_init();                                                                         // initialize the display

gl_init(LCD_WIDTH, LCD_HEIGHT);                                                     // graphics library initialization.
                                                                                    // make sure to call **gl_init** ==before== drawing and sending anything to the display.
gl_clear();                                                                         // clear the frame buffer. default clear color is black

gl_draw_line(8, 8, 128, 16, rgb_u16(255, 255, 0));                                  // draw a yellow line from [8, 8] to [128, 16].
gl_draw_text(8, 16, "hello world!\nnew line", &font8x14, rgb_u16(255, 255, 255));   // draw a text at [8, 16] using 8px by 14px font
gl_draw_bitmap(8, 48, 7, 7, bitmap, 0x0f70);                                        // draw a bitmap at [8, 48] with 7px width and 7px height. here we are passing raw 16 bit color

lcd_flush(gl_frame_buffer, gl_frame_buffer_size, gl_frame_width, gl_frame_height);  // send the frame buffer to the display over spi.
                                                                                    // note that 'gl' and 'lcd' are separate libraries and dont share any variables or functions
```

the code above produces following output:
<br>![image](https://user-images.githubusercontent.com/84726280/183489179-5ad3a943-2f2a-4dd3-b15b-2c402b8633df.png)

for more info contact me on discord \- **mr boblox#4444**
