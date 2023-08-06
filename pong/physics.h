#ifndef PHYSICS_H
#define PHYSICS_H

#include "math.h"

unsigned int IntersectsScreenBoundUp(vec2 a0, vec2 a1);
unsigned int IntersectsScreenBoundDown(vec2 a0, vec2 a1);
unsigned int Intersects(vec2 a0, vec2 a1, vec2 b0, vec2 b1);

#endif
