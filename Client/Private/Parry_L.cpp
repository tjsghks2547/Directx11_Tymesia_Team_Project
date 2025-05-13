#include "pch.h"
#include "Parry_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParry_L::CParry_L()
{
}

HRESULT CParry_L::Initialize()
{
	return S_OK;
}

void CParry_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CParry_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CParry_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParry_L* CParry_L::Create()
{
	CParry_L* pInstance = new CParry_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Parry_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParry_L::Free()
{
	__super::Free();
}
