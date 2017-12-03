#include "Screen.h"

SDL_Surface* screenSurface = NULL;

int getWidthMax(void)
{
	return SCREEN_WIDTH;
}

int getHeightMax(void)
{
	return SCREEN_HEIGHT;
}

unsigned getFPS(void){
	return FPS;
}