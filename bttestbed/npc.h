//
//  npc.h
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef npc_h
#define npc_h

#include "entity.h"

class NPC : public Entity
{
public:

    virtual void update(float dt) override;

};

#endif
