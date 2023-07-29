#include "render.h"
#include "math.h"

void ClearScreen(unsigned int color)
{
	unsigned int* pixel = renderBuffer.pixels;
	for (int y = 0; y < renderBuffer.height; y++) {
		for (int x = 0; x < renderBuffer.width; x++) {
			*pixel++ = color;
		}
	}
}

void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color)
{
	x0 = Clamp(0, x0, renderBuffer.width);
	x1 = Clamp(0, x1, renderBuffer.width);
	y0 = Clamp(0, y0, renderBuffer.height);
	y1 = Clamp(0, y1, renderBuffer.height);

	for (int y = y0; y < y1; y++) {
		unsigned int* pixel = renderBuffer.pixels + x0 + renderBuffer.width * y;
		for (int x = x0; x < x1; x++) {
			*pixel++ = color;
		}
	}
}