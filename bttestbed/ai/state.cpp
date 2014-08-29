//
//  npcstate.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "state.h"
#include "npc.h"

using namespace AI;

void AI::MoveAtVelocity::update(float dt)
{
    getNPC().moveBy( velocity*dt );
}