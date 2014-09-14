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
using namespace AI::Behavior;

Root::~Root()
{
    if( activeChild < children.size() )
    {
        children[activeChild]->term();
    }
}

void Root::update(float dt)
{
    
    for( std::size_t currentChild = 0; currentChild < activeChild && currentChild < children.size(); ++currentChild )
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

Result PrioritySelector::initialize()
{
    for(std::size_t iChild = 0; iChild < children.size(); ++iChild)
    {
        const auto result = children[iChild]->initialize();
        if( result == Result::Fail )
            continue;
        
        activeChild = iChild;
        return result;
    }
    
    return Result::Fail;
}

Result PrioritySelector::update(float dt)
{
    if( activeChild < children.size())
        return children[activeChild]->update(dt);
    
    return Result::Fail;
}

void PrioritySelector::term()
{
    if( activeChild < children.size())
    {
        children[activeChild]->term();
        activeChild = std::numeric_limits<std::size_t>::max();
    }
}


Result MoveAtVelocity::initialize()
{
    state = std::unique_ptr<State::MoveAtVelocity>(new State::MoveAtVelocity{getNPC(), velocity});
    
    if( !state )
        return Result::Fail;
    
    return Result::Continue;
}

Result MoveAtVelocity::update(float dt)
{
    if( !state )
        return Result::Fail;

    return state->update(dt);
}

void MoveAtVelocity::term()
{
    state = nullptr;
}

Result MoveTowardTarget::initialize()
{
    if( !getNPC().getTargetPos())
        return Result::Fail;
        
    state = std::unique_ptr<State::MoveTowardTarget>(new State::MoveTowardTarget{getNPC(), speed, desiredRange});
    
    if( !state )
        return Result::Fail;
    
    return Result::Continue;
}

Result MoveTowardTarget::update(float dt)
{
    if( !state )
        return Result::Fail;

    return state->update(dt);
}

void MoveTowardTarget::term()
{
    state = nullptr;
}
