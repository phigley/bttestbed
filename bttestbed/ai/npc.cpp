//
//  npc.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npc.h"
#include "behavior.h"

#include <algorithm>

using namespace AI;

AI::NPC::NPC()
    : rootBehavior{ *this, Behavior::Ptr{new MoveAtVelocityBehavior{ *this, glm::vec2{-0.5, 0.1}} } }
{
    rootBehavior.initialize();
}

void AI::NPC::update(float dt)
{
    rootBehavior.update(dt);

    if( currentState )
        currentState->update(dt);
}