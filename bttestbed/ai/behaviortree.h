//
//  npcbehavior.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbednpc_behaviortree_h
#define bttestbednpc_behaviortree_h

#include "behavior.h"

#include "rapidxml/rapidxml.hpp"

#include <vector>

namespace AI
{
    class NPC;

    namespace Behavior
    {
       
        class Tree
        {
        public:

            Tree(NPC& npc_, rapidxml::xml_node<>& rootNode);
            
            void update(float dt);
            
        private :
        
			void replan();

            NPC&                    npc;
            std::vector<Base::Ptr>  children;
            ActiveList              activePath;
            float                   planningUpdateDuration = 0.0f;

        };
    }
}

#endif
