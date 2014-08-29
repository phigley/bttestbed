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

NPCBehavior::Result BehaviorSelector::initialize()
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

NPCBehavior::Result BehaviorSelector::update(float dt)
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


NPCBehavior::Result MoveAtVelocityBehavior::initialize()
{
    getNPC().setState<MoveAtVelocity>(velocity);
    return Result::Continue;
}

NPCBehavior::Result MoveAtVelocityBehavior::update(float dt)
{
    return Result::Continue;
}

void MoveAtVelocityBehavior::term()
{
    getNPC().clearState();
}
