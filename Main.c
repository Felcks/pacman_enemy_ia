#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "Screen.h"
#include "GameManager.h"
#include "Player.h"
#include "Map.h"
#include "Vector.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool isRunning = true;
ptrMap map = NULL;
ptrPlayer player = NULL;

int init();
void sdl_update();
void end();

void start(char* mapName);
void draw();
void update();

int main(int argc, char* argv[]){	

	if(!init()){
		printf("Failed to initialize!\n");
		return 0;
	}

	if(argc < 2){
		printf("Insira o nome do arquivo de mapa. Ex: game config.txt \n");
		return 0;
	}

	start(argv[1]);

	sdl_update();

	end();

	return 0;
}

int init(){
	int sucess = 1;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		sucess = 0;
	}
	else
	{
		window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED, getWidthMax(), 
									getHeightMax(), SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			sucess = 0;
		}
	}

	screenSurface = SDL_GetWindowSurface( window );
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
	SDL_UpdateWindowSurface( window );

	return sucess;
}

void sdl_update(){

	unsigned long long start;
	while(isRunning)
	{
		start = SDL_GetTicks();
		isRunning = run(player, map); // null por enquanto!
    	
    	update();
    	draw();

		if((1000u/getFPS()) > (SDL_GetTicks() - start))
			SDL_Delay(10);
    		//SDL_Delay((1000u/getFPS()) - (SDL_GetTicks() - start));
	}
}

void start(char* mapName){
	
	map = createMapFromFile(mapName);
	resizeMap(map);
	player = createPlayer(map);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 	
}

void draw(){

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE); 
	SDL_RenderClear(renderer); 
	

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	SDL_Rect rect;
	rect.w = map->rectSize;
	rect.h = map->rectSize;
	for(int i = 0; i < map->rows; i++){
		for(int j = 0; j < map->columns; j++){

			rect.x = j * rect.w + j + map->start_x ;
			rect.y = i * rect.h + i + map->start_y;
			if(map->matrix[i][j] == 1)
				SDL_RenderFillRect(renderer, &rect);
		}
	}

	drawPlayer(player, map, renderer);

	SDL_RenderPresent(renderer); 
}

void update(){

	updatePlayer(player, map);
}

void end(){

	//destroyMap(map);
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}