#include "render.h"
#include "platform_common.h"

int posX = 20;
int posY = 20;
int size = 30;

void SimulateGame(Input* input)
{
	ClearScreen(0x551100);
	
	if (Pressed(BUTTON_LEFT))   posX -= 20;
	if (Pressed(BUTTON_RIGHT))	posX += 20;
	if (Pressed(BUTTON_UP))		posY += 20;
	if (Pressed(BUTTON_DOWN))	posY -= 20;

	DrawRectInPixels(posX, posY, posX + size, posY + size, 0xffff00);
}