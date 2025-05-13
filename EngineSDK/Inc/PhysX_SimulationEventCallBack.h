/*
* PxSimulationEventCallback Ŀ������ ���� PhysX_SimulationEventCallBack Ŭ�����Դϴ�.
* �浹 �̺�Ʈ �߻��� PhysXCollider ������Ʈ�� �浹 ������ �����ϵ��� �����߽��ϴ�.
*/

#pragma once

#include "Engine_Defines.h"

#define PHYSX_SIBULATIONCALLBACK_DEBUG 0	

BEGIN(Engine)

class CPhysX_SimulationEventCallBack : public PxSimulationEventCallback
{
public:
    CPhysX_SimulationEventCallBack();
    ~CPhysX_SimulationEventCallBack();

public:
    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override;
    virtual void onWake(PxActor** actors, PxU32 count) override;
    virtual void onSleep(PxActor** actors, PxU32 count) override;
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
    virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override;

private:
    class CGameInstance* m_pGameInstance = { nullptr };
};

END