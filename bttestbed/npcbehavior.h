//
//  npcbehavior.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_npcbehavior_h
#define bttestbed_npcbehavior_h

#include <glm/glm.hpp>
#include <vector>

class NPC;

class NPCBehavior
{
public:

    enum class Result
    {
        Continue,
        Complete,
        Fail
    };
    
    typedef std::shared_ptr<NPCBehavior> NPCBehaviorPtr;
    
public:

    NPCBehavior(NPC& _npc)
        : npc{_npc}
    { }
    
    virtual ~NPCBehavior() { }

    virtual Result initialize() { return Result::Continue; }
    virtual Result update(float dt) { return Result::Continue; }
    virtual void term() { }
    
    NPC& getNPC() { return npc; }
    const NPC& getNPC() const { return npc; }

private:

    NPC& npc;
};


class BehaviorSelector : public NPCBehavior
{
public:

    template<typename... Args>
    BehaviorSelector(NPC& _npc, Args&&... _children)
        : NPCBehavior{_npc}
        , children{ std::forward<Args>(_children)... }
    {
    }
    
    virtual Result initialize() override;
    virtual Result update(float dt) override;
    virtual void term() override;
    
private :

    std::vector<NPCBehaviorPtr> children;
    NPCBehavior*                activeChild = nullptr;
};

class MoveAtVelocityBehavior : public NPCBehavior
{
public:

    MoveAtVelocityBehavior(NPC& _npc, const glm::vec2& _velocity)
        : NPCBehavior{_npc}
        , velocity{_velocity}
    {
    }
    
    virtual Result initialize() override;
    virtual Result update(float dt) override;
    virtual void term() override;

private:

    glm::vec2 velocity;

};

#endif
