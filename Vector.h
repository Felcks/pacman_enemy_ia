#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct vector2{
	int row;
	int column;
} Vector2;

typedef struct vector4{
	int row;
	int column;
	int width;
	int height;
} Vector4;

typedef struct pos{
	float x;
	float y;
} Pos;

typedef struct direction{
	int horizontal;
	int vertical;
} Direction;

#endif