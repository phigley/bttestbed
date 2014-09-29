//
//  entity.h
//  bttestbed
//
//  Created by Peter Higley on 8/28/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_entity_h
#define bttestbed_entity_h

#include "glm/glm.hpp"

class World;
namespace sf
{
  class RenderWindow;
}

// PLH NOTE : This class is premature generalization.  Feel free to remove it and deal with derived types directly!

class Entity
{
public:

    Entity() { }
    
    virtual ~Entity() { }
    
    virtual void update(const World& world, float dt) = 0;
    virtual void render(sf::RenderWindow& window, int windowWidth, int windowHeight);

    const glm::vec2& getPos() const { return currentPos; }
    
    void moveTo(const glm::vec2& newPos);
    void moveBy(const glm::vec2& deltaPos);
    
private :

    glm::vec2 currentPos = glm::vec2{0};
};

#endif
