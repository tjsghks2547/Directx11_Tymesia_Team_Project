#include "pch.h"
#include "HurtMFL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHurtMFL::CHurtMFL()
{
}

HRESULT CHurtMFL::Initialize()
{
	return S_OK;
}

void CHurtMFL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir)) * -1.f;


	float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, MonsterDir);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);



}

void CHurtMFL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CHurtMFL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHurtMFL* CHurtMFL::Create()
{
	CHurtMFL* pInstance = new CHurtMFL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CHurtMFL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHurtMFL::Free()
{
	__super::Free();
}
