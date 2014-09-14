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

using namespace AI;

AI::NPC::NPC()
    : rootBehavior{ Behavior::Base::Ptr{new Behavior::MoveTowardTarget{ *this, 0.75, 0.01} } }
{
}

void AI::NPC::update(const World& world, float dt)
{
    targetPos = world.getTargetPos();
    
    rootBehavior.update(dt);
}