
#include "sdlapplication.h"
#include <exception>
#include <string>

SdlApplication::SdlApplication(int width, int height)
{
	// Initialize the SDL library.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
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

    if( !SDL_WaitEvent(&ev) )
    {
        return false;
    }
    
    return onEvent(&ev);
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
	}
    
    return true;
}

void SdlApplication::render()
{
	SDL_Rect r;
	int w,h;
	SDL_GetWindowSize(win, &w, &h);
	
	r.w = 200;
	r.h = 200;
	r.x = w/2-(r.w/2);
	r.y = h/2-(r.h/2);
	
	
	//
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0, 0xff);
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
	SDL_RenderPresent(renderer);
}

