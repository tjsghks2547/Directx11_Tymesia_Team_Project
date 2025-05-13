#include "pch.h"
#include "Rebound_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRebound_R::CRebound_R()
{
}

HRESULT CRebound_R::Initialize()
{
	return S_OK;
}

void CRebound_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
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

void  CRebound_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CRebound_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRebound_R* CRebound_R::Create()
{
	CRebound_R* pInstance = new  CRebound_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rebound_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRebound_R::Free()
{
	__super::Free();
}
