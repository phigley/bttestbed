//
//  npcbehavior.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbednpc_behavior_h
#define bttestbednpc_behavior_h

#include "result.h"
#include "state.h"

#include "glm/glm.hpp"
#include <vector>
#include <memory>
#include <limits>

namespace AI
{
    class NPC;

    namespace Behavior
    {
        class Base
        {
        public:

            
            typedef std::shared_ptr<Base> Ptr;
            
        public:

            Base(NPC& npc_)
                : npc{npc_}
            { }
            
            virtual ~Base() { }

            // Called to set-up the behavior.
            virtual Result initialize() { return Result::Continue; }
            
            // Called if initialize returned Result::Continue.
            virtual Result update(float dt) { return Result::Continue; }
            
            // Called if and only if initialize returned Result::Continue.
            virtual void term() { }
            
            NPC& getNPC() { return npc; }
            const NPC& getNPC() const { return npc; }

        private:

            NPC& npc;
        };

        class Priority : public Base
        {
        public:

            template<typename... Args>
            Priority(NPC& npc_, Args&&... children_)
                : Base{npc_}
                , children{ std::forward<Args>(children_)... }
            {
            }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override;
            virtual void term() override;
            
        private :

            std::vector<Ptr>    children;
            std::size_t         activeChild = std::numeric_limits<std::size_t>::max();
        };


        class Sequence : public Base
        {
        public:
            template<typename... Args>
            Sequence(NPC& npc_, Args&&... children_)
                : Base{npc_}
                , children{ std::forward<Args>(children_)... }
            {
            }
        
            virtual Result initialize() override;
            virtual Result update(float dt) override;
            virtual void term() override;
            
        private :

            Result initializeNextChild();

            std::vector<Ptr>    children;
            std::size_t         activeChild = std::numeric_limits<std::size_t>::max();
            
        };

        class HasTarget : public Base
        {
        public:
        
            HasTarget(NPC& npc_, Ptr&& child_)
                : Base{npc_}
                , child(child_)
            { }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override { return child->update(dt); }
            virtual void term() override { child->term(); }
            
        private:
        
            Ptr child;
        };

        class MoveAtVelocity : public Base
        {
        public:

            MoveAtVelocity(NPC& npc_, const glm::vec2& velocity_)
                : Base{npc_}
                , velocity{velocity_}
            {
            }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override;
            virtual void term() override;

        private:

            glm::vec2 velocity;

            std::unique_ptr<State::MoveAtVelocity> state;
        };
        
        class MoveTowardTarget : public Base
        {
        public:
        
            MoveTowardTarget(NPC& npc_, float speed_, float desiredRange_)
                : Base{npc_}
                , speed{speed_}
                , desiredRange(desiredRange_)
            {
            }

            virtual Result initialize() override;
            virtual Result update(float dt) override;
            virtual void term() override;
            
        private:
        
            float   speed;
            float   desiredRange;
            
            std::unique_ptr<State::MoveTowardTarget> state;
        };
        
        class Wait : public Base
        {
        public:
        
            Wait(NPC& npc_, float duration_)
                : Base{npc_}
                , duration{duration_}
            { }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override;

        private:
        
            float duration;
            
            float timeRemaining = 0.0f;
        };
        
        class ClearTarget : public Base
        {
        public:
        
            ClearTarget(NPC& npc_)
                : Base{npc_}
            { }
            
            virtual Result initialize() override;
        };
    }
}

#endif
