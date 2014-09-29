
#include "sdlapplication.h"

#include "entity.h"

#include <exception>
#include <string>

SdlApplication::SdlApplication(int width, int height)
    : window{ sf::VideoMode(width, height), "BT Test Bed" }
{
    window.setFramerateLimit(30);
}

SdlApplication::~SdlApplication()
{
}

bool SdlApplication::beginFrame()
{
    sf::Event event;
    while( window.pollEvent(event) )
    {
        if( !onEvent(event) )
            return false;
    }
    
    const auto deltaTime = clock.restart();

    world.update(deltaTime.asSeconds());
    
    return true;
}

bool SdlApplication::onEvent(const sf::Event& event)
{
	switch( event.type )
	{
		case sf::Event::Closed :
			return false;
			
		case sf::Event::KeyPressed :
		{
			if( event.key.code == sf::Keyboard::Escape )
			{
				return false;
			}
		}
        break;
        
        case sf::Event::MouseButtonPressed :
        {
            if( event.mouseButton.button == sf::Mouse::Left )
            {
                const auto windowSize = window.getSize();
                
                const auto pos = glm::vec2
                    { float(event.mouseButton.x)*2.0f/float(windowSize.x) - 1.0f
                    , 1.0f - float(event.mouseButton.y)*2.0f/float(windowSize.y)
                    };
                
                
                world.setTargetPos(pos);
            }
            else if( event.mouseButton.button == sf::Mouse::Right )
            {
                world.clearTargetPos();
            }
        }
        break;
        
        default:
            break;
    }
    
    return true;
}

void SdlApplication::render()
{
    window.clear();

    const auto windowSize = window.getSize();
    world.render(window, windowSize.x, windowSize.y);
    
    window.display();
}


