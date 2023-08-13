#ifndef RENDER_H
#define RENDER_H

#include "math.h"

struct {
    int width, height;
    unsigned int* pixels;
} typedef RenderBuffer;

struct {
    int width, height;
} typedef ScreenSize;

struct {
	long width, height;
	char const* text;
	long textLength;
	unsigned int active;
} typedef Text;

enum {
	CLICK_TO_START,
	GAME_PAUSED,
	GAME_SCORE,
	TEXT_COUNT
};

ScreenSize initialScreenSize;
RenderBuffer renderBuffer;
Text uiTexts[TEXT_COUNT];


void ClearScreen(unsigned int color);
void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color);
void DrawRectAnchorLeftBottom(vec2 v, vec2 size, unsigned int color);
void DrawRect(vec2 v, vec2 size, unsigned int color);

float heightMultiplier();
float widthMultiplier();
vec2 normByMultiplier(vec2 v);

#endif
