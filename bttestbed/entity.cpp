//
//  entity.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "entity.h"

#include <SDL2/SDL.h>

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

void Entity::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    SDL_Rect r;
	r.w = 10;
	r.h = 10;
	r.x = int((currentPos.x*0.5f + 0.5f)*float(windowWidth)) - (r.w/2);
	r.y = int((currentPos.y*0.5f + 0.5f)*float(windowHeight)) - (r.h/2);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
}