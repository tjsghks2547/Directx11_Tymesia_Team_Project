#include "pch.h"
#include "ParryDeflect_Large.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParryDeflect_Large::CParryDeflect_Large()
{
}

HRESULT  CParryDeflect_Large::Initialize()
{
	return S_OK;
}

void  CParryDeflect_Large::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CParryDeflect_Large::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CParryDeflect_Large::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParryDeflect_Large* CParryDeflect_Large::Create()
{
	CParryDeflect_Large* pInstance = new  CParryDeflect_Large();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ParryDeflect_Large");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryDeflect_Large::Free()
{
	__super::Free();
}
