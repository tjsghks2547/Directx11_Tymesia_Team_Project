#include "PhysX_SimulationEventCallBack.h"
#include "PhysX_Manager.h"  	

#include "GameInstance.h"
#include "GameObject.h"


CPhysX_SimulationEventCallBack::CPhysX_SimulationEventCallBack()
    :m_pGameInstance(CGameInstance::GetInstance())
{

}

CPhysX_SimulationEventCallBack::~CPhysX_SimulationEventCallBack()
{

}

void CPhysX_SimulationEventCallBack::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{
    cout << "onConstraintBreak" << endl;
}

void CPhysX_SimulationEventCallBack::onWake(PxActor** actors, PxU32 count)
{
    cout << "onWake" << endl;
}

void CPhysX_SimulationEventCallBack::onSleep(PxActor** actors, PxU32 count)
{
    cout << "onSleep" << endl;
}

void CPhysX_SimulationEventCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    PX_UNUSED((pairHeader));

    for (PxU32 i = 0; i < nbPairs; ++i) {
        const PxContactPair& cp = pairs[i];

        if (pairHeader.flags & (PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | PxContactPairHeaderFlag::eREMOVED_ACTOR_1))
            return;

        PxRigidActor* Actor0 = cp.shapes[0]->getActor();
        PxRigidActor* Actor1 = cp.shapes[1]->getActor();

        if (Actor0->userData == nullptr || Actor1->userData == nullptr)
            return;


        CGameObject* colliderA = (CGameObject*)Actor0->userData;
        CGameObject* colliderB = (CGameObject*)Actor1->userData;


        /* if (!colliderA->Get_Simulation() || !colliderB->Get_Simulation())
            return;*/

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            colliderA->OnCollisionEnter(colliderB, cp);
            colliderB->OnCollisionEnter(colliderA, cp);

            //PxContactPairPoint contactPoints[10]; // 최대 10개까지 저장
            //cp.extractContacts(contactPoints, 1);
        }

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            colliderA->OnCollision(colliderB, cp);
            colliderB->OnCollision(colliderA, cp);
        }

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            colliderA->OnCollisionExit(colliderB, cp);
            colliderB->OnCollisionExit(colliderA, cp);
        }
    }
}

void CPhysX_SimulationEventCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

}

void CPhysX_SimulationEventCallBack::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{

}


