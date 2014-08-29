//
//  npc.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npc.h"

#include <algorithm>


void NPC::update(float dt)
{
    if( currentState )
        currentState->update(dt);
}