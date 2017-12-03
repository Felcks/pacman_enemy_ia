#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum mapType {
	GROUND = 0, WALL = 1
} MapType;

typedef struct map{
	int rows;
	int columns;
	int** matrix;
	int start_x;
	int start_y;
	int rectSize;
} *ptrMap;

typedef struct map Map;

ptrMap createMapFromFile(const unsigned char* name);
void resizeMap(ptrMap map);
bool printMap(const ptrMap map);
bool destroyMap(ptrMap map);

#endif