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

namespace
{
    std::size_t handleResult(Base::ActiveList& activePath, std::size_t currentIndex, Result result)
    {
        // Terminate everything below us.
        while( activePath.size() > currentIndex )
        {
            // PLH TODO - Pass parent result into term function.
            activePath.back().getBehavior().term(/*result*/);
            activePath.pop_back();
        }

        // Nothing more to do if we emptied our path.
        if( currentIndex == 0 )
        {
            assert(activePath.empty());
            return 0;
        }

        const std::size_t parentIndex = currentIndex - 1;
        
        Base::PendingList pendingPath;
        const auto parentResult = result == Result::Complete
            ? activePath[parentIndex].getBehavior().onChildCompleted(pendingPath)
            : activePath[parentIndex].getBehavior().onChildFailed(pendingPath);

        if( parentResult == Result::Continue )
        {
            while( !pendingPath.empty() )
            {
                activePath.push_back( ActiveBehavior{*pendingPath.back()} );
                pendingPath.pop_back();
            }
            
            // Increment past the end of the activeBehaviors to exit the loop.
            return activePath.size();
        }
        
        assert( pendingPath.empty() );
        
        return handleResult(activePath, parentIndex, parentResult);
    }
}


ActiveBehavior::ActiveBehavior(Base& behavior_)
    : requiresUpdate{behavior_.getRequiresUpdate()}
    , behavior{behavior_}
{
}

void Root::update(float dt)
{
    const float planningUpdateDelay = 0.25f;
    
    planningUpdateDuration += dt;
    if( activePath.empty() || planningUpdateDuration > planningUpdateDelay )
    {
        planningUpdateDuration = 0.0f;
        
        const auto activeChildPtr = activePath.empty() ? nullptr : &activePath[0].getBehavior();
        
        for( const auto& currentChild : children )
        {
            if( currentChild.get() == activeChildPtr )
            {
                for( std::size_t activePathIndex = 0; activePathIndex < activePath.size(); ++activePathIndex )
                {
                    Base::PendingList pendingPath;
                    
                    const auto activeChildIndex = activePathIndex + 1;
                    const auto activeChildPtr = activeChildIndex < activePath.size()
                        ? &activePath[activeChildIndex].getBehavior()
                        : nullptr;
                    
                    activePath[activePathIndex].getBehavior().reinitialize(activeChildPtr, pendingPath);
                    
                    if( !pendingPath.empty() )
                    {
                        // Term the remaining behaviors on the acive path.
                        // Notice that this does not pop the behavior that
                        // was just re-initialized.
                        while( activePath.size() > activeChildIndex )
                        {
                            activePath.back().getBehavior().term();
                            activePath.pop_back();
                        }
                        
                        // Add the new behaviors to the active path.
                        while( !pendingPath.empty() )
                        {
                            activePath.push_back( ActiveBehavior{*pendingPath.back()} );
                            pendingPath.pop_back();
                        }

                        
                        break;
                    }
                }
                
                break;
            }
            
            Base::PendingList pendingPath;
            const auto initializeResult = currentChild->initialize(pendingPath);
            if( initializeResult == Result::Continue )
            {
                // Terminate the old path.
                while( !activePath.empty() )
                {
                    activePath.back().getBehavior().term();
                    activePath.pop_back();
                }
                
                // Add the current child to the top of the active path.
                activePath.push_back( ActiveBehavior{*currentChild} );
                
                // Add the rest of the path nodes to the active path.
                // The search pushed the nodes back in reverse order.
                while( !pendingPath.empty() )
                {
                    activePath.push_back( ActiveBehavior{*pendingPath.back()} );
                    pendingPath.pop_back();
                }
                break;
            }
        }
    }

    // We want to always have a valid behavior.
    assert(!activePath.empty());

    if( !activePath.empty() )
    {
        std::size_t currentIndex = 0;
        
        while( currentIndex < activePath.size() )
        {
            ActiveBehavior& currentChild = activePath[currentIndex];
            
            if( !currentChild.getRequiresUpdate() )
            {
                ++currentIndex;
                continue;
            }
            
            const auto updateResult = currentChild.getBehavior().update(dt);
            
            if( updateResult == Result::Continue )
            {
                ++currentIndex;
            }
            else
            {
                currentIndex = handleResult(activePath, currentIndex, updateResult);
            }
        }
    }
}


Result Priority::initialize(PendingList& pendingPath)
{
    bool allCompleted = true;
    for(std::size_t iChild = 0; iChild < children.size(); ++iChild)
    {
        const auto result = children[iChild]->initialize(pendingPath);
        if( result == Result::Continue )
        {
            pendingPath.push_back(children[iChild].get());
            return result;
        }

        allCompleted &= result == Result::Complete;
    }
    
    return allCompleted
        ? Result::Complete
        : Result::Fail;
}

void Priority::reinitialize(const Base* childPtr, PendingList& pendingPath)
{
    for(std::size_t iChild = 0; iChild < children.size() && children[iChild].get() != childPtr; ++iChild)
    {
        const auto result = children[iChild]->initialize(pendingPath);
        if( result == Result::Continue )
        {
            pendingPath.push_back(children[iChild].get());
            return;
        }

    }
}

Result Sequence::initialize(PendingList& pendingPath)
{
    activeChild = 0;
    return initializeNextChild(pendingPath);
}

Result Sequence::onChildCompleted(PendingList& pendingPath)
{
    ++activeChild;
    return initializeNextChild(pendingPath);
}

Result Sequence::initializeNextChild(PendingList& pendingPath)
{
    // Keep initializing the next child while they return Complete.
    for( ; activeChild < children.size(); ++activeChild )
    {
        const auto initializeResult = children[activeChild]->initialize(pendingPath);
        
        if( initializeResult != Result::Complete )
        {
            if( initializeResult == Result::Continue )
            {
                pendingPath.push_back(children[activeChild].get());
            }
            
            return initializeResult;
        }
    }

    return Result::Complete;
}

Result HasTarget::initialize(PendingList& pendingPath)
{
    if( !getNPC().getTargetPos() )
        return Result::Fail;
    
    if( maxDuration > 0.0f && getNPC().getTargetDuration() > maxDuration )
        return Result::Complete;

    const auto initializeResult = child->initialize(pendingPath);
    if( initializeResult == Result::Continue )
        pendingPath.push_back(child.get());
    
    return initializeResult;
}

Result HasTarget::update(float dt)
{
    if( !getNPC().getTargetPos() )
        return Result::Fail;
    
    if( maxDuration > 0.0f && getNPC().getTargetDuration() > maxDuration )
        return Result::Complete;
    
    return Result::Continue;
}

Result MoveAtVelocity::initialize(PendingList&)
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

Result MoveTowardTarget::initialize(PendingList&)
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

Result Wait::initialize(PendingList&)
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

Result ClearTarget::initialize(PendingList&)
{
    getNPC().clearTarget();
    return Result::Complete;
}


Result LockTarget::initialize(PendingList& pendingPath)
{
    const auto initializeResult = child->initialize(pendingPath);
    
    if( initializeResult == Result::Continue )
    {
        getNPC().lockTarget();
        pendingPath.push_back(child.get());
    }
    
    return initializeResult;
}

void LockTarget::term()
{
    getNPC().unlockTarget();
}
