//
//  world.cpp
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "world.h"

#include "entity.h"

#include <SDL2/SDL.h>

void World::update(float dt)
{
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->update(*this, dt);
    }
}

void World::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    if( targetPos )
    {
        SDL_Rect r;
        r.w = 3;
        r.h = 3;
        r.x = int((targetPos->x*0.5f + 0.5f)*float(windowWidth)) - (r.w/2);
        r.y = int((-targetPos->y*0.5f + 0.5f)*float(windowHeight)) - (r.h/2);

        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderFillRect(renderer, &r);
    }
    
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->render(renderer, windowWidth, windowHeight);
    }
}