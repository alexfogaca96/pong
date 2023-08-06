#include "game.h"
#include "math.h"
#include "render.h"
#include "platform_common.h"
#include "physics.h"

#include <stdlib.h>

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
	float width5Percent = (float) initialScreenSize.width / 20; 

	vec2 initialPaddlesSize = { 15.0, 130.0 };
	leftPaddle  = (Paddle) {
		.pos = { (width5Percent * 3) - (initialPaddlesSize.x / 2), halfHeight - initialPaddlesSize.y / 2 }, // 15% left
		.size = initialPaddlesSize,
		.speed = 750.0f,
		.color = 0xffffff };
	rightPaddle = (Paddle){
		.pos = { (width5Percent * 17) - (initialPaddlesSize.x / 2), halfHeight - initialPaddlesSize.y / 2 }, // 15% right
		.size = initialPaddlesSize,
		.speed = 750.0f,
		.color = 0xffffff };

	float initialBallDiameter = 12.0;
	ball = (Ball) {
		.pos = { (width5Percent * 10) - (initialBallDiameter / 2), halfHeight - (initialBallDiameter / 2) }, // screen center
		.dir = { 1.0, 0.0 },
		.speed = 1000.0f,
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
		unsigned int intersect = IntersectsScreenBoundDown(leftPaddle.pos, (vec2) { leftPaddle.pos.x, nextY }, NULL, NULL);
		if (!intersect) leftPaddle.pos.y = nextY;
	}
	rightPaddle.pos.y = leftPaddle.pos.y;
}

void SimulateBall(float deltaTime)
{
	vec2 nextPos = add(ball.pos, mul(ball.dir, ball.speed * deltaTime));
	float sx = -1.0, sy = -1.0, px = -1.0, py = -1.0;
	float* intersectScreenX = &sx;
	float* intersectScreenY = &sy;
	float* intersectPaddleX = &px;
	float* intersectPaddleY = &py;
	if (IntersectsScreenBoundUp(ball.pos, nextPos, intersectScreenX, intersectScreenY)
	 || IntersectsScreenBoundDown(ball.pos, nextPos, intersectScreenX, intersectScreenY)) {
		ball.dir.y *= -1.0;
	}
	if (Intersects(ball.pos, nextPos, leftPaddle.pos, (vec2) { leftPaddle.pos.x, leftPaddle.pos.y + leftPaddle.size.y }, intersectPaddleX, intersectPaddleY)
	 || Intersects(ball.pos, nextPos, rightPaddle.pos, (vec2) { rightPaddle.pos.x, rightPaddle.pos.y + rightPaddle.size.y }, intersectPaddleX, intersectPaddleY)) {
		ball.dir.x *= -1.0;
		float angleOffset = (float) ((rand() % 20) - 10);
		ball.dir = rotate(ball.dir, angleOffset);
	}
	// TODO: Calculate ball dir considering screen and paddle intersections:
	// - screen > no speed up or down
	// - paddle > define rule for ball angle and speed changes
	ball.pos = add(ball.pos, mul(ball.dir, ball.speed * deltaTime));
}

void DrawEverything()
{
	ClearScreen(0x111111);
	DrawRectAnchorLeftBottom(leftPaddle.pos, leftPaddle.size, leftPaddle.color);
	DrawRectAnchorLeftBottom(rightPaddle.pos, rightPaddle.size, rightPaddle.color);
	// DrawCircle(ball.pos, ball.diameter, ball.color);  <-- waiting implementation
	DrawRectAnchorLeftBottom(ball.pos, (vec2) { ball.diameter, ball.diameter }, ball.color);
}