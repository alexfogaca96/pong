#include "math.h"
#include "render.h"

#include <sal.h>
#include <stdio.h>

unsigned int IntersectsScreenBoundUp(vec2 a0, vec2 a1, float* intersectX, float* intersectY)
{
    vec2 screenLimUp0 = { 0.0, (float) renderBuffer.height };
    vec2 screenLimUp1 = { (float) renderBuffer.width, (float) renderBuffer.height };
    a0 = normByMultiplier(a0);
    a1 = normByMultiplier(a1);
    return Intersects(a0, a1, screenLimUp0, screenLimUp1, intersectX, intersectY);
}

unsigned int IntersectsScreenBoundDown(vec2 a0, vec2 a1, float* intersectX, float* intersectY)
{
    vec2 screenLimDown0 = { 0.0, 0.0 };
    vec2 screenLimDown1 = { (float) renderBuffer.width, 0.0 };
    a0 = normByMultiplier(a0);
    a1 = normByMultiplier(a1);
    return Intersects(a0, a1, screenLimDown0, screenLimDown1, intersectX, intersectY);
}

unsigned int Intersects(vec2 a0, vec2 a1, vec2 b0, vec2 b1, float* intersectX, float* intersectY)
{
    vec2 s1 = { a1.x - a0.x, a1.y - a0.y };
    vec2 s2 = { b1.x - b0.x, b1.y - b0.y };
    float s = (-s1.y * (a0.x - b0.x) + s1.x * (a0.y - b0.y)) / (-s2.x * s1.y + s1.x * s2.y);
    float t = (s2.x * (a0.y - b0.y) - s2.y * (a0.x - b0.x)) / (-s2.x * s1.y + s1.x * s2.y);
    if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
        if (intersectX != NULL) *intersectX = a0.x + (t * s1.x);
        if (intersectY != NULL) *intersectY = a0.y + (t * s1.y);
        return 1;
    }
    return 0;
}
