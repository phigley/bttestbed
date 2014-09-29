//
//  entity.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "entity.h"

#include <SFML/Graphics.hpp>

namespace
{
  float wrapLimits(float value, float minValue, float maxValue)
  {
    if( value < minValue )
        return maxValue;
      
    if( value > maxValue )
        return minValue;
      
    return value;
  }
}

void Entity::moveTo(const glm::vec2& newPos)
{
    currentPos = newPos;
    
    currentPos.x = wrapLimits(currentPos.x, -1.0f, 1.0f);
    currentPos.y = wrapLimits(currentPos.y, -1.0f, 1.0f);
}

void Entity::moveBy(const glm::vec2& deltaPos)
{
    moveTo(currentPos + deltaPos);
}

void Entity::render(sf::RenderWindow& window, int windowWidth, int windowHeight)
{
    const auto size = 10.0f;
    
    auto drawShape = sf::CircleShape{size};
    
    const auto position = sf::Vector2f
        { (currentPos.x*0.5f + 0.5f)*float(windowWidth) - size*0.5f
        , (-currentPos.y*0.5f + 0.5f)*float(windowHeight) - size*0.5f
        };
        
    drawShape.setPosition(position);
    drawShape.setFillColor( sf::Color{0xff, 0xff, 0, 0xff} );

    window.draw(drawShape);
}