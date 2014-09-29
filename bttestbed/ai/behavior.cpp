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

#include "rapidxml/rapidxml.hpp"

using namespace AI;
using namespace AI::Behavior;

Base::Ptr AI::Behavior::createChild(NPC& npc, rapidxml::xml_node<>& node)
{
    if( strcmp(node.name(), "priority") == 0 )
        return Base::Ptr{ new Priority{npc, node} };
    if( strcmp(node.name(), "sequence") == 0 )
        return Base::Ptr{ new Sequence{npc, node} };
    if( strcmp(node.name(), "hastarget") == 0 )
        return Base::Ptr{ new HasTarget{npc, node} };
    if( strcmp(node.name(), "moveatvelocity") == 0 )
        return Base::Ptr{ new MoveAtVelocity{npc, node} };
    if( strcmp(node.name(), "movetowardtarget") == 0 )
        return Base::Ptr{ new MoveTowardTarget{npc, node} };
    if( strcmp(node.name(), "wait") == 0 )
        return Base::Ptr{ new Wait{npc, node} };
    if( strcmp(node.name(), "cleartarget") == 0 )
        return Base::Ptr{ new ClearTarget{npc, node} };
    if( strcmp(node.name(), "locktarget") == 0 )
        return Base::Ptr{ new LockTarget{npc, node} };
    
    return Base::Ptr{};
}

Priority::Priority(NPC& npc_, rapidxml::xml_node<>& priorityNode)
    : Base{npc_, false}
{
    for( auto childNode = priorityNode.first_node(); childNode; childNode = childNode->next_sibling() )
    {
        children.push_back( createChild(npc_, *childNode) );
        assert(children.back().get() != nullptr);
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

Sequence::Sequence(NPC& npc_, rapidxml::xml_node<>& sequenceNode)
    : Base{npc_, false}
{
    for( auto childNode = sequenceNode.first_node(); childNode; childNode = childNode->next_sibling() )
    {
        children.push_back( createChild(npc_, *childNode) );
       assert(children.back().get() != nullptr);
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

Conditional::Conditional(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, true}
{
    auto childNode = xmlNode.first_node();
    assert(childNode);
    
    if( childNode )
    {
        child = createChild(npc_, *childNode);
        assert(childNode->next_sibling() == nullptr);
    }
}

Result Conditional::initialize(PendingList& pendingPath)
{
    if( !setUp() )
        return Result::Fail;
    
    if( !isValid() )
    {
        tearDown();
        return Result::Fail;
    }
    
    if( isComplete() )
    {
        tearDown();
        return Result::Complete;
    }
    
    const auto initializeResult = child->initialize(pendingPath);
    if( initializeResult != Result::Continue )
    {
        tearDown();
        return initializeResult;
    }
    
    pendingPath.push_back(child.get());
    
    return Result::Continue;
}

Result Conditional::update(float dt)
{
    if( !isValid() )
        return Result::Fail;
    
    if( isComplete() )
        return Result::Complete;
    
    return Result::Continue;
}

void Conditional::term()
{
    tearDown();
}

HasTarget::HasTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Conditional{npc_, xmlNode}
    , maxDuration{-1.0f}
{
    for( auto attribute = xmlNode.first_attribute(); attribute; attribute = attribute->next_attribute())
    {
        if( 0 == strcmp(attribute->name(), "maxduration") )
        {
            maxDuration = atof(attribute->value());
        }
        else
        {
            assert(false);
        }
    }
}

bool HasTarget::isValid() const
{
    if( !getNPC().getTargetPos() )
        return false;
    
    return true;
}

bool HasTarget::isComplete() const
{
    if( maxDuration > 0.0f && getNPC().getTargetDuration() > maxDuration )
        return true;
    
    return false;
}

MoveAtVelocity::MoveAtVelocity(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, true}
    , velocity{0,0}
{
    for( auto attribute = xmlNode.first_attribute(); attribute; attribute = attribute->next_attribute())
    {
        if( 0 == strcmp(attribute->name(), "velocity_x") )
        {
            velocity.x = atof(attribute->value());
        }
        else if( 0 == strcmp(attribute->name(), "velocity_y") )
        {
            velocity.y = atof(attribute->value());
        }
        else
        {
            assert(false);
        }
     }
    

    assert(xmlNode.first_node() == nullptr);
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


MoveTowardTarget::MoveTowardTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, true}
    , speed{0}
    , desiredRange{0}
{
    for( auto attribute = xmlNode.first_attribute(); attribute; attribute = attribute->next_attribute())
    {
        if( 0 == strcmp(attribute->name(), "speed") )
        {
            speed = atof(attribute->value());
        }
        else if( 0 == strcmp(attribute->name(), "desiredrange") )
        {
            desiredRange = atof(attribute->value());
        }
        else
        {
            assert(false);
        }

    }

    assert(xmlNode.first_node() == nullptr);
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

Wait::Wait(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, true}
    , duration{0}
{
    for( auto attribute = xmlNode.first_attribute(); attribute; attribute = attribute->next_attribute())
    {
        if( 0 == strcmp(attribute->name(), "duration") )
        {
            duration = atof(attribute->value());
        }
        else
        {
            assert(false);
        }
    }

    assert(xmlNode.first_node() == nullptr);
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

ClearTarget::ClearTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, false}
{
    assert(xmlNode.first_node() == nullptr);
}

Result ClearTarget::initialize(PendingList&)
{
    getNPC().clearTarget();
    return Result::Complete;
}

Decorator::Decorator(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Base{npc_, false}
{
    auto childNodePtr = xmlNode.first_node();
    assert(childNodePtr != nullptr);
    if( childNodePtr != nullptr )
    {
        child = createChild(npc_, *childNodePtr);
        assert(childNodePtr->next_sibling() == nullptr);
    }
}

Result Decorator::initialize(PendingList& pendingPath)
{
    if( !setUp() )
        return Result::Fail;
        
    const auto initializeResult = child->initialize(pendingPath);
    
    if( initializeResult != Result::Continue )
    {
        tearDown();
        return initializeResult;
    }
    
    pendingPath.push_back(child.get());
    return Result::Continue;
}

void Decorator::term()
{
    tearDown();
}

LockTarget::LockTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode)
    : Decorator{npc_, xmlNode}
{
}

bool LockTarget::setUp()
{
    getNPC().lockTarget();
    return true;
}

void LockTarget::tearDown()
{
    getNPC().unlockTarget();
}
