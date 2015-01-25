//
//  world.cpp
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "world.h"

#include "entity.h"

#include <SFML/Graphics.hpp>

void World::update(float dt)
{
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->update(*this, dt);
    }
}

void World::render(sf::RenderWindow& window, int windowWidth, int windowHeight)
{
    if( targetPos )
    {
        const auto targetSize = 3.0f;
        
        auto targetShape = sf::RectangleShape( sf::Vector2f(3.0f, 3.0f) );
        
        const auto drawPos = sf::Vector2f
            { (targetPos->x*0.5f + 0.5f)*float(windowWidth) - targetSize*0.5f
            , (-targetPos->y*0.5f + 0.5f)*float(windowHeight) - targetSize*0.5f
            };
        
        targetShape.setPosition(drawPos);
        targetShape.setFillColor(sf::Color::White);
        
        window.draw(targetShape);
    }
    
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->render(window, windowWidth, windowHeight);
    }
}