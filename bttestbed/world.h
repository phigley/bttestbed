//
//  world.h
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_world_h
#define bttestbed_world_h

#include <memory>
#include <vector>

class Entity;
class SDL_Renderer;

class World
{

public:
    typedef std::shared_ptr<Entity> EntityOwnedPtr;

    typedef std::vector<EntityOwnedPtr> EntityContainer;
public:


    void update(float dt);

    void render(SDL_Renderer* renderer, int windowWidth, int windowHeight);


    template<typename T, typename... Args>
    std::weak_ptr<T> addEntity(Args&&... args)
    {
        auto newEntity = std::make_shared<T>(std::forward<Args>(args)...);
        entityPtrs.push_back(newEntity);
        
        return newEntity;
    }
    

private:

    EntityContainer entityPtrs;

};

#endif
