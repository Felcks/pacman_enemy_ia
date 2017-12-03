#include "GameManager.h"
#include "Player.h"
#include "Map.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

#define getPlayer(Obj) ((ptrPlayer)Obj)
#define getMap(Obj) ((ptrMap)Obj)

bool run(void* parameters, void* map)
{
	SDL_Event event;
	int quit = 0;

	ptrPlayer p = getPlayer(parameters);
	ptrMap m = getMap(map);

	Direction down = { .horizontal = 0, .vertical = 1};
	Direction up = { .horizontal = 0, .vertical = -1};
	Direction right = { .horizontal = 1, .vertical = 0};
	Direction left = { .horizontal = -1, .vertical = 0};

	while(SDL_PollEvent(&event) != 0)
	{
		if(event.type == SDL_Quit)
		{
			return false;
		}
		switch(event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
		            case SDLK_ESCAPE:    
		              	return false;
		                break;

		            case SDLK_RETURN:
		           		break;

		           	case SDLK_DOWN:
		         		changeDirectionPlayer(p, m, down);
		           		break;
		           	case SDLK_UP:
		         		changeDirectionPlayer(p, m, up);
		           		break;
		           	case SDLK_RIGHT:
		         		changeDirectionPlayer(p, m, right);
		           		break;
		           	case SDLK_LEFT:
		         		changeDirectionPlayer(p, m, left);
		           		break;
				}     				
		}
	}	

	return true;
}