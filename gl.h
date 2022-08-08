#ifndef __GL_H__
#define __GL_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "fonts.h"

static uint16_t*    gl_frame_buffer;
static uint16_t     gl_frame_width;
static uint16_t     gl_frame_height;
static size_t       gl_frame_buffer_size;

int gl_init(uint16_t w, uint16_t h)
{
	gl_frame_width = w;
	gl_frame_height = h;
	gl_frame_buffer_size = (size_t)(w * h * sizeof(uint16_t));
	gl_frame_buffer = (uint16_t*)malloc(gl_frame_buffer_size);

	if (gl_frame_buffer == NULL)
	{
		return 0;
	}

	return 1;
}

void gl_shutdown()
{
	free(gl_frame_buffer);

	gl_frame_buffer = NULL;

	gl_frame_width = 0;
	gl_frame_height = 0;
	gl_frame_buffer_size = 0;
}

void gl_draw_pixel(int16_t x, int16_t y, uint16_t color)
{
	if ((x >= gl_frame_width) || (y >= gl_frame_height) || (x < 0) || (y < 0))
	{
		return;
	}

	gl_frame_buffer[x+y*gl_frame_width] = color;
}

void gl_clear(uint16_t color = 0x0000)
{
	for (int y = 0; y < gl_frame_height; y++)
	{
		for (int x = 0; x < gl_frame_width; x++)
		{
			gl_draw_pixel(x, y, color);
		}
	}
}

// bresenhams line algorithm (https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm)
void gl_draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	if ((x0 > gl_frame_width && x1 > gl_frame_width) || (y0 > gl_frame_height && y1 > gl_frame_height) || (x0 < 0 && x1 < 0) || (y0 < 0 && y1 < 0))
	{
		return;
	}

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

void gl_draw_image(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint16_t* buffer)
{
	if ((x > gl_frame_width) || (h > gl_frame_height) || (x + w < 0) || (y + h < 0))
	{
		return;
	}

	for (int j = y; j < y+h; j++)
	{
		for (int i = x; i < x+w; i++)
		{
			if (i >= gl_frame_width || j >= gl_frame_height || i < 0 || j < 0)
			{
				continue;
			}

			gl_frame_buffer[i+j*gl_frame_width] = buffer[(i-x)+(j-y)*w];
		}
	}
}

void gl_draw_bitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint8_t* buffer, uint16_t color)
{
	if ((x > gl_frame_width) || (y > gl_frame_height) || (x + w < 0) || (y + h < 0))
	{
		return;
	}

	const uint8_t* p = buffer;

	for (uint16_t j = 0; j < h; j++)
	{
		for (uint16_t i = 0; i < w; i++)
		{
			if (*p & (0x80 >> (i % 8)))
			//if (*p & (1 << (i % 8)))
			{
				gl_draw_pixel(x + i, y + j, color);
			}

			if (i % 8 == 7)
			{
				p++;
			}
		}

		if (w % 8 != 0)
		{
			p++;
		}
	}
}

void gl_invert_bitmap(uint8_t* buffer, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		buffer[i] = ~buffer[i];
	}
}

// TODO: implement a font stack (something similar to Dear ImGui's approach)

void gl_draw_char(int16_t x, int16_t y, char ascii_char, const font_t* font, uint16_t color)
{
	if ((x > gl_frame_width) || (y > gl_frame_height) || (x + font->width < 0) || (y + font->width < 0))
	{
		return;
	}

	uint32_t bytes_spacing = font->height * (font->width >> 3 + !!(font->width % 8)); // version of "font->height * (font->width / 8 + ((font->width % 8) ? 1 : 0))" without division and if statement
	uint32_t offset = (ascii_char - ' ') * bytes_spacing;

	gl_draw_bitmap(x, y, font->width, font->height, &font->data[offset], color);
}

void gl_draw_text(int16_t x, int16_t y, const char* string, const font_t* font, uint16_t color)
{
	if ((x > gl_frame_width) || (y > gl_frame_height))
	{
		return;
	}

	const char* p = string;

	int16_t current_x = x;
	int16_t current_y = y;

	while (*p != '\0')
	{
		if (current_x + font->width >= gl_frame_width)
		{
			current_x = x;
			current_y += font->height;
		}

		if (*p == '\n')
		{
			current_x = x;
			current_y += font->height;

			p++;
		}
		else if (*p == '\t')
		{
			current_x += font->width * 3;

			p++;
		}

		gl_draw_char(current_x, current_y, *p, font, color);

		p++;
		current_x += font->width;
	}
}



#endif