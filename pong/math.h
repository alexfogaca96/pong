#ifndef MATH_H
#define MATH_H

struct {
	union {
		struct {
			float x;
			float y;
		};

		float e[2];
	};
} typedef vec2;

#define ZERO_VEC2 (vec2) { 0.0, 0.0 }

struct {
	union {
		struct {
			float x;
			float y;
			float z;
		};

		float e[3];
	};
} typedef vec3;

#define ZERO_VEC3 (vec3) { 0.0, 0.0, 0.0 }


int Clamp(int min, int val, int max);
vec3 Cross2(vec2 a, vec2 b);
vec3 Cross(vec3 a, vec3 b);

#endif
