#include "pch.h"
#include "ParryDeflect_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParryDeflect_L::CParryDeflect_L()
{
}

HRESULT  CParryDeflect_L::Initialize()
{
	return S_OK;
}

void  CParryDeflect_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CParryDeflect_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CParryDeflect_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParryDeflect_L* CParryDeflect_L::Create()
{
	CParryDeflect_L* pInstance = new  CParryDeflect_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ParryDeflect_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryDeflect_L::Free()
{
	__super::Free();
}
