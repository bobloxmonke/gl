#ifndef __GL_H__
#define __GL_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint16_t*    frame_buffer;
static uint16_t     frame_width;
static uint16_t     frame_height;
static size_t       frame_buffer_size;

uint16_t rgb_u16(uint8_t r, uint8_t g, uint8_t b)
{
    uint16_t color = 0x0000;

	color = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
    color = ((color & 0x00ff) << 8) | ((color & 0xff00) >> 8); // rev16

    return color;
}

int gl_init(uint16_t w, uint16_t h)
{
    frame_width = w;
    frame_height = h;
    frame_buffer_size = (size_t)(w * h * sizeof(uint16_t));
    frame_buffer = (uint16_t*)malloc(frame_buffer_size);

    if (frame_buffer == NULL)
    {
        return 0;
    }

    return 1;
}

void gl_shutdown()
{
    free(frame_buffer);

    frame_buffer = NULL;

	frame_width = 0;
    frame_height = 0;
    frame_buffer_size = 0;
}

void gl_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
    if (x >= frame_width || y >= frame_height || x < 0 || y < 0)
    {
        return;
    }

    frame_buffer[x+y*frame_width] = color;
}

void gl_clear(uint16_t color = 0x0000)
{
	for (int y = 0; y < frame_height; y++)
	{
		for (int x = 0; x < frame_width; x++)
		{
			gl_draw_pixel(x, y, color);
		}
	}
}

// bresenhams line algorithm (https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
void gl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	int e = dx + dy;

	while (1)
	{
		gl_draw_pixel(x0, y0, color);

		if (x0 == x1 && y0 == y1)
		{
			break;
		}

		int e2 = e * 2;

		if (e2 >= dy)
		{
			if (x0 == x1)
			{
				break;
			}

			e += dy;
			x0 += sx;
		}

		if (e2 <= dx)
		{
			if (y0 == y1)
			{
				break;
			}

			e += dx;
			y0 += sy;
		}
	}
}


#endif