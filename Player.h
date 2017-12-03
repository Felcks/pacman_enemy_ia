#ifndef PLAYER_H_
#define PLAYER_H_

#include "Map.h"
#include "Object.h"

typedef struct player{
	Object obj;
	Pos additionalPos;
	int moveDistance;
	int currDistance;
	Direction dir;
	Direction nextDir;
} *ptrPlayer;

ptrPlayer createPlayer();
void movePlayer(ptrPlayer p, ptrMap m);
void changeDirectionPlayer(ptrPlayer p, ptrMap m, Direction dir);
void drawPlayer(ptrPlayer p, ptrMap m, SDL_Renderer* renderer);
void updatePlayer(ptrPlayer p, ptrMap m);
#endif