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

Result Priority::initialize()
{
    assert(activeChild >= children.size());
    
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

Result Priority::update(float dt)
{
    for( std::size_t currentChild = 0; currentChild < activeChild && currentChild < children.size(); ++currentChild )
    {
        const auto initializeResult = children[currentChild]->initialize();
        if( initializeResult == Result::Continue )
        {
            if( activeChild < children.size() )
            {
                children[activeChild]->term();
            }
            
            activeChild = currentChild;
            break;
        }
    }

    if( activeChild < children.size() )
    {
        const auto updateResult = children[activeChild]->update(dt);

        if( updateResult != Result::Continue )
        {
            children[activeChild]->term();
            activeChild = std::size_t(-1);
        }

        return updateResult;
    }
    
    return Result::Fail;
}

void Priority::term()
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
    
    const auto targetDistance = glm::distance(*getNPC().getTargetPos(), getNPC().getPos());

    if( speed > 0.0f )
    {
        if( targetDistance <= desiredRange )
            return Result::Complete;
    }
    else
    {
        if( targetDistance >= desiredRange )
            return Result::Complete;
    }
    
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

Result Wait::initialize()
{
    if( duration <= 0.0f )
        return Result::Complete;
    
    timeRemaining = duration;
    return Result::Continue;
}


Result Wait::update(float dt)
{
    timeRemaining -= dt;
    
    if( timeRemaining <= 0.0f )
        return Result::Complete;
    
    return Result::Continue;
}


