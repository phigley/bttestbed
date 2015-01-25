//
//  npc.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npc.h"
#include "behavior.h"
#include "world.h"

#include <algorithm>
#include <memory>

using namespace AI;

NPC::NPC(rapidxml::xml_node<>& behaviorDescription)
    : behaviorTree( *this, behaviorDescription )
{
}

void NPC::update(const World& world, float dt)
{
    if( canChangeTarget && world.getTargetPos() != oldTargetPos )
    {
        targetPos = world.getTargetPos();
        targetDuration = 0.0f;
        oldTargetPos = world.getTargetPos();
    }
    else
    {
        targetDuration += dt;
    }
    
    behaviorTree.update(dt);
}

void NPC::clearTarget()
{
    targetPos = Maybe<glm::vec2>{};
    targetDuration = 0.0f;
}

void NPC::lockTarget()
{
    canChangeTarget = false;
}

void NPC::unlockTarget()
{
    canChangeTarget = true;
}

