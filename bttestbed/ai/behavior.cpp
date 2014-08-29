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

AI::RootBehavior::~RootBehavior()
{
    if( activeChild < children.size() )
    {
        children[activeChild]->term();
    }
}

void AI::RootBehavior::update(float dt)
{
    Behavior::Result result = Behavior::Result::Fail;
    
    if( activeChild < children.size() )
    {
        result = children[activeChild]->update(dt);
    }
    else
    {
        for( activeChild = 0; result != Behavior::Result::Continue && activeChild < children.size(); ++activeChild )
        {
            result = children[activeChild]->initialize();
        }
    }
    
    switch( result )
    {
    case Behavior::Result::Fail :
    case Behavior::Result::Complete :
        if( activeChild < children.size() )
        {
            children[activeChild]->term();
            activeChild = std::size_t(-1);
        }
        break;
        
    case Behavior::Result::Continue :
        break;
    }
}

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
