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
    std::size_t handleResult(ActiveList& activePath, std::size_t currentIndex, Result result)
    {
        // Terminate everything below us.
        while( activePath.size() > currentIndex )
        {
            activePath.back().getBehavior().term();
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


Root::Root(NPC& npc_, rapidxml::xml_node<>& rootNode)
    : npc{npc_}
{
    assert(0 == strcmp(rootNode.name(), "root"));
    for( auto childNode = rootNode.first_node(); childNode; childNode = childNode->next_sibling() )
    {
        children.push_back( createChild(npc_, *childNode) );
    }
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
