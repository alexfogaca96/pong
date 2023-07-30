#include "math.h"
#include "render.h"
#include "platform_common.h"

vec2 playerPos;

void SimulateGame(Input* input, float deltaTime)
{
	ClearScreen(0x551100);

	float speed = 1000.0f;
	if (IsDown(BUTTON_LEFT)) playerPos.x -= speed * deltaTime;
	if (IsDown(BUTTON_RIGHT)) playerPos.x += speed * deltaTime;
	if (IsDown(BUTTON_UP)) playerPos.y += speed * deltaTime;
	if (IsDown(BUTTON_DOWN)) playerPos.y -= speed * deltaTime;

	DrawRect(playerPos, (vec2) { 10, 10 }, 0xffff00);
}