#include "math.h"
#include "render.h"
#include "platform_common.h"
#include "physics.h"


struct {
	vec2 pos;
	vec2 size;
	unsigned int color;
} typedef Paddle;

vec2 initialSize = { 10, 60 };
Paddle leftPaddle, rightPaddle;
float moveSpeed = 1000.0f;


void CreateGame() {
	float halfHeight	= (float) initialScreenSize.height / 2;
	float width5Percent = (float) initialScreenSize.width / 20;
	leftPaddle  = (Paddle) { .pos = { width5Percent * 3,  halfHeight + initialSize.y / 2 }, .size = initialSize, .color = 0xffff00 };
	rightPaddle = (Paddle) { .pos = { width5Percent * 17, halfHeight + initialSize.y / 2 }, .size = initialSize, .color = 0xffff00 };

	ClearScreen(0x551100);
	DrawRect(leftPaddle.pos, leftPaddle.size, 0xffff00);
	DrawRect(rightPaddle.pos, rightPaddle.size, 0xffff00);
}

void SimulateGame(Input* input, float deltaTime)
{
	if (IsDown(BUTTON_UP)) {
		float nextY = leftPaddle.pos.y + (moveSpeed * deltaTime);
		unsigned int intersect = IntersectsScreenBoundUp(
			(vec2) { leftPaddle.pos.x, leftPaddle.pos.y },
			(vec2) { leftPaddle.pos.x, nextY + leftPaddle.size.y });
		if (!intersect) leftPaddle.pos.y = nextY;
	}

	
	if (IsDown(BUTTON_DOWN)) {
		float nextY = leftPaddle.pos.y - (moveSpeed * deltaTime);
		unsigned int intersect = IntersectsScreenBoundDown(
			(vec2) { leftPaddle.pos.x, leftPaddle.pos.y },
			(vec2) { leftPaddle.pos.x, nextY - leftPaddle.size.y });
		if (!intersect) leftPaddle.pos.y = nextY;
	}

	rightPaddle.pos.y = leftPaddle.pos.y;


	ClearScreen(0x551100);
	DrawRect(leftPaddle.pos, leftPaddle.size, 0xffff00);
	DrawRect(rightPaddle.pos, rightPaddle.size, 0xffff00);
}
