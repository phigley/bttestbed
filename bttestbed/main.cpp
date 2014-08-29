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
#include "npc.h"
#include "npcstate.h"

int main(int argc, const char * argv[])
{
	SdlApplication app{800, 800};

    auto npcWeakPtr = app.addEntity<NPC>();

    {
        auto npcPtr = npcWeakPtr.lock();
        if( npcPtr )
        {
            npcPtr->setState<MoveAtVelocity>( glm::vec2{0.1, -0.25f} );
        }
    }
    
    while( app.beginFrame() )
    {
        app.render();
    }

    return 0;
}

