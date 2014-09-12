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

namespace AI
{
    class NPC;

    class State
    {
    public:

        State(NPC& _npc)
            : npc(_npc)
        { }
        
        virtual ~State() { }
        
        virtual Result update(float dt) = 0;
        

        NPC& getNPC() { return npc; }
        const NPC& getNPC() const { return npc; }

    private:

        NPC& npc;
    };

    class MoveAtVelocityState : public State
    {
    public:

        MoveAtVelocityState(NPC& _npc, const glm::vec2& _velocity)
            : State(_npc)
            , velocity(_velocity)
        { }
        
        virtual Result update(float dt) override;
        
    private :

        glm::vec2 velocity;
    };
    
    
}

#endif
