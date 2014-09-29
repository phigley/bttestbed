
#ifndef bttestbed_sdlapplication_h
#define bttestbed_sdlapplication_h

#include <SFML/Graphics.hpp>

#include "world.h"

class Entity;

class Application
{

public:

	Application(int width, int height);
	~Application();

    bool beginFrame();
	void render();


    World& getWorld() { return world; }
    const World& getWorld() const { return world; }
    
private:

	bool onEvent(const sf::Event& event);
	
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
        
    std::uint32_t currentTime = 0;
    
    World   world;
    
    sf::Clock           clock;
    sf::RenderWindow    window;
};

#endif