//
//  entity.h
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef entity_h
#define entity_h

class SDL_Renderer;

class Entity
{
public:

    Entity() { }
    
    virtual ~Entity() { }
    
    virtual void update(float dt) = 0;
    
    virtual void render(SDL_Renderer*, int windowWidth, int windowHeight);
    
    void moveTo(float x, float y);
    
    float getX() const { return currentX; }
    float getY() const { return currentY; }
    
private :

    float currentX = 0.0f;
    float currentY = 0.0f;
};

#endif
