//
//  npcstate.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npcstate.h"
#include "npc.h"

void MoveAtVelocity::update(float dt)
{
    getNPC().moveBy( velocity*dt );
}