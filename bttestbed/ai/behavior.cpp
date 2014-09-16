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


Result Sequence::initialize()
{
    activeChild = 0;
    return initializeNextChild();
}

Result Sequence::update(float dt)
{
    if( activeChild < children.size() )
    {
        const auto updateResult = children[activeChild]->update(dt);
    
        switch(updateResult)
        {
            case Result::Continue:
                return Result::Continue;
            
            case Result::Fail:
                children[activeChild]->term();
                return Result::Fail;
                
            case Result::Complete:
            {
                children[activeChild]->term();
                
                ++activeChild;
                return initializeNextChild();
            }
        }
        
        
        return updateResult;
    }
    
    return Result::Fail;
}

Result Sequence::initializeNextChild()
{
    while( activeChild < children.size() )
    {
        const auto initializeResult = children[activeChild]->initialize();
        
        if( initializeResult != Result::Complete )
            return initializeResult;

        activeChild += 1;
    }

    return Result::Complete;
}

void Sequence::term()
{
    if( activeChild < children.size())
    {
        children[activeChild]->term();
    }
}

Result HasTarget::initialize()
{
    if( getNPC().getTargetPos() )
        return child->initialize();
    
    return Result::Fail;
}

Result HasTarget::update(float dt)
{
    if( getNPC().getTargetPos() )
        return child->update(dt);
    
    return Result::Fail;
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

Result ClearTarget::initialize()
{
    getNPC().clearTarget();
    return Result::Complete;
}


Result LockTarget::initialize()
{
    return child->initialize();
}

Result LockTarget::update(float dt)
{
    if( !appliedLock )
    {
        getNPC().lockTarget();
        appliedLock = true;
    }
    
    return child->update(dt);
}

void LockTarget::term()
{
    child->term();
    
    if( appliedLock )
    {
        getNPC().unlockTarget();
        appliedLock = false;
    }
}
