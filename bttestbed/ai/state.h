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
        class Base
        {
        public:

            Base(NPC& npc_)
                : npc{npc_}
            { }
            
            virtual ~Base() { }
            
            virtual Result update(float dt) = 0;
            

            NPC& getNPC() { return npc; }
            const NPC& getNPC() const { return npc; }

        private:

            NPC& npc;
        };

        class MoveAtVelocity : public Base
        {
        public:

            MoveAtVelocity(NPC& npc_, const glm::vec2& velocity_)
                : Base{npc_}
                , velocity{velocity_}
            { }
            
            virtual Result update(float dt) override;
            
        private :

            glm::vec2 velocity;
        };
        
        class MoveTowardTarget : public Base
        {
        public:
        
            MoveTowardTarget(NPC& npc_, float speed_, float desiredRange_)
                : Base{npc_}
                , speed{speed_}
                , desiredRange{ std::max(desiredRange_, 0.0f) }
            { }
            
            virtual Result update(float dt) override;

        private :

            float       speed;
            float       desiredRange;
        };
    }
    
}

#endif
