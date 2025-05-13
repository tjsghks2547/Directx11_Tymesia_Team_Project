#include "pch.h"
#include "PlayerIdle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CPlayerIdle::CPlayerIdle()
{
}

HRESULT CPlayerIdle::Initialize()
{
	return S_OK;
}

void CPlayerIdle::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CPlayerIdle::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CPlayerIdle::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CPlayerIdle* CPlayerIdle::Create()
{
	CPlayerIdle* pInstance = new CPlayerIdle();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : PlayerIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerIdle::Free()
{
	__super::Free();
}
