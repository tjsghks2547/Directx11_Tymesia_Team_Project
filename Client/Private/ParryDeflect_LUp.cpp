#include "pch.h"
#include "ParryDeflect_LUp.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CParryDeflect_LUp::CParryDeflect_LUp()
{
}

HRESULT  CParryDeflect_LUp::Initialize()
{
	return S_OK;
}

void  CParryDeflect_LUp::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CParryDeflect_LUp::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CParryDeflect_LUp::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CParryDeflect_LUp* CParryDeflect_LUp::Create()
{
	CParryDeflect_LUp* pInstance = new  CParryDeflect_LUp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ParryDeflect_LUp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParryDeflect_LUp::Free()
{
	__super::Free();
}
