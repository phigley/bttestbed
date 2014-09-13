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


        class Root
        {
        public:
        
            template<typename... Args>
            Root(Args&&... _children)
                : children{ std::forward<Args>(_children)... }
            { }
            
            ~Root();
            
            void update(float dt);
            
        private :

            std::vector<Base::Ptr>    children;
            std::size_t               activeChild = std::numeric_limits<std::size_t>::max();
        };

        class PrioritySelector : public Base
        {
        public:

            template<typename... Args>
            PrioritySelector(NPC& _npc, Args&&... _children)
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
    }
}

#endif
