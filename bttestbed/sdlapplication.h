#include <SDL2/SDL.h>

class SdlApplication
{
public:

	SdlApplication(int width, int height);
	~SdlApplication();

    bool beginFrame();
	void render();

private:

	// Called to process SDL event.
	bool onEvent(SDL_Event* ev);
	
    SdlApplication(const SdlApplication&) = delete;
    SdlApplication& operator=(const SdlApplication&) = delete;
    
    
	// Whether the application is in event loop.
	bool quit = false;
	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
};

