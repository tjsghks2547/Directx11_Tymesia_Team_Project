#include "pch.h"
#include "Ladder_Climb_R_Up_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLadder_Climb_R_Up_End::CLadder_Climb_R_Up_End()
{
}

HRESULT CLadder_Climb_R_Up_End::Initialize()
{
	return S_OK;
}

void CLadder_Climb_R_Up_End::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterUp = XMVector3Normalize(XMLoadFloat4(&m_vMonsterUpDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	MonsterPos -= MonsterDir * 0.3f;
	MonsterPos += MonsterUp * 0.4f;


	float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir * -1.f));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, MonsterDir * -1.f);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f)
	{
		Radian = -Radian;
	}


	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, MonsterPos);

}

void CLadder_Climb_R_Up_End::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_R_Up_End::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CLadder_Climb_R_Up_End* CLadder_Climb_R_Up_End::Create()
{
	CLadder_Climb_R_Up_End* pInstance = new CLadder_Climb_R_Up_End();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Ladder_Climb_R_Up_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder_Climb_R_Up_End::Free()
{
	__super::Free();
}
