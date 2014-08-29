//
//  npcstate.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_npcstate_h
#define bttestbed_npcstate_h

class NPC;

class NPCState
{
public:

    NPCState(NPC& _npc)
        : npc(_npc)
    { }
    
    virtual ~NPCState() { }
    
    virtual void update(float dt) = 0;
    

    NPC& getNPC() { return npc; }
    const NPC& getNPC() const { return npc; }

private:

    NPC& npc;
};

class MoveDown : public NPCState
{
public:

    MoveDown(NPC& _npc)
        : NPCState(_npc)
    { }
    
    virtual void update(float dt) override;
};

#endif
