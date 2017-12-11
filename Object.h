#ifndef OBJECT_H_
#define OBJECT_H_

#include <SDL2/SDL.h>
#include "Vector.h"

typedef struct color{
	int r;
	int g;
	int b;
	int a;
} Color;

typedef struct object{
	Vector4 pos;
	Color color;
} Object;

#endif