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
        , std::make_shared<Behavior::HasTarget>(*this, 0.0f
            , std::make_shared<Behavior::Priority>(*this
                , std::make_shared<Behavior::HasTarget>(*this, 2.0f
                    , std::make_shared<Behavior::MoveTowardTarget>(*this, -0.1f, 10000.0f)
                    )
                , std::make_shared<Behavior::Sequence>(*this
                    , std::make_shared<Behavior::MoveTowardTarget>(*this, 0.75f, 0.01f)
                    , std::make_shared<Behavior::ClearTarget>(*this)
                    )
                )
            )
        , std::make_shared<Behavior::MoveAtVelocity>(*this, glm::vec2{0.5f, 0.25f})
        }
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
    
    rootBehavior.update(dt);
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

