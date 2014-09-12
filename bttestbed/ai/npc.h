//
//  npc.h
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_npc_h
#define bttestbed_npc_h

#include "entity.h"
#include "state.h"
#include "behavior.h"

#include <memory>

namespace AI
{
    class NPC : public Entity
    {
    public:

        NPC();

        virtual void update(float dt) override;

    private:

        RootBehavior rootBehavior;
        
    };
}

#endif
