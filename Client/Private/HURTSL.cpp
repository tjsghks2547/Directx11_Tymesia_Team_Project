#include "pch.h"
#include "HURTSL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHURTSL::CHURTSL()
{
}

HRESULT CHURTSL::Initialize()
{
	return S_OK;
}

void CHURTSL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
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

void  CHURTSL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CHURTSL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHURTSL* CHURTSL::Create()
{
	CHURTSL* pInstance = new  CHURTSL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : HURTSL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHURTSL::Free()
{
	__super::Free();
}
