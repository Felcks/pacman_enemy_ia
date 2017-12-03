#include "Map.h"
#include "Screen.h"
#include <stdio.h>
#include <stdlib.h>

#define minimum(x,y) x < y?x:y;

ptrMap createMapFromFile(const unsigned char* name){

	ptrMap map = NULL;
	FILE* inputFile = NULL;
	unsigned char *buffer;
	unsigned i,j;
	int column;
	int row;
	int linesRead;
	int temp;

	buffer = (unsigned char*)calloc(1000,sizeof(char));
	if(buffer == NULL)
		return NULL;

	map = (ptrMap)malloc(sizeof(Map));
	if(map == NULL)
		return NULL;

	inputFile = fopen(name,"r");
	if(name == NULL)
	{
		puts("Failed to open file");
		return NULL;
	}

	  fscanf(inputFile,"%s",buffer);

    row = atoi(buffer);
    fscanf(inputFile,"%s",buffer);
    column = atoi(buffer);

    map->columns = column;
    map->rows = row;
    linesRead = row;
    map->matrix = (int**)calloc(row,sizeof(int*));
    if(map->matrix == NULL)
    	return NULL;
    for(i = 0; i < row; i++)
    {
    	map->matrix[i] = (int*)calloc(column,sizeof(int));
    	if(map->matrix[i] == NULL)
    		return NULL;
    }
    
    for(i = 0; i < row; i++)
    {
    	for(j = 0; j < column; j++)
    	{
    		fscanf(inputFile,"%s",buffer);
    		temp = atoi(buffer);
    		map->matrix[i][j] = temp;
    	}
    }
   
	fclose(inputFile);

	return map;
}

void resizeMap(ptrMap map)
{	
	map->rectSize = minimum(SCREEN_WIDTH/map->columns, SCREEN_HEIGHT/map->rows);
	map->start_x = (SCREEN_WIDTH - (map->rectSize*map->columns))/2;
	map->start_y = (SCREEN_HEIGHT - (map->rectSize*map->rows))/2;
	map->rectSize -= 1;
	int a = minimum(SCREEN_WIDTH/map->columns, SCREEN_HEIGHT/map->rows);	
	printf("%i\n", map->rectSize );
}

