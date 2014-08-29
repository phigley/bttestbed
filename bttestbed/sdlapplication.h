
#ifndef bttestbed_sdlapplication_h
#define bttestbed_sdlapplication_h

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

class Entity;

class SdlApplication
{
public:
    typedef std::shared_ptr<Entity> EntityOwnedPtr;

public:

	SdlApplication(int width, int height);
	~SdlApplication();

    bool beginFrame();
	void render();

    template<typename T, typename... Args>
    std::weak_ptr<T> addEntity(Args&&... args)
    {
        auto newEntity = std::make_shared<T>(std::forward<Args>(args)...);
        entityPtrs.push_back(newEntity);
        
        return newEntity;
    }
    
private:

	// Called to process SDL event.
	bool onEvent(SDL_Event* ev);
	
    SdlApplication(const SdlApplication&) = delete;
    SdlApplication& operator=(const SdlApplication&) = delete;
    
    
    std::vector<EntityOwnedPtr> entityPtrs;
    
    std::uint32_t currentTime = 0;
    
	// Whether the application is in event loop.
	bool quit = false;
	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
};

#endif