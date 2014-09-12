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
    
    for( std::size_t currentChild = 0; currentChild < activeChild; ++currentChild )
    {
        const auto initializeResult = children[currentChild]->initialize();
        if( initializeResult != Result::Fail )
        {
            if( activeChild < children.size() )
            {
                children[activeChild]->term();
            }
            
            dt = 0.0f;
            activeChild = currentChild;
            break;
        }
    }
    
    if( activeChild < children.size() )
    {
        const auto updateResult = children[activeChild]->update(dt);

        switch( updateResult )
        {
        case Result::Fail :
        case Result::Complete :
            if( activeChild < children.size() )
            {
                children[activeChild]->term();
                activeChild = std::size_t(-1);
            }
            break;
            
        case Result::Continue :
            break;
        }
    }
    
}

Result AI::BehaviorSelector::initialize()
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

Result AI::BehaviorSelector::update(float dt)
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


Result AI::MoveAtVelocityBehavior::initialize()
{
    state = std::unique_ptr<MoveAtVelocityState>(new MoveAtVelocityState(getNPC(), velocity));
    
    if( !state )
        return Result::Fail;
    
    return Result::Continue;
}

Result AI::MoveAtVelocityBehavior::update(float dt)
{
    if( !state )
        return Result::Fail;

    return state->update(dt);
}

void AI::MoveAtVelocityBehavior::term()
{
    state = nullptr;
}
