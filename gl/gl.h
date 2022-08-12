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

void gl_draw_rect_filled(int16_t x_min, int16_t y_min, int16_t x_max, int16_t y_max, uint16_t color)
{
	if ((x_min > gl_frame_width) || (y_min > gl_frame_height) || (x_max < 0) || (y_max < 0) || (x_min > x_max) || (y_min > y_max))
	{
		return;
	}

	for (int16_t y = y_min; y < y_max; y++)
	{
		for (int16_t x = x_min; x < x_max; x++)
		{
			gl_draw_pixel(x, y, color);
		}
	}
}

void gl_draw_rect(int16_t x_min, int16_t y_min, int16_t x_max, int16_t y_max, uint16_t color)
{
	if ((x_min > gl_frame_width) || (y_min > gl_frame_height) || (x_max < 0) || (y_max < 0))
	{
		return;
	}

	gl_draw_line(x_min, y_min, x_max, y_min, color);
	gl_draw_line(x_min, y_min, x_min, y_max, color);
	gl_draw_line(x_max, y_max, x_max, y_min, color);
	gl_draw_line(x_max, y_max, x_min, y_max, color);
}

void gl_draw_image(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint16_t* buffer)
{
	if ((x > gl_frame_width) || (y > gl_frame_height) || (x + w < 0) || (y + h < 0))
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

// FIXME: memory endianness is fucking shit uppp
// is it the font issue? idk

//void gl_draw_bitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint8_t* buffer, uint16_t color)
//{
//	if ((x > gl_frame_width) || (y > gl_frame_height) || (x + w < 0) || (y + h < 0))
//	{
//		return;
//	}
//
//	const uint8_t* p = buffer;
//
//	for (uint16_t j = 0; j < h; j++)
//	{
//		for (uint16_t i = 0; i < w; i++)
//		{
//			if (*p & (0x80 >> (i % 8)))
//			//if (*p & (1 << (i % 8)))
//			{
//				gl_draw_pixel(x + i, y + j, color);
//			}
//
//			if (i % 8 == 7)
//			{
//				p++;
//			}
//		}
//
//		if (w % 8 != 0)
//		{
//			p++;
//		}
//	}
//}

//void gl_invert_bitmap(uint8_t* buffer, size_t size)
//{
//	for (int i = 0; i < size; i++)
//	{
//		buffer[i] = ~buffer[i];
//	}
//}

// TODO: implement a font stack (something similar to Dear ImGui's approach)

void gl_draw_bitmap(int16_t x, int16_t y, uint16_t w, uint16_t h, const uint8_t* bitmap, uint16_t fg_color, uint16_t bg_color = 0x0000)
{
	const uint8_t* p = bitmap;

	int16_t byte_width = (w + 7) / 8;
	uint8_t b = 0;

	for (uint16_t j = 0; j < h; j++)
	{
		for (uint16_t i = 0; i < w; i++)
		{
			b = (i & 7) ? (b << 1) : (p[(i / 8) + j * byte_width]); // maybe use rsh 3 instead of division. idk if gcc-arm compiler optimizes division like this?

			gl_draw_pixel(x + i, y + j, (b & 0x80) ? fg_color : bg_color);
		}
	}
}

void gl_draw_char(int16_t x, int16_t y, char ascii_char, const font_t* font, uint16_t fg_color, uint16_t bg_color = 0x0000)
{
	if ((x > gl_frame_width) || (y > gl_frame_height) || (x + font->width < 0) || (y + font->width < 0))
	{
		return;
	}

	uint32_t offset = (ascii_char - ' ') * font->height * ((font->width >> 3) + !!(font->width % 8));

	gl_draw_bitmap(x, y, font->width, font->height, &font->data[offset], fg_color, bg_color);
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

		while (*p == '\n')
		{
			current_x = x;
			current_y += font->height;

			p++;
		}

		gl_draw_char(current_x, current_y, *p, font, color);

		current_x += font->width;
		p++;
	}
}



#endif