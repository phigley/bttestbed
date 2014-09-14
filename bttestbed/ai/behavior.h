//
//  npcbehavior.h
//  bttestbed
//
//  Created by Peter Higley on 8/29/14.
//  Copyright (c) 2014 Peter Higley. All rights reserved.
//

#ifndef bttestbed_npcbehavior_h
#define bttestbed_npcbehavior_h

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

            Base(NPC& _npc)
                : npc{_npc}
            { }
            
            virtual ~Base() { }

            virtual Result initialize() { return Result::Continue; }
            virtual Result update(float dt) { return Result::Continue; }
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
            Priority(NPC& _npc, Args&&... _children)
                : Base{_npc}
                , children{ std::forward<Args>(_children)... }
            {
            }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override;
            virtual void term() override;
            
        private :

            std::vector<Ptr>    children;
            std::size_t         activeChild = std::numeric_limits<std::size_t>::max();
        };

        class MoveAtVelocity : public Base
        {
        public:

            MoveAtVelocity(NPC& _npc, const glm::vec2& _velocity)
                : Base{_npc}
                , velocity{_velocity}
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
        
            MoveTowardTarget(NPC& _npc, float _speed, float _desiredRange)
                : Base{_npc}
                , speed{_speed}
                , desiredRange(_desiredRange)
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
        
            Wait(NPC& _npc, float _duration)
                : Base{_npc}
                , duration{_duration}
            { }
            
            virtual Result initialize() override;
            virtual Result update(float dt) override;

        private:
        
            float duration;
            
            float timeRemaining = 0.0f;
        };
    }
}

#endif
