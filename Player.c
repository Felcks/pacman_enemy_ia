#include "Player.h"
#include "Screen.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define YELLOW SDL_MapRGB(screenSurface->format,0xFF,0xFF,0x00)

Object findFirstMapTile(ptrMap m){
	
	for(int i = 0; i < m->rows; i++){
		for(int j = 0; j < m->columns; j++){
			if(m->matrix[i][j] == GROUND){
				Vector4 v = { .row = i, .column = j};
				Object obj = { .pos = v };
				return obj;
			}
		}
	}

	Object obj;
	return obj;
}

ptrPlayer createPlayer(ptrMap m){

	ptrPlayer p = malloc(sizeof(ptrPlayer));
	p->obj = findFirstMapTile(m);

	Color color = { .r = 255, .g = 255, .b = 0, .a = 255};
	p->obj.color = color;

	Direction direction = { .horizontal = 1, .vertical = 0};
	p->dir = direction;
	p->nextDir = direction;

	Pos pos = { .x = 0.0, .y = 0.0};
	p->additionalPos = pos;

	p->moveDistance = 100; 
	p->currDistance = 0;

	return p;
}

void drawPlayer(ptrPlayer p, ptrMap m, SDL_Renderer* renderer){

	SDL_SetRenderDrawColor(renderer, p->obj.color.r, p->obj.color.g, p->obj.color.b, p->obj.color.a);
	int j = p->obj.pos.column;
	int i = p->obj.pos.row;

	SDL_Rect rect;
	rect.w = m->rectSize;
	rect.h = m->rectSize;
	rect.x = j * rect.w + j + m->start_x + (int)p->additionalPos.x;
	rect.y = i * rect.h + i + m->start_y + (int)p->additionalPos.y;

	SDL_RenderFillRect(renderer, &rect);
}

void updatePlayer(ptrPlayer p, ptrMap m){

	movePlayer(p, m);
}

void movePlayer(ptrPlayer p, ptrMap m){

	//Como eu vou fazer o movimento
	//Pega a posição do player e acrescenta rectsize/moveDistance
	p->additionalPos.x += (m->rectSize/(float)p->moveDistance) * p->dir.horizontal;
	p->additionalPos.y += (m->rectSize/(float)p->moveDistance) * p->dir.vertical;

	p->currDistance++;
	if(p->currDistance >= p->moveDistance){
		p->currDistance = 0;
		p->obj.pos.column += p->dir.horizontal;
		p->obj.pos.row += p->dir.vertical;
		p->additionalPos.x = 0;
		p->additionalPos.y = 0;
		//Seta nova direção
		p->dir = p->nextDir;

	}
}
//			  *
// [0] ----> [1] *
//			  *
void changeDirectionPlayer(ptrPlayer p, ptrMap m, Direction dir){

	Vector2 nextPos = { .row = p->obj.pos.row + p->dir.vertical, .column = p->obj.pos.column + p->dir.horizontal};
	
	if(m->matrix[nextPos.row + dir.vertical][nextPos.column + dir.horizontal] == GROUND){
		
		p->nextDir = dir;
	}
}