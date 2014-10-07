//
//  npcbehavior.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "behaviortree.h"
#include "npc.h"

#include "rapidxml/rapidxml.hpp"

using namespace AI;
using namespace AI::Behavior;

namespace
{
    void handleResult(ActiveList& activePath, std::size_t currentIndex, Result currentResult)
    {
        // Terminate everything below us.
        while( activePath.size() > currentIndex )
        {
            activePath.back().getBehavior().term();
            activePath.pop_back();
        }

        while( currentIndex > 0 )
        {
            const std::size_t parentIndex = currentIndex - 1;
            
            Base::PendingList pendingPath;
            const auto parentResult = currentResult == Result::Complete
                ? activePath[parentIndex].getBehavior().onChildCompleted(pendingPath)
                : activePath[parentIndex].getBehavior().onChildFailed(pendingPath);

            if( parentResult == Result::Continue )
            {
                while( !pendingPath.empty() )
                {
                    activePath.push_back( ActiveBehavior{*pendingPath.back()} );
                    pendingPath.pop_back();
                }
                
                return;
            }
        
            assert( pendingPath.empty() );

            // Pop the current node off the stack.
            activePath.back().getBehavior().term();
            activePath.pop_back();
            
            currentIndex  = parentIndex;
            currentResult = parentResult;
        }
        
        
        assert(activePath.empty());
    }
}


ActiveBehavior::ActiveBehavior(Base& behavior_)
    : requiresUpdate{behavior_.getRequiresUpdate()}
    , behavior{behavior_}
{
}


Tree::Tree(NPC& npc_, rapidxml::xml_node<>& rootNode)
    : npc{npc_}
{
    assert(0 == strcmp(rootNode.name(), "root"));
    for( auto childNode = rootNode.first_node(); childNode; childNode = childNode->next_sibling() )
    {
        children.push_back( createChild(npc_, *childNode) );
    }
}

void Tree::update(float dt)
{
    for( std::size_t currentIndex = 0; currentIndex < activePath.size(); ++currentIndex )
    {
        auto& currentChild = activePath[currentIndex];
        
        if( currentChild.getRequiresUpdate() )
        {
            const auto updateResult = currentChild.getBehavior().update(dt);
            
            if( updateResult != Result::Continue )
            {
                handleResult(activePath, currentIndex, updateResult);
                break;
            }
        }
    }

    const float planningUpdateDelay = 0.25f;
    
    planningUpdateDuration += dt;
    if( activePath.empty() || planningUpdateDuration > planningUpdateDelay )
    {
		replan();
    }

	assert(!activePath.empty());
}


void Tree::replan()
{
	planningUpdateDuration = 0.0f;

	const auto activeChildPtr = activePath.empty() ? nullptr : &activePath[0].getBehavior();

    for( std::size_t childIndex = 0; childIndex < children.size() && children[childIndex].get() != activeChildPtr; ++childIndex )
	{
        const auto& currentChild = children[childIndex];
        
		Base::PendingList pendingPath;
		const auto initializeResult = currentChild->initialize( pendingPath );
		if( initializeResult == Result::Continue )
		{
			// Terminate the old path.
			while( !activePath.empty() )
			{
				activePath.back().getBehavior().term();
				activePath.pop_back();
			}

			// Add the current child to the top of the active path.
			activePath.push_back( ActiveBehavior{ *currentChild } );

			// Add the rest of the path nodes to the active path.
			// The search pushed the nodes back in reverse order.
			while( !pendingPath.empty() )
			{
				activePath.push_back( ActiveBehavior{ *pendingPath.back() } );
				pendingPath.pop_back();
			}
			
			return;
		}
	}

    // Give the active path a chance to re-initialize.
    for( std::size_t activePathIndex = 0; activePathIndex < activePath.size(); ++activePathIndex )
    {
        Base::PendingList pendingPath;
        
        const auto activeChildIndex = activePathIndex + 1;
        const auto activeChildPtr = activeChildIndex < activePath.size()
            ? &activePath[activeChildIndex].getBehavior()
            : nullptr;
        
        activePath[activePathIndex].getBehavior().reinitialize( activeChildPtr, pendingPath );
        
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
                activePath.push_back( ActiveBehavior{ *pendingPath.back() } );
                pendingPath.pop_back();
            }
            
            return;
        }
    }
}