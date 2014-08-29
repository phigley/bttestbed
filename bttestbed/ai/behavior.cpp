//
//  npcbehavior.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "behavior.h"
#include "state.h"
#include "npc.h"

using namespace AI;

Behavior::Result AI::BehaviorSelector::initialize()
{
    for( auto& child : children )
    {
        const auto result = child->initialize();
        if( result == Result::Fail )
            continue;
        
        activeChild = child.get();
        return result;
    }
    
    return Result::Fail;
}

Behavior::Result AI::BehaviorSelector::update(float dt)
{
    if( activeChild )
        return activeChild->update(dt);
    
    return Result::Fail;
}

void BehaviorSelector::term()
{
    if( activeChild )
    {
        activeChild->term();
        activeChild = nullptr;
    }
}


Behavior::Result AI::MoveAtVelocityBehavior::initialize()
{
    getNPC().setState<MoveAtVelocity>(velocity);
    return Result::Continue;
}

Behavior::Result AI::MoveAtVelocityBehavior::update(float dt)
{
    return Result::Continue;
}

void AI::MoveAtVelocityBehavior::term()
{
    getNPC().clearState();
}
