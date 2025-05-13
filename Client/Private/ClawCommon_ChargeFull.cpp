#include "pch.h"
#include "ClawCommon_ChargeFull.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"


CClawCommon_ChargeFull::CClawCommon_ChargeFull()
{
}

HRESULT CClawCommon_ChargeFull::Initialize()
{
	return S_OK;
}

void CClawCommon_ChargeFull::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CClawCommon_ChargeFull::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CClawCommon_ChargeFull::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CClawCommon_ChargeFull* CClawCommon_ChargeFull::Create()
{
	CClawCommon_ChargeFull* pInstance = new CClawCommon_ChargeFull();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ClawCommon_ChargeFull");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CClawCommon_ChargeFull::Free()
{
	__super::Free();
}
