
#ifndef bttestbed_sdlapplication_h
#define bttestbed_sdlapplication_h

#include <SDL2/SDL.h>

#include "world.h"

class Entity;

class SdlApplication
{

public:

	SdlApplication(int width, int height);
	~SdlApplication();

    bool beginFrame();
	void render();


    World& getWorld() { return world; }
    const World& getWorld() const { return world; }
    
private:

	// Called to process SDL event.
	bool onEvent(SDL_Event* ev);
	
    SdlApplication(const SdlApplication&) = delete;
    SdlApplication& operator=(const SdlApplication&) = delete;
        
    std::uint32_t currentTime = 0;
    
    World   world;
    
	// Whether the application is in event loop.
	bool quit = false;
	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
};

#endif