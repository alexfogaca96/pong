#ifndef GAME_H
#define GAME_H

#include "platform_common.h"

void CreateGame();
void SimulateGame(Input* input, float deltaTime);
void SimulatePaddles(Input* input, float deltaTime);
unsigned int SimulateBall(float deltaTime);
void DrawEverything();

#endif
