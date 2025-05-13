#include "pch.h"
#include "HurtMFR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHurtMFR::CHurtMFR()
{
}

HRESULT CHurtMFR::Initialize()
{
	return S_OK;
}

void CHurtMFR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
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

void CHurtMFR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CHurtMFR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHurtMFR* CHurtMFR::Create()
{
	CHurtMFR* pInstance = new CHurtMFR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CHurtMFR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHurtMFR::Free()
{
	__super::Free();
}
