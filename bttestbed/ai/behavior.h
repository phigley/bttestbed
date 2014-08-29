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

namespace AI
{
    class NPC;

    class Behavior
    {
    public:

        enum class Result
        {
            Continue,
            Complete,
            Fail
        };
        
        typedef std::shared_ptr<Behavior> NPCBehaviorPtr;
        
    public:

        Behavior(NPC& _npc)
            : npc{_npc}
        { }
        
        virtual ~Behavior() { }

        virtual Result initialize() { return Result::Continue; }
        virtual Result update(float dt) { return Result::Continue; }
        virtual void term() { }
        
        NPC& getNPC() { return npc; }
        const NPC& getNPC() const { return npc; }

    private:

        NPC& npc;
    };


    class BehaviorSelector : public Behavior
    {
    public:

        template<typename... Args>
        BehaviorSelector(NPC& _npc, Args&&... _children)
            : Behavior{_npc}
            , children{ std::forward<Args>(_children)... }
        {
        }
        
        virtual Result initialize() override;
        virtual Result update(float dt) override;
        virtual void term() override;
        
    private :

        std::vector<NPCBehaviorPtr> children;
        Behavior*                activeChild = nullptr;
    };

    class MoveAtVelocityBehavior : public Behavior
    {
    public:

        MoveAtVelocityBehavior(NPC& _npc, const glm::vec2& _velocity)
            : Behavior{_npc}
            , velocity{_velocity}
        {
        }
        
        virtual Result initialize() override;
        virtual Result update(float dt) override;
        virtual void term() override;

    private:

        glm::vec2 velocity;

    };
}

#endif
