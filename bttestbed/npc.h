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
#include "npcstate.h"

#include <memory>

class NPC : public Entity
{
public:

    virtual void update(float dt) override;

    template<typename T, typename... Args>
    void setState(Args&&... args)
    {
        currentState = std::unique_ptr<NPCState>{ new T{std::forward<Args>(args)...} };
    }

private:

    std::unique_ptr<NPCState> currentState;
};

#endif
