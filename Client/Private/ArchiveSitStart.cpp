#include "pch.h"
#include "ArchiveSitStart.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CArchiveSitStart::CArchiveSitStart()
{
}

HRESULT CArchiveSitStart::Initialize()
{
	return S_OK;
}

void CArchiveSitStart::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	MonsterPos += MonsterDir * 0.2f;


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

void CArchiveSitStart::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitStart::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CArchiveSitStart* CArchiveSitStart::Create()
{
	CArchiveSitStart* pInstance = new CArchiveSitStart();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ArchiveSitStart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchiveSitStart::Free()
{
	__super::Free();
}
