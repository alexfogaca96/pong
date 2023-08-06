#include "math.h"

#define PI 3.1415926535

int Clamp(int min, int val, int max)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

vec3 Cross2(vec2 a, vec2 b)
{
	float x = a.y - b.y;
	float y = b.x - a.x;
	float z = (a.x * b.y) - (a.y * b.x);
	return (vec3) { x, y, z };
}

vec3 Cross(vec3 a, vec3 b)
{
	float x = (a.y * b.z) - (a.z * b.y);
	float y = (b.x * a.z) - (a.x * b.z);
	float z = (a.x * b.y) - (a.y * b.x);
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

vec2 rotate(vec2 v, float eulerAngle)
{
	float rad = -eulerAngle * (PI / 180);
	float sin0 = 1;
	float* sinAngle = &sin0;
	float cosAngle = cosAndSin(rad, sinAngle);
	return (vec2) {
		round(10000 * (v.x * cosAngle - v.y * *sinAngle)) / 10000,
		round(10000 * (v.x * *sinAngle + v.y * cosAngle)) / 10000
	};
};

float sin(float x)
{
	float res = 0, pow = x, fact = 1;
	for (int i = 0; i < 5; ++i) {
		res += pow / fact;
		pow *= -1 * x * x;
		fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
	}
	return res;
}

float cos(float x)
{
	float sinX = sin(x);
	return sqrt(1.0 - sinX * sinX);
}

float cosAndSin(float x, float* sinX)
{
	*sinX = sin(x);
	return sqrt(1.0 - *sinX * *sinX);
}

float sqrt(float x)
{
	float z = 1.0;
	for (int i = 1; i <= 10; i++) {
		z -= (z * z - x) / (2 * z);
	}
	return z;
}

float round(float x)
{
	if (x < 0.0) {
		return (float) ((int)(x - 0.5));
	}
	return (float) ((int)(x + 0.5));
}