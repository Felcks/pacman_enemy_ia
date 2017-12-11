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

SDL_Rect scoreRect;
SDL_Rect lifeRect;
SDL_Texture* messageScore = NULL;
SDL_Texture* messageLifes = NULL;
SDL_Texture* valueScore = NULL;
static char valueScoreText[10] = {0};
static char valueLifesText[10] = {0};
static const char* textScore = "Score: ";
static const char* textLifes = "Lifes: ";

int init();
void sdl_update();
void end();

void start(char* mapName);
void draw();
void update();
void drawText(void);
void render_text(SDL_Renderer *renderer, int x, int y,const char *text, TTF_Font *font, SDL_Rect *rect,  SDL_Color *color);

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
    		gameManager->score += 0.1;
    	}
	}
}

void createFont(){

	font = TTF_OpenFont("arial.ttf", 17); //this opens a font style and sets a size
	
	int w,h;
	SDL_Color fColor ;
	fColor.r = 255;
    fColor.g = 255;
    fColor.b = 255;
    SDL_Surface* surfacemessageScore = TTF_RenderText_Solid(font,textScore, fColor); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    SDL_Surface* surfacemessageLifes = TTF_RenderText_Solid(font,textLifes, fColor);

	messageScore = SDL_CreateTextureFromSurface(renderer, surfacemessageScore); //now you can convert it into a texture
	messageLifes = SDL_CreateTextureFromSurface(renderer, surfacemessageLifes);

	TTF_SizeText(font,textScore,&w,&h);
	
	scoreRect.x = 10;  
	scoreRect.y = 10; 
	scoreRect.w = w; 
	scoreRect.h = h;

	TTF_SizeText(font,textLifes,&w,&h);
	lifeRect.x = 10;
	lifeRect.y = 30;
	lifeRect.w = w;
	lifeRect.h = h;
}

void render_text(SDL_Renderer *renderer, int x, int y,const char *text, TTF_Font *font, SDL_Rect *rect,  SDL_Color *color) 
{
	SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->x = x;
    rect->y = y;
    rect->w = surface->w;
    rect->h = surface->h;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
}
void drawText(void){

	SDL_Rect rectScore;
	SDL_Rect rectLifes;
	int w,h;
	SDL_Color fColor ;
	fColor.r = 255;
    fColor.g = 255;
    fColor.b = 255;

    TTF_SizeText(font,textScore,&w,&h);
    itoa((int)gameManager->score,valueScoreText,10);
    render_text(renderer,w+10,10,  valueScoreText,font,&rectScore,&fColor);

    TTF_SizeText(font,textLifes,&w,&h);
    itoa(gameManager->life,valueLifesText,10);
    render_text(renderer,w+10,30,valueLifesText,font,&rectLifes,&fColor);
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
	gameManager->score = 0.0;
	gameManager->scoreStrike = 1;	
	createFont();
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
	drawText();
	SDL_RenderCopy(renderer, messageScore, NULL, &scoreRect);
	SDL_RenderCopy(renderer, messageLifes, NULL, &lifeRect);
	//SDL_RenderCopy(renderer, message, NULL, &scoreRect);

	SDL_RenderPresent(renderer); 
}

void update(){

	updateEnemy(blink, map);
	int resultPlayer = updatePlayer(player, map, enemies);

	if(player->pilulaTime > 0){

		for(int i = 0; i < 4; i++)
			changeState(enemies[i], PREY, i);
	}
	else{

		gameManager->scoreStrike = 1;
		for(int i = 0; i < 4; i++)
			changeState(enemies[i], PREDATOR, i);
	}

	if(resultPlayer == 1){ //game Over
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
	else if(resultPlayer == 2){ //Comeu um cara

		gameManager->score += pow(200, gameManager->scoreStrike);
		gameManager->scoreStrike++;
	}
}	

void end(){

	//destroyMap(map);;
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();
}