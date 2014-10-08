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
    void handleResult(ActiveList& activePath, const Base& current, Result result)
    {
        // Terminate everything below us.
        while( !activePath.empty() && &activePath.back().getBehavior() != &current )
        {
            activePath.back().getBehavior().term();
            activePath.pop_back();
        }

		// We should never actually fail with an empty path.
		assert( !activePath.empty() );

		// Terminate the child that returned this result.
		activePath.back().getBehavior().term();
		activePath.pop_back();

		Result currentResult = result;
        while( !activePath.empty() )
        {
			// Notify the parent.
			Base::PendingList pendingPath;
            const auto parentResult = currentResult == Result::Complete
                ? activePath.back().getBehavior().onChildCompleted(pendingPath)
				: activePath.back().getBehavior().onChildFailed( pendingPath );

			
			if( parentResult == Result::Continue )
            {
				// Parent has a new sub-plan, add it.
                while( !pendingPath.empty() )
                {
                    activePath.push_back( ActiveBehavior{*pendingPath.back()} );
                    pendingPath.pop_back();
                }
                
                return;
            }
        
			assert( pendingPath.empty() );

			// Terminate the parent behavior.
			activePath.back().getBehavior().term();
			activePath.pop_back();

			// Pass parent's result to its parent.
            currentResult = parentResult;
		}
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
	for( auto& currentChild : activePath )
    {
        if( currentChild.getRequiresUpdate() )
        {
            const auto updateResult = currentChild.getBehavior().update(dt);
            
            if( updateResult != Result::Continue )
            {
                handleResult(activePath, currentChild.getBehavior(), updateResult);
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

	const auto activeChildPtr = activePath.empty() ? nullptr : &activePath.front().getBehavior();

	for( const auto& currentChild : children )
	{
		if( currentChild.get() == activeChildPtr )
		{
			break;
		}


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
	for( auto iterPath = activePath.begin(); iterPath != activePath.end(); ++iterPath )
    {
		const auto iterChild = iterPath + 1;
		const auto activeChildPtr = iterChild != activePath.end()
			? &iterChild->getBehavior()
            : nullptr;
        
		auto& currentBehavior = iterPath->getBehavior();

		Base::PendingList pendingPath;
		currentBehavior.reinitialize( activeChildPtr, pendingPath );
        
        if( !pendingPath.empty() )
        {
            // Term the remaining behaviors on the active path.
            // Notice that this does not pop the behavior that
            // was just re-initialized.			
			while( !activePath.empty() && &activePath.back().getBehavior() != &currentBehavior )
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