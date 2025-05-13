#include "pch.h"
#include "ParryDeflect_RUp.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParryDeflect_RUp::CParryDeflect_RUp()
{
}

HRESULT CParryDeflect_RUp::Initialize()
{
	return S_OK;
}

void CParryDeflect_RUp::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CParryDeflect_RUp::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CParryDeflect_RUp::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParryDeflect_RUp* CParryDeflect_RUp::Create()
{
	CParryDeflect_RUp* pInstance = new CParryDeflect_RUp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ParryDeflect_RUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryDeflect_RUp::Free()
{
	__super::Free();
}
