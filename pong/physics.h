#ifndef PHYSICS_H
#define PHYSICS_H

#include "math.h"

#include <sal.h>

unsigned int IntersectsScreenBoundUp(vec2 a0, vec2 a1, _Out_opt_ float* intersectX, _Out_opt_ float* intersectY);
unsigned int IntersectsScreenBoundDown(vec2 a0, vec2 a1, _Out_opt_ float* intersectX, _Out_opt_ float* intersectY);
unsigned int Intersects(vec2 a0, vec2 a1, vec2 b0, vec2 b1, _Out_opt_ float* intersectX, _Out_opt_ float* intersectY);

#endif
