#include "game.h"
#include "math.h"
#include "render.h"
#include "platform_common.h"
#include "physics.h"

#include <stdio.h>

struct {
	vec2 pos;
	vec2 size;
	float speed;
	unsigned int color;
} typedef Paddle;

struct {
	vec2 pos;
	vec2 dir;
	float diameter;
	float speed;
	unsigned int color;
} typedef Ball;


Paddle leftPaddle, rightPaddle;
Ball ball;

void CreateGame() {
	float halfHeight	= (float) initialScreenSize.height / 2;
	float width5Percent = (float) initialScreenSize.width / 20; // 15% -> paddle -> 70% -> paddle -> 15%

	vec2 initialPaddlesSize = { 7.5, 60.0 };
	leftPaddle  = (Paddle) {
		.pos = { width5Percent * 3,  halfHeight + initialPaddlesSize.y / 2 },
		.size = initialPaddlesSize,
		.speed = 1000.0f,
		.color = 0xffffff };
	rightPaddle = (Paddle){
		.pos = { width5Percent * 17, halfHeight + initialPaddlesSize.y / 2 },
		.size = initialPaddlesSize,
		.speed = 1000.0f,
		.color = 0xffffff };

	float initialBallDiameter = 7.5;
	ball = (Ball) {
		.pos = { width5Percent * 10 + initialBallDiameter / 2, halfHeight + initialBallDiameter / 2 }, // screen center
		.dir = { 0.0, 1.0 },
		.speed = 2000.0f,
		.diameter = initialBallDiameter,
		.color = 0xffffff
	};

	DrawEverything();
}

void SimulateGame(Input* input, float deltaTime)
{
	SimulatePaddles(input, deltaTime);
	SimulateBall(deltaTime);
	DrawEverything();
}

void SimulatePaddles(Input* input, float deltaTime)
{
	if (IsDown(BUTTON_UP)) {
		float nextY = leftPaddle.pos.y + (leftPaddle.speed * deltaTime);
		unsigned int intersect = IntersectsScreenBoundUp(leftPaddle.pos, (vec2) { leftPaddle.pos.x, nextY + leftPaddle.size.y }, NULL, NULL);
		if (!intersect) leftPaddle.pos.y = nextY;
	}
	if (IsDown(BUTTON_DOWN)) {
		float nextY = leftPaddle.pos.y - (rightPaddle.speed * deltaTime);
		unsigned int intersect = IntersectsScreenBoundDown(leftPaddle.pos, (vec2) { leftPaddle.pos.x, nextY - leftPaddle.size.y }, NULL, NULL);
		if (!intersect) leftPaddle.pos.y = nextY;
	}
	rightPaddle.pos.y = leftPaddle.pos.y;
}

void SimulateBall(float deltaTime)
{
	vec2 nextPos = add(ball.pos, mul(ball.dir, ball.speed * deltaTime));

	float noIntersection = -1.0;
	float* intersectScreenX = &noIntersection;
	float* intersectScreenY = &noIntersection;
	float* intersectPaddleX = &noIntersection;
	float* intersectPaddleY = &noIntersection;
	if (!IntersectsScreenBoundUp(ball.pos, nextPos, intersectScreenX, intersectScreenY)) {
		IntersectsScreenBoundDown(ball.pos, nextPos, intersectScreenX, intersectScreenY);
	}
	if (!Intersects(ball.pos, nextPos, leftPaddle.pos, (vec2) { leftPaddle.pos.x, leftPaddle.pos.y + leftPaddle.size.y }, intersectPaddleX, intersectPaddleY)) {
		Intersects(ball.pos, nextPos, rightPaddle.pos, (vec2) { rightPaddle.pos.x, rightPaddle.pos.y + rightPaddle.size.y }, intersectPaddleX, intersectPaddleY);
	}

	// TODO: Calculate ball dir considering screen and paddle intersections:
	// - screen > no speed up or down
	// - paddle > define rule for ball angle and speed changes
}

void DrawEverything()
{
	ClearScreen(0x111111);
	DrawRect(leftPaddle.pos, leftPaddle.size, leftPaddle.color);
	DrawRect(rightPaddle.pos, rightPaddle.size, rightPaddle.color);
	// DrawCircle(ball.pos, ball.diameter, ball.color);  <-- waiting implementation
	DrawRect(ball.pos, (vec2) { ball.diameter, ball.diameter }, ball.color);
}
