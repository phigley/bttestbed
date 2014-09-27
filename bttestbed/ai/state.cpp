//
//  npcstate.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "state.h"
#include "npc.h"

using namespace AI;
using namespace AI::State;

Result MoveAtVelocity::update(float dt)
{
    npc.moveBy( velocity*dt );
    
    return Result::Continue;
}


Result MoveTowardTarget::update(float dt)
{
    const auto& targetPos = npc.getTargetPos();
    if( !targetPos )
        return Result::Fail;
    
    const auto deltaPos = *targetPos - npc.getPos();
    const auto deltaDistance = glm::length(deltaPos);
    
    if( speed > 0.0f )
    {
        if( deltaDistance <= desiredRange )
            return Result::Complete;
    }
    else
    {
        if( deltaDistance >= desiredRange )
            return Result::Complete;
    }
    
    const auto scaleAmount = deltaDistance > speed*dt
        ? speed*dt/deltaDistance
        : 1.0f;
    
    const auto moveAmount = deltaPos * scaleAmount;
    
    npc.moveBy( moveAmount );
    
    return Result::Continue;
}