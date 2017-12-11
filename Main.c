#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "Screen.h"
#include "GameManager.h"
#include "Player.h"
#include "Map.h"
#include "Vector.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font;
bool isRunning = true;
ptrMap map = NULL;
ptrPlayer player = NULL;
ptrPlayer blink = NULL;


ptrPlayer enemies[4];
ptrGameManager gameManager;

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

	TTF_Init();
	screenSurface = SDL_GetWindowSurface( window );
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
	SDL_UpdateWindowSurface( window );

	return sucess;
}

void sdl_update(){

	unsigned long long l_start;
	while(isRunning)
	{
		l_start = SDL_GetTicks();
		isRunning = run(player, map); // null por enquanto!
    	
    	update();
    	draw();

		if((1000u/getFPS()) > (SDL_GetTicks() - l_start)){
			//SDL_Delay(10);
    		SDL_Delay((1000u/getFPS()) - (SDL_GetTicks() - l_start));
    		gameManager->score++;
    	}
	}
}

SDL_Rect scoreRect;
SDL_Texture* message = NULL;

void createFont(){

	font = TTF_OpenFont("arial.ttf", 12); //this opens a font style and sets a size
	SDL_Color fColor ;
	 fColor.r = 255;
       fColor.g = 255;
        fColor.b = 255;
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "put your text here", fColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

	message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

	 //create a rect
	scoreRect.x = 0;  //controls the rect's x coordinate 
	scoreRect.y = 0; // controls the rect's y coordinte
	scoreRect.w = 0; // controls the width of the rect
	scoreRect.h = 0; // controls the height of the rect
}

void start(char* mapName){
	
	map = createMapFromFile(mapName);
	resizeMap(map);
	player = createPlayer(map);
	blink = createBlink(map);

	enemies[0] = blink;
	enemies[1] = blink;
	enemies[2] = blink;
	enemies[3] = blink;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 

	gameManager = malloc(sizeof(struct gameManager));
	gameManager->maxLifes = 3;
	gameManager->life = 3;
	gameManager->score = 0;	
	//createFont();
}

void draw(){

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE); 
	SDL_RenderClear(renderer); 
	

	SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);

	SDL_Rect rect;
	rect.w = map->rectSize;
	rect.h = map->rectSize;
	for(int i = 0; i < map->rows; i++){
		for(int j = 0; j < map->columns; j++){

			rect.x = j * rect.w + j + map->start_x ;
			rect.y = i * rect.h + i + map->start_y;
			if(map->matrix[i][j] == 1){
				SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			if(map->matrix[i][j] == 2){
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}

		}
	}

	drawPlayer(player, map, renderer);
	drawPlayer(blink, map, renderer);

	//SDL_RenderCopy(renderer, message, NULL, &scoreRect);

	SDL_RenderPresent(renderer); 
}

void update(){

	updateEnemy(blink, map);
	int gameOver = updatePlayer(player, map, enemies);

	if(gameOver == 1){
		SDL_Delay(1500);
		gameManager->life--;
		
		if(gameManager->life <= 0){
			isRunning = false;			
		}

		player = createPlayer(map);
		blink = createBlink(map);

		enemies[0] = blink;
		enemies[1] = blink;
		enemies[2] = blink;
		enemies[3] = blink;
	}
}	

void end(){

	//destroyMap(map);
	SDL_FreeSurface(screenSurface);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}