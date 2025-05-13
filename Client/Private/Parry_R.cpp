#include "pch.h"
#include "Parry_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParry_R::CParry_R()
{
}

HRESULT CParry_R::Initialize()
{
	return S_OK;
}

void CParry_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CParry_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CParry_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParry_R* CParry_R::Create()
{
	CParry_R* pInstance = new CParry_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Parry_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParry_R::Free()
{
	__super::Free();
}
