#include "pch.h"
#include "Researcher_Cathced.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CResearcher_Cathced::CResearcher_Cathced()
{
}

HRESULT CResearcher_Cathced::Initialize()
{
	return S_OK;
}

void CResearcher_Cathced::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir)) * -1.f;
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);


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

void CResearcher_Cathced::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CResearcher_Cathced::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CResearcher_Cathced* CResearcher_Cathced::Create()
{
	CResearcher_Cathced* pInstance = new CResearcher_Cathced();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CResearcher_Cathced");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CResearcher_Cathced::Free()
{
	__super::Free();
}
