//
//  world.h
//  bttestbed
//
//  Created by Peter Higley on 9/13/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_world_h
#define bttestbed_world_h

#include "glm/glm.hpp"
#include "maybe.h"

#include <memory>
#include <vector>

class Entity;
namespace sf
{
  class RenderWindow;
}

class World
{

public:
    typedef std::shared_ptr<Entity> EntityOwnedPtr;

    typedef std::vector<EntityOwnedPtr> EntityContainer;
public:


    void update(float dt);

    void render(sf::RenderWindow& window, int windowWidth, int windowHeight);

    void setTargetPos(const glm::vec2& newPosition) { targetPos = newPosition; }
    void clearTargetPos()                           { targetPos = Maybe<glm::vec2>{}; }
    const Maybe<glm::vec2>& getTargetPos() const    { return targetPos; }
    
    template<typename T, typename... Args>
    std::weak_ptr<T> addEntity(Args&&... args)
    {
        auto newEntity = std::make_shared<T>(std::forward<Args>(args)...);
        entityPtrs.push_back(newEntity);
        
        return newEntity;
    }
    

private:

    EntityContainer entityPtrs;
    
    Maybe<glm::vec2> targetPos;

};

#endif
