#include "render.h"
#include "math.h"

#define GAME_SCALE 0.001f
#define ASPECT_RATIO 1.77f

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

/* didn't quite understand and other than being pixel-independent it seems bad */
void DrawRect(vec2 v, vec2 halfSize, unsigned int color)
{
	float aspectMultiplier = (float) renderBuffer.height;
	if ((float) renderBuffer.width / (float) renderBuffer.height < ASPECT_RATIO) {
		aspectMultiplier = (float) renderBuffer.width / ASPECT_RATIO;
	}

	float aspectInScale = aspectMultiplier * GAME_SCALE;
	halfSize.x *= aspectInScale;
	halfSize.y *= aspectInScale;
	v.x *= aspectInScale;
	v.y *= aspectInScale;

	int x0 = (int) (v.x - halfSize.x);
	int y0 = (int) (v.y - halfSize.y);
	int x1 = (int) (v.x + halfSize.x);
	int y1 = (int) (v.y + halfSize.y);

	DrawRectInPixels(x0, y0, x1, y1, color);
}
