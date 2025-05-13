#include "pch.h"
#include "StunnedEnd.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CStunnedEnd::CStunnedEnd()
{
}

HRESULT CStunnedEnd::Initialize()
{
	return S_OK;
}

void CStunnedEnd::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CStunnedEnd::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CStunnedEnd::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CStunnedEnd* CStunnedEnd::Create()
{
	CStunnedEnd* pInstance = new  CStunnedEnd();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : StunnedEnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStunnedEnd::Free()
{
	__super::Free();
}
