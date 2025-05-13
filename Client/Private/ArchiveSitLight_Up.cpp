#include "pch.h"
#include "ArchiveSitLight_Up.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CArchiveSitLight_Up::CArchiveSitLight_Up()
{
}

HRESULT CArchiveSitLight_Up::Initialize()
{
	return S_OK;
}

void CArchiveSitLight_Up::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	MonsterPos += MonsterDir * 0.7f;


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

void CArchiveSitLight_Up::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitLight_Up::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CArchiveSitLight_Up* CArchiveSitLight_Up::Create()
{
	CArchiveSitLight_Up* pInstance = new CArchiveSitLight_Up();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ArchiveSitLight_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchiveSitLight_Up::Free()
{
	__super::Free();
}
