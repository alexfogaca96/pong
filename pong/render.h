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

ScreenSize initialScreenSize;
RenderBuffer renderBuffer;


void ClearScreen(unsigned int color);
void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color);
void DrawRectAnchorLeftBottom(vec2 v, vec2 size, unsigned int color);
void DrawRect(vec2 v, vec2 size, unsigned int color);
void DrawCircle(vec2 v, float diameter, unsigned int color);

float heightMultiplier();
float widthMultiplier();
vec2 normByMultiplier(vec2 v);

#endif
