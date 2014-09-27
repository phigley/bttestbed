//
//  npcstate.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbednpc_state_h
#define bttestbednpc_state_h

#include "result.h"

#include "glm/glm.hpp"

#include <algorithm>

namespace AI
{
    class NPC;

    namespace State
    {
        class MoveAtVelocity
        {
        public:

            MoveAtVelocity(NPC& npc_, const glm::vec2& velocity_)
                : npc{npc_}
                , velocity{velocity_}
            { }
            
            Result update(float dt);
            
        private :

            NPC& npc;
            glm::vec2 velocity;
        };
        
        class MoveTowardTarget
        {
        public:
        
            MoveTowardTarget(NPC& npc_, float speed_, float desiredRange_)
                : npc{npc_}
                , speed{speed_}
                , desiredRange{ std::max(desiredRange_, 0.0f) }
            { }
            
            Result update(float dt);

        private :

            NPC&        npc;
            float       speed;
            float       desiredRange;
        };
    }
    
}

#endif
