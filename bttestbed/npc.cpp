//
//  npc.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npc.h"
#include "npcbehavior.h"

#include <algorithm>



NPC::NPC()
    : rootBehavior{ *this, NPCBehavior::NPCBehaviorPtr{new MoveAtVelocityBehavior{ *this, glm::vec2{-0.5, 0.1}} } }
{
    rootBehavior.initialize();
}

void NPC::update(float dt)
{
    rootBehavior.update(dt);

    if( currentState )
        currentState->update(dt);
}