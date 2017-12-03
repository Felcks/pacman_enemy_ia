#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>
#include "Vector.h"

typedef struct color{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} Color;

typedef struct object{
	Vector4 pos;
	Color color;
} Object;

#endif