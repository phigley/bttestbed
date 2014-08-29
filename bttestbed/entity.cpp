//
//  entity.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "entity.h"

#include <SDL2/SDL.h>

void Entity::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    SDL_Rect r;
	r.w = 200;
	r.h = 200;
	r.x = windowWidth/2-(r.w/2);
	r.y = windowHeight/2-(r.h/2);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0xff);
	SDL_RenderFillRect(renderer, &r);
}