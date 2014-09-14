//
//  npcstate.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_npcstate_h
#define bttestbed_npcstate_h

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

            Base(NPC& _npc)
                : npc{_npc}
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

            MoveAtVelocity(NPC& _npc, const glm::vec2& _velocity)
                : Base{_npc}
                , velocity{_velocity}
            { }
            
            virtual Result update(float dt) override;
            
        private :

            glm::vec2 velocity;
        };
        
        class MoveTowardTarget : public Base
        {
        public:
        
            MoveTowardTarget(NPC& _npc, float _speed, float _desiredRange)
                : Base{_npc}
                , speed{_speed}
                , desiredRange{ std::max(_desiredRange, 0.0f) }
            { }
            
            virtual Result update(float dt) override;

        private :

            float       speed;
            float       desiredRange;
        };
    }
    
}

#endif
