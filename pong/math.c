#include "math.h"

int Clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

vec3 Cross2(vec2 a, vec2 b)
{
	int x = a.y - b.y;
	int y = b.x - a.x;
	int z = (a.x * b.y) - (a.y * b.x);
	return (vec3) { x, y, z };
}

vec3 Cross(vec3 a, vec3 b)
{
	int x = (a.y * b.z) - (a.z * b.y);
	int y = (b.x * a.z) - (a.x * b.z);
	int z = (a.x * b.y) - (a.y * b.x);
	return (vec3) { x, y, z };
}

vec2 mul(vec2 v, float n)
{
	return (vec2) { v.x * n, v.y * n };
}

vec2 add(vec2 a, vec2 b)
{
	return (vec2) { a.x + b.x, a.y + b.y };
}