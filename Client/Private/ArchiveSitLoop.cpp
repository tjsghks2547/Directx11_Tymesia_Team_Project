#include "pch.h"
#include "ArchiveSitLoop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CArchiveSitLoop::CArchiveSitLoop()
{
}

HRESULT CArchiveSitLoop::Initialize()
{
	return S_OK;
}

void CArchiveSitLoop::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitLoop::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitLoop::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CArchiveSitLoop* CArchiveSitLoop::Create()
{
	CArchiveSitLoop* pInstance = new CArchiveSitLoop();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ArchiveSitLoop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchiveSitLoop::Free()
{
	__super::Free();
}
