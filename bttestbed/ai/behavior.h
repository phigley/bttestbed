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

            Base(NPC& npc_, bool requiresUpdate_)
                : npc{npc_}
                , requiresUpdate{requiresUpdate_}
            { }
            
            virtual ~Base() { }

            // Called to set-up the behavior.
            virtual Result initialize(std::vector<Base::Ptr>&) { return Result::Continue; }
            
            // Called if initialize returned Result::Continue.
            virtual Result update(float dt) { return Result::Continue; }
            
            // Called if and only if initialize returned Result::Continue.
            virtual void term() { }
            
            virtual Result onChildFailed(std::vector<Base::Ptr>&)      { return Result::Fail; }
            virtual Result onChildCompleted(std::vector<Base::Ptr>&)   { return Result::Complete; }
            
            NPC& getNPC() { return npc; }
            const NPC& getNPC() const { return npc; }

            bool getRequiresUpdate() const { return requiresUpdate; }

        private:

            NPC&    npc;
            bool    requiresUpdate;
        };

        typedef std::vector<Base::Ptr> ActiveList;

        class Root
        {
        public:

            template<typename... Args>
            Root(NPC& npc_, Args&&... children_)
                : npc{npc_}
                , children{ std::forward<Args>(children_)... }
            {
            }
            
            void update(float dt);
            
        private :
        
            NPC&                    npc;
            std::vector<Base::Ptr>  children;
            ActiveList              activePath;
            
        };
        
        class Priority : public Base
        {
        public:

            template<typename... Args>
            Priority(NPC& npc_, Args&&... children_)
                : Base{npc_, false}
                , children{ std::forward<Args>(children_)... }
            {
            }
            
            virtual Result initialize(ActiveList&) override;
            
        private :

            std::vector<Ptr>    children;
        };


        class Sequence : public Base
        {
        public:
            template<typename... Args>
            Sequence(NPC& npc_, Args&&... children_)
                : Base{npc_, false}
                , children{ std::forward<Args>(children_)... }
            {
            }
        
            virtual Result initialize(ActiveList&) override;
            virtual Result onChildCompleted(ActiveList&) override;
                        
        private :

            Result initializeNextChild(ActiveList& activeList);

            std::vector<Ptr>    children;
            std::size_t         activeChild = std::numeric_limits<std::size_t>::max();
            
        };

        class HasTarget : public Base
        {
        public:
        
            HasTarget(NPC& npc_, Ptr&& child_)
                : Base{npc_, true}
                , child(child_)
            { }
            
            virtual Result initialize(ActiveList&) override;
            virtual Result update(float dt) override;
            
        private:
        
            Ptr child;
        };

        class MoveAtVelocity : public Base
        {
        public:

            MoveAtVelocity(NPC& npc_, const glm::vec2& velocity_)
                : Base{npc_, true}
                , velocity{velocity_}
            {
            }
            
            virtual Result initialize(ActiveList&) override;
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
                : Base{npc_, true}
                , speed{speed_}
                , desiredRange(desiredRange_)
            {
            }

            virtual Result initialize(ActiveList&) override;
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
                : Base{npc_, true}
                , duration{duration_}
            { }
            
            virtual Result initialize(ActiveList&) override;
            virtual Result update(float dt) override;

        private:
        
            float duration;
            
            float timeRemaining = 0.0f;
        };
        
        class ClearTarget : public Base
        {
        public:
        
            ClearTarget(NPC& npc_)
                : Base{npc_, false}
            { }
            
            virtual Result initialize(ActiveList&) override;
        };
        
        class LockTarget : public Base
        {
        public:
            LockTarget(NPC& npc_, Ptr&& child_)
                : Base{npc_, false}
                , child{child_}
            { }

            virtual Result initialize(ActiveList&) override;
            virtual void term() override;
            
        private:
        
            bool    appliedLock = false;
            Ptr     child;
        
        };
    }
}

#endif
