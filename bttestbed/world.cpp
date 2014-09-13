//
//  world.cpp
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "world.h"

#include "entity.h"

void World::update(float dt)
{
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->update(dt);
    }
}

void World::render(SDL_Renderer* renderer, int windowWidth, int windowHeight)
{
    for(auto& entityPtr : entityPtrs)
    {
        entityPtr->render(renderer, windowWidth, windowHeight);
    }
}