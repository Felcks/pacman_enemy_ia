#include "Player.h"
#include "Screen.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define YELLOW SDL_MapRGB(screenSurface->format,0xFF,0xFF,0x00)

Color preyColor = { .r = 0, .g = 0, .b = 255};
Color blinkColor = { .r = 255, .g = 0, .b = 0};
Color pinkColor = { .r = 200, .g = 0, .b = 50};



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

	srand(time(NULL)); 

	ptrPlayer p = malloc(sizeof(struct player));
	p->obj = findFirstMapTile(m);

	Color color = { .r = 255, .g = 255, .b = 0, .a = 255};
	p->obj.color = color;

	Direction direction = { .horizontal = 1, .vertical = 0};
	p->dir = direction;
	p->nextDir = direction;

	Pos pos = { .x = 0.0, .y = 0.0};
	p->additionalPos = pos;

	p->moveDistance = 10; 
	p->currDistance = 0;

	return p;
}

ptrPlayer createBlink(ptrMap m){

	ptrPlayer p = malloc(sizeof(struct player));
	Vector4 v = { .row = 62, .column = 30};
	Object obj = { .pos = v };
	p->obj = obj;

	Color color = { .r = 255, .g = 0, .b = 0, .a = 255};
	p->obj.color = color;

	Direction direction = { .horizontal = -1, .vertical = 0};
	p->dir = direction;
	p->nextDir = direction;

	Pos pos = { .x = 0.0, .y = 0.0};
	p->additionalPos = pos;

	p->moveDistance = 14; 
	p->currDistance = 0;

	p->state = PREDATOR;
	p->captureRange = 10;
	p->maxChangeDirChanceDelay = 50;
	p->changeDirChanceDelay = 0;
	return p;
}



void drawPlayer(ptrPlayer p, ptrMap m, SDL_Renderer* renderer){

	SDL_SetRenderDrawColor(renderer, p->obj.color.r,  p->obj.color.g, p->obj.color.b, p->obj.color.a);
	int j = p->obj.pos.column;
	int i = p->obj.pos.row;

	SDL_Rect rect;
	rect.w = m->rectSize;
	rect.h = m->rectSize;
	rect.x = j * rect.w + j + m->start_x + (int)p->additionalPos.x;
	rect.y = i * rect.h + i + m->start_y + (int)p->additionalPos.y;

	SDL_RenderFillRect(renderer, &rect);
}

int updatePlayer(ptrPlayer p, ptrMap m, ptrPlayer enemies[4]){

	movePlayer(p, m);

	for(int i = 0; i < 4; i++){

		if(abs(p->obj.pos.row - enemies[i]->obj.pos.row) <= 1 &&
			abs(p->obj.pos.column - enemies[i]->obj.pos.column) <= 1){
			//TODO conferir se ta com pilula ou nao
			//Resetar tudo
			return 1;
		}
	}


	if(m->matrix[p->obj.pos.row][p->obj.pos.column] == 2){
		m->matrix[p->obj.pos.row][p->obj.pos.column] = 0;
	}

	return 0;
}

void updateEnemy(ptrPlayer p, ptrMap m){

	moveEnemy(p, m);
	
	if(p->state == PREDATOR){
		randomEnemyMovement(p, m);
	}
	else if(p->state == PREY){
		randomEnemyMovement(p, m);
	}


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

		if(m->matrix[p->obj.pos.row + p->dir.vertical][p->obj.pos.column + p->dir.horizontal] == WALL){
			//procurar novo lugar para ir 
			tryChangeDirections(p, m, p->dir);
		}
	}
}

void moveEnemy(ptrPlayer p, ptrMap m){

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

		if(m->matrix[p->obj.pos.row + p->dir.vertical][p->obj.pos.column + p->dir.horizontal] == WALL){
			//procurar novo lugar para ir 
			tryChangeDirections(p, m, p->dir);
		}
		
	}
}

void tryChangeDirections(ptrPlayer p, ptrMap m, Direction currDir){

	Direction inverseDir = {.horizontal = currDir.horizontal * -1, .vertical = currDir.vertical * -1};
	Direction dir = {.horizontal = 1, .vertical = 0};
	Direction allDir[] = { {.horizontal = 1, .vertical = 0},
							{.horizontal = 0, .vertical = 1},
							{.horizontal = -1, .vertical = 0},
							{.horizontal = 0, .vertical = -1}, };

	int i = 0;
	int sucess = 0;
	dir = allDir[i];

	while(i < 4){

		//A direcao inversa e o ultimo recurso e sabemos que vai dar certo, logo nao fazemos a tentativa;
		if(dir.horizontal == inverseDir.horizontal && dir.vertical == inverseDir.vertical){ }
		else{
			sucess = changeDirectionPlayerCollision(p, m, dir);
			if(sucess == 1)
				break;
		}

		i++;
		dir = allDir[i];
	}


	if(sucess == 0){
		changeDirectionPlayerCollision(p, m, inverseDir);
	}
}

int changeDirectionPlayerCollision(ptrPlayer p, ptrMap m, Direction dir){

	Vector2 nextPos = { .row = p->obj.pos.row, .column = p->obj.pos.column};
	
	if(m->matrix[nextPos.row + dir.vertical][nextPos.column + dir.horizontal] == GROUND){
		
		p->dir = dir;
		p->nextDir = dir;
		return 1;
	}

	return 0;
}

int changeDirectionPlayer(ptrPlayer p, ptrMap m, Direction dir){

	Vector2 nextPos = { .row = p->obj.pos.row + p->dir.vertical, .column = p->obj.pos.column + p->dir.horizontal};
	
	if(m->matrix[nextPos.row + dir.vertical][nextPos.column + dir.horizontal] == GROUND){
		
		p->nextDir = dir;
		return 1;
	}

	return 0;
}

void randomEnemyMovement(ptrPlayer p, ptrMap m){

	p->changeDirChanceDelay++;

	if(p->changeDirChanceDelay >= p->maxChangeDirChanceDelay){
		
		p->changeDirChanceDelay = 0;
		Direction allDir[] = { {.horizontal = 1, .vertical = 0},
						{.horizontal = 0, .vertical = 1},
						{.horizontal = -1, .vertical = 0},
						{.horizontal = 0, .vertical = -1}, };

		int sortedDirId = rand() % 4;
		Direction dir = allDir[sortedDirId];

		changeDirectionPlayer(p, m, dir);
	}
}

