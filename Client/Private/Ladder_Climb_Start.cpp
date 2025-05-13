#include "pch.h"
#include "Ladder_Climb_Start.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLadder_Climb_Start::CLadder_Climb_Start()
{
}

HRESULT CLadder_Climb_Start::Initialize()
{
	return S_OK;
}

void CLadder_Climb_Start::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterUp = XMVector3Normalize(XMLoadFloat4(&m_vMonsterUpDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	MonsterPos += MonsterDir * -1.f * 0.65f;

	float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, MonsterDir);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f)
	{
		Radian = -Radian;
	}


	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, MonsterPos);

}

void CLadder_Climb_Start::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_Start::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CLadder_Climb_Start* CLadder_Climb_Start::Create()
{
	CLadder_Climb_Start* pInstance = new CLadder_Climb_Start();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Ladder_Climb_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder_Climb_Start::Free()
{
	__super::Free();
}
