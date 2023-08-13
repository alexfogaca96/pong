#include "game.h"
#include "math.h"
#include "render.h"
#include "platform_common.h"
#include "physics.h"

#include <stdlib.h>
#include <string.h>

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


unsigned int started;
unsigned int paused;
Paddle leftPaddle, rightPaddle;
Ball ball;
unsigned int paddleHitCount;

void CreateGame() {
	started = 0;
	paused = 0;
	paddleHitCount = 0;
	
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
		.speed = 750.0f,
		.diameter = initialBallDiameter,
		.color = 0xffffff
	};

	// TODO: maybe change to an image (bitmap) and put in renderBuffer for simplicity
	// or learn how to properly do text (I think this is more complicated)
	char const* text = "Press any key!";
	Text clickToStartText = {
		.width = 200, .height = 50,
		.text = text, .textLength = strlen(text),
		.active = 1
	};
	uiTexts[CLICK_TO_START] = clickToStartText;

	char const* pauseText = "Game is paused.";
	Text gamePausedText = {
		.width = 200, .height = 50,
		.text = pauseText, .textLength = strlen(text),
		.active = 0
	};
	uiTexts[GAME_PAUSED] = gamePausedText;

	DrawEverything();
}

void SimulateGame(Input* input, float deltaTime)
{
	if (!started) {
		if ((Released(BUTTON_DOWN) || Released(BUTTON_UP) || Released(BUTTON_LEFT) || Released(BUTTON_RIGHT))) {
			started = 1;
			uiTexts[CLICK_TO_START].active = 0;
		}
		DrawEverything();
	}
	else {
		if (Released(BUTTON_PAUSE)) {
			paused ^= 1;
			uiTexts[GAME_PAUSED].active ^= 1;
		}
		if (!paused) {
			SimulatePaddles(input, deltaTime);
			if (!SimulateBall(deltaTime)) {
				CreateGame();
				return;
			}
			DrawEverything();
		}
		if (Released(BUTTON_RESTART)) CreateGame();
	}
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

unsigned int SimulateBall(float deltaTime)
{
	vec2 nextPos = add(ball.pos, mul(ball.dir, ball.speed * deltaTime));
	float sx = -1.0, sy = -1.0, px = -1.0, py = -1.0;
	float* intersectScreenX = &sx, * intersectScreenY = &sy, * intersectPaddleX = &px, *intersectPaddleY = &py;
	if (IntersectsScreenBoundLeft(ball.pos, nextPos, intersectScreenX, intersectScreenY) || IntersectsScreenBoundRight(ball.pos, nextPos, intersectScreenX, intersectScreenY)) {
		CreateGame();
		return 0;
	}

	if (IntersectsScreenBoundUp(ball.pos, (vec2) { nextPos.x, nextPos.y + ball.diameter } , intersectScreenX, intersectScreenY) || IntersectsScreenBoundDown(ball.pos, nextPos, intersectScreenX, intersectScreenY)) {
		ball.dir.y *= -1.0;
	}
	float paddleHitDivergence = (float) (rand() % 10);
	float paddleCornerHitDivergence = paddleHitDivergence + 10;
	#define IntersectPaddle(p,r) \
	if (Intersects(ball.pos, nextPos, p.pos, (vec2) { p.pos.x, p.pos.y + p.size.y }, intersectPaddleX, intersectPaddleY)) {\
		float partHit = (*intersectPaddleY - p.pos.y) / p.size.y;\
		if (partHit > 0.85) ball.dir = rotate(ball.dir, -paddleCornerHitDivergence * r);\
		else if (partHit < 0.15) ball.dir = rotate(ball.dir, paddleCornerHitDivergence * r);\
		else {\
			unsigned int revert = rand() % 2;\
			if (revert) ball.dir = rotate(ball.dir, -paddleHitDivergence * r);\
			else		ball.dir = rotate(ball.dir, paddleHitDivergence * r);\
		}\
	}
	IntersectPaddle(leftPaddle, -1);
	IntersectPaddle(rightPaddle, 1);
	if (*intersectPaddleX > 0 || *intersectPaddleY > 0) {
		ball.dir.x *= -1.0;
		ball.speed += 1;
		paddleHitCount++;
		if (paddleHitCount == 100) {
			leftPaddle.speed += 50;
			rightPaddle.speed += 50;
		}
	}
	ball.pos = add(ball.pos, mul(ball.dir, ball.speed * deltaTime)); 
	return 1;
}

void DrawEverything()
{
	ClearScreen(0x111111);
	DrawRectAnchorLeftBottom(leftPaddle.pos, leftPaddle.size, leftPaddle.color);
	DrawRectAnchorLeftBottom(rightPaddle.pos, rightPaddle.size, rightPaddle.color);
	DrawRectAnchorLeftBottom(ball.pos, (vec2) { ball.diameter, ball.diameter }, ball.color);
}