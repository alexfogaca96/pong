#include "render.h"
#include "math.h"

#define GAME_SCALE 0.001f
#define MAX_RESOLUTION_WIDTH 1920
#define MAX_RESOLUTION_HEIGHT 1080


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

void DrawRectAnchorLeftBottom(vec2 v, vec2 size, unsigned int color)
{
	int x0 = (int)(v.x * widthMultiplier());
	int y0 = (int)(v.y * heightMultiplier());
	int x1 = (int)((v.x + size.x) * widthMultiplier());
	int y1 = (int)((v.y + size.y) * heightMultiplier());
	DrawRectInPixels(x0, y0, x1, y1, color);
}

void DrawRect(vec2 v, vec2 size, unsigned int color)
{
	int x0 = (int)((v.x - size.x / 2) * widthMultiplier());
	int y0 = (int)((v.y - size.y / 2) * heightMultiplier());
	int x1 = (int)((v.x + size.x / 2) * widthMultiplier());
	int y1 = (int)((v.y + size.y / 2) * heightMultiplier());
	DrawRectInPixels(x0, y0, x1, y1, color);
}

float widthMultiplier()
{
	return (float)Clamp(0, renderBuffer.width, MAX_RESOLUTION_WIDTH) / initialScreenSize.width;
}

float heightMultiplier()
{
	return (float)Clamp(0, renderBuffer.height, MAX_RESOLUTION_WIDTH) / initialScreenSize.height;
}

vec2 normByMultiplier(vec2 v)
{
	return (vec2) { v.x * widthMultiplier(), v.y * heightMultiplier() };
}
