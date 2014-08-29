//
//  npc.cpp
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#include "npc.h"

#include <algorithm>

namespace  {
  float wrapLimits(float value, float minValue, float maxValue)
  {
    if( value < minValue )
        return maxValue;
      
    if( value > maxValue )
        return minValue;
      
    return value;
  }
}

void NPC::update(float dt)
{
    const float velocity = 0.25f;
    
    const float projectedX = getX() + velocity*dt;
    const float projectedY = getY() + velocity*dt;
    
    const float newX = wrapLimits(projectedX, -1.0f, 1.0f);
    const float newY = wrapLimits(projectedY, -1.0f, 1.0f);
    
    moveTo(newX, newY);
}