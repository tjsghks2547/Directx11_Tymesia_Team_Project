#include "pch.h"
#include "LockOn_Evade_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLockOn_Evade_L::CLockOn_Evade_L()
{
}

HRESULT CLockOn_Evade_L::Initialize()
{
	return S_OK;
}

void CLockOn_Evade_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLockOn_Evade_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CLockOn_Evade_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CLockOn_Evade_L* CLockOn_Evade_L::Create()
{
	CLockOn_Evade_L* pInstance = new CLockOn_Evade_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : LockOn_Evade_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLockOn_Evade_L::Free()
{
	__super::Free();
}
