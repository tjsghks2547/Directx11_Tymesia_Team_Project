#include "pch.h"
#include "StunnedLoop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CStunnedLoop::CStunnedLoop()
{
}

HRESULT CStunnedLoop::Initialize()
{
	return S_OK;
}

void CStunnedLoop::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{


}

void  CStunnedLoop::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void  CStunnedLoop::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CStunnedLoop* CStunnedLoop::Create()
{
	CStunnedLoop* pInstance = new  CStunnedLoop();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : StunnedLoop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStunnedLoop::Free()
{
	__super::Free();
}
