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

NPC::NPC()
    : rootBehavior
       { *this
        , std::make_shared<Behavior::Sequence>(*this
            , std::make_shared<Behavior::MoveTowardTarget>(*this, 0.75f, 0.01f)
            , std::make_shared<Behavior::Wait>(*this, 1.0f)
            )
        , std::make_shared<Behavior::MoveAtVelocity>(*this, glm::vec2{0.5f, 0.25f})
        }
{
}

void NPC::update(const World& world, float dt)
{
    targetPos = world.getTargetPos();
    
    rootBehavior.update(dt);
}