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

#include "rapidxml/rapidxml.hpp"
#include "glm/glm.hpp"

#include <vector>
#include <memory>
#include <limits>

namespace AI
{
    class NPC;

    namespace Behavior
    {
        class Base;
        
        class ActiveBehavior
        {
        public:
        
            explicit ActiveBehavior(Base&);
            
            bool     getRequiresUpdate() const { return requiresUpdate; }
            
            Base&    getBehavior() { return behavior; }
            const Base& getBehavior() const { return behavior; }
            
        private :
        
            bool    requiresUpdate;
            Base&   behavior;
        };

        class Base
        {
        public:

            
            typedef std::shared_ptr<Base>       Ptr;
            
            typedef std::vector<Base*>          PendingList;
            typedef std::vector<ActiveBehavior> ActiveList;
            
        public:

            Base(NPC& npc_, bool requiresUpdate_)
                : npc{npc_}
                , requiresUpdate{requiresUpdate_}
            { }
            
            virtual ~Base() { }

            // Called to set-up the behavior.
            virtual Result initialize(PendingList& pendingBehaviors) { return Result::Continue; }
            
            // Called on the active child during planning updates.
            // childPtr contains a pointer to the behavior's child (so the behavior does not need to track that itself).
            // If pendingBehaviors comes back non-empty, the remaining children will be terminated and replaced with
            // pendingBehaviors.
            virtual void reinitialize(const Base* childPtr, PendingList& pendingBehaviors) { }
            
            // Called if initialize returned Result::Continue.
            virtual Result update(float dt) { return Result::Continue; }
            
            // Called if and only if initialize returned Result::Continue.
            virtual void term() { }
            
            virtual Result onChildFailed(PendingList&)      { return Result::Fail; }
            virtual Result onChildCompleted(PendingList&)   { return Result::Complete; }
            
            NPC& getNPC() { return npc; }
            const NPC& getNPC() const { return npc; }

            bool getRequiresUpdate() const { return requiresUpdate; }

        private:

            NPC&    npc;
            bool    requiresUpdate;
        };

        class Root
        {
        public:

            Root(NPC& npc_, rapidxml::xml_node<>& rootNode);
            
            void update(float dt);
            
        private :
        
            NPC&                    npc;
            std::vector<Base::Ptr>  children;
            Base::ActiveList        activePath;
            float                   planningUpdateDuration = 0.0f;

        };
        
        class Priority : public Base
        {
        public:

            Priority(NPC& npc_, rapidxml::xml_node<>& priorityNode);
            
            virtual Result initialize(PendingList&) override;
            virtual void reinitialize(const Base* childPtr, PendingList& pendingBehaviors) override;
            
        private :

            std::vector<Ptr>    children;
        };


        class Sequence : public Base
        {
        public:

            Sequence(NPC& npc_, rapidxml::xml_node<>& sequenceNode);
        
            virtual Result initialize(PendingList&) override;
            virtual Result onChildCompleted(PendingList&) override;
                        
        private :

            Result initializeNextChild(PendingList& activeList);

            std::vector<Ptr>    children;
            std::size_t         activeChild = std::numeric_limits<std::size_t>::max();
            
        };

        class HasTarget : public Base
        {
        public:
        
            HasTarget(NPC& npc_, rapidxml::xml_node<>& hasTargetNode);
            
            virtual Result initialize(PendingList&) override;
            virtual Result update(float dt) override;
            
        private:
        
            Ptr child;
            float maxDuration;
        };
        

        class MoveAtVelocity : public Base
        {
        public:

            MoveAtVelocity(NPC& npc_, rapidxml::xml_node<>& xmlNode);

            virtual Result initialize(PendingList&) override;
            virtual Result update(float dt) override;
            virtual void term() override;

        private:

            glm::vec2 velocity;

            std::unique_ptr<State::MoveAtVelocity> state;
        };
        
        class MoveTowardTarget : public Base
        {
        public:
        
            MoveTowardTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode);
            
            virtual Result initialize(PendingList&) override;
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
        
            Wait(NPC& npc_, rapidxml::xml_node<>& xmlNode);
            
            virtual Result initialize(PendingList&) override;
            virtual Result update(float dt) override;

        private:
        
            float duration;
            
            float timeRemaining = 0.0f;
        };
        
        class ClearTarget : public Base
        {
        public:
        
            ClearTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode);
            
            virtual Result initialize(PendingList&) override;
        };
        
        class LockTarget : public Base
        {
        public:
            LockTarget(NPC& npc_, rapidxml::xml_node<>& xmlNode);

            virtual Result initialize(PendingList&) override;
            virtual void term() override;
            
        private:
        
            bool    appliedLock = false;
            Ptr     child;
        
        };
    }
}

#endif
