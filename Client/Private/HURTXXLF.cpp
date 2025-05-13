#include "pch.h"
#include "HURTXXLF.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHURTXXLF::CHURTXXLF()
{
}

HRESULT CHURTXXLF::Initialize()
{
	return S_OK;
}

void CHURTXXLF::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
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

void  CHURTXXLF::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CHURTXXLF::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHURTXXLF* CHURTXXLF::Create()
{
	CHURTXXLF* pInstance = new  CHURTXXLF();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : HURTXXLF");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHURTXXLF::Free()
{
	__super::Free();
}
