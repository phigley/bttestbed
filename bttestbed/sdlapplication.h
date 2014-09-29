
#ifndef bttestbed_sdlapplication_h
#define bttestbed_sdlapplication_h

#include <SFML/Graphics.hpp>

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
	bool onEvent(const sf::Event& event);
	
    SdlApplication(const SdlApplication&) = delete;
    SdlApplication& operator=(const SdlApplication&) = delete;
        
    std::uint32_t currentTime = 0;
    
    World   world;
    
    sf::Clock           clock;
    sf::RenderWindow    window;
};

#endif