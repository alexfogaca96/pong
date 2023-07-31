#ifndef WIN32_RENDER_H
#define WIN32_RENDER_H

#include <Windows.h>

struct {
	BITMAPINFO bitmap;
} typedef Win32RenderBuffer;

Win32RenderBuffer win32RenderBuffer;

#endif