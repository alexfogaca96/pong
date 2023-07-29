#ifndef MATH_H
#define MATH_H

int Clamp(int min, int val, int max);

struct {
	union {
		struct {
			float x;
			float y;
		};

		float e[2];
	};
} typedef vec2;

#endif
