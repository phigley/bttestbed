//
//  main.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include <iostream>
#include <memory>

#include "sdlapplication.h"
#include "world.h"
#include "ai/npc.h"
#include "ai/state.h"

int main(int argc, const char * argv[])
{
	SdlApplication app{800, 800};

    app.getWorld().addEntity<AI::NPC>();
    
    while( app.beginFrame() )
    {
        app.render();
    }

    return 0;
}

