
#include "sdlapplication.h"

#include "entity.h"

#include <exception>
#include <string>

SdlApplication::SdlApplication(int width, int height)
{
	// Initialize the SDL library.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
        std::string msg = "SDL_INIT() failed: ";
        msg += SDL_GetError();
        throw std::runtime_error(msg);
	}
	
    const char apptitle[] = "SDL Sample Application";
	win = SDL_CreateWindow(apptitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SdlApplication::~SdlApplication()
{
	if (win)
	{
		SDL_DestroyWindow(win);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}
}

bool SdlApplication::beginFrame()
{
	// Enter to the SDL event loop.
	SDL_Event ev;
    while( SDL_PollEvent(&ev) != 0 )
    {
        if( !onEvent(&ev) )
            return false;
    }
    
    
    const std::uint32_t newTime = SDL_GetTicks();
    
    const float deltaTime = float(newTime - currentTime)*1e-3f;
    currentTime = newTime;
    
    world.update(deltaTime);
    
    return true;
}

bool SdlApplication::onEvent(SDL_Event* ev)
{
	switch (ev->type)
	{
		case SDL_QUIT:
			return false;
			break;
			
		case SDL_KEYDOWN:
		{
			if (ev->key.keysym.sym == SDLK_ESCAPE)
			{
				return false;
			}
		}
        break;
    }
    
    return true;
}

void SdlApplication::render()
{
	int w,h;
	SDL_GetWindowSize(win, &w, &h);
    
	//
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0, 0xff);
	SDL_RenderClear(renderer);
	
    world.render(renderer, w, h);
    
	SDL_RenderPresent(renderer);
    SDL_Delay(10);
}


