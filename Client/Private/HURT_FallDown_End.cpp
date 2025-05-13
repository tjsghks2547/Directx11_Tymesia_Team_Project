#include "pch.h"
#include "HURT_FallDown_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHURT_FallDown_End::CHURT_FallDown_End()
{
}

HRESULT CHURT_FallDown_End::Initialize()
{
	return S_OK;
}

void CHURT_FallDown_End::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir)) * -1.f;



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

}

void  CHURT_FallDown_End::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CHURT_FallDown_End::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHURT_FallDown_End* CHURT_FallDown_End::Create()
{
	CHURT_FallDown_End* pInstance = new  CHURT_FallDown_End();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CHURT_FallDown_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHURT_FallDown_End::Free()
{
	__super::Free();
}
