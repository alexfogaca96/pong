#ifndef RENDER_H
#define RENDER_H

#include "math.h"

struct {
    int width, height;
    unsigned int* pixels;
} typedef RenderBuffer;

RenderBuffer renderBuffer;


void ClearScreen(unsigned int color);
void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color);
void DrawRect(vec2 v, vec2 halfSize, unsigned int color);

#endif
