//
//  entity.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "entity.h"

#include <SDL2/SDL.h>

void Entity::moveTo(float x, float y)
{
    currentX = x;
    currentY = y;
}

void Entity::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    SDL_Rect r;
	r.w = 10;
	r.h = 10;
	r.x = int((currentX*0.5f + 0.5f)*float(windowWidth)) - (r.w/2);
	r.y = int((currentY*0.5f + 0.5f)*float(windowHeight)) - (r.h/2);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
}