#ifndef SCREEN_H_
#define SCREEN_H_

#include <SDL2/SDL.h>

static const unsigned int SCREEN_WIDTH = 800;
static const unsigned int SCREEN_HEIGHT = 600;
extern SDL_Surface* screenSurface;
static unsigned int FPS = 60;

int getWidthMax(void);
int getHeightMax(void);
unsigned getFPS(void);

#endif