#include <SDL2/SDL.h>

#include <memory>
#include <vector>

class Entity;

class SdlApplication
{
public:
    typedef std::unique_ptr<Entity> EntityPtr;
    
public:

	SdlApplication(int width, int height);
	~SdlApplication();

    bool beginFrame();
	void render();

    template<typename T, typename... Args>
    void addEntity(Args&&... args)
    {
        entityPtrs.emplace_back(new T{std::forward<Args>(args)...});
    }
    
private:

	// Called to process SDL event.
	bool onEvent(SDL_Event* ev);
	
    SdlApplication(const SdlApplication&) = delete;
    SdlApplication& operator=(const SdlApplication&) = delete;
    
    
    std::vector<EntityPtr> entityPtrs;
    
    std::uint32_t currentTime = 0;
    
	// Whether the application is in event loop.
	bool quit = false;
	SDL_Window* win = nullptr;
	SDL_Renderer* renderer = nullptr;
};

