//
//  npcstate.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npcstate.h"
#include "npc.h"

void MoveDown::update(float dt)
{
    const float velocity = 0.2f;
    
    getNPC().moveBy( glm::vec2{0.0f, -velocity*dt} );
}