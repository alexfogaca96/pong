#ifndef RENDER_H
#define RENDER_H

#include <Windows.h>

struct {
    int width, height;
    unsigned int* pixels;
    BITMAPINFO bitmap;
} typedef RenderBuffer;

RenderBuffer renderBuffer;


void ClearScreen(unsigned int color);
void DrawRectInPixels(int x0, int y0, int x1, int y1, unsigned int color);

#endif
