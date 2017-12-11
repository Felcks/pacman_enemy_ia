#ifndef PLAYER_H_
#define PLAYER_H_

#include "Map.h"
#include "Object.h"


typedef enum state {
	PREY, PREDATOR
} State;

typedef struct player{
	Object obj;
	Pos additionalPos;
	int moveDistance;
	int defaultMoveDistance;
	int currDistance;
	Direction dir;
	Direction nextDir;
	State state;
	int captureRange;
	int changeDirChanceDelay;
	int maxChangeDirChanceDelay;
	int pilulaTime;
	int isDead;
	int deadTime;
} *ptrPlayer;

ptrPlayer createPlayer(ptrMap m);
ptrPlayer createBlink(ptrMap m);
ptrPlayer createInky(ptrMap m);
ptrPlayer createPink(ptrMap m);
ptrPlayer createClyde(ptrMap m);
void movePlayer(ptrPlayer p, ptrMap m);
void moveEnemy(ptrPlayer p, ptrMap m);
int changeDirectionPlayer(ptrPlayer p, ptrMap m, Direction dir);
void randomEnemyMovement(ptrPlayer p, ptrMap m);
void drawPlayer(ptrPlayer p, ptrMap m, SDL_Renderer* renderer);
int updatePlayer(ptrPlayer p, ptrMap m, ptrPlayer enemies[4]);
void changeState(ptrPlayer p, State state, int index);
#endif