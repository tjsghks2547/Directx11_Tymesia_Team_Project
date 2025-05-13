#include "pch.h"
#include "ParryDeflect_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParryDeflect_R::CParryDeflect_R()
{
}

HRESULT CParryDeflect_R::Initialize()
{
	return S_OK;
}

void CParryDeflect_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CParryDeflect_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CParryDeflect_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParryDeflect_R* CParryDeflect_R::Create()
{
	CParryDeflect_R* pInstance = new CParryDeflect_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ParryDeflect_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryDeflect_R::Free()
{
	__super::Free();
}
