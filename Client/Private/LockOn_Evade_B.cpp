#include "pch.h"
#include "LockOn_Evade_B.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLockOn_Evade_B::CLockOn_Evade_B()
{
}

HRESULT CLockOn_Evade_B::Initialize()
{
	return S_OK;
}

void CLockOn_Evade_B::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLockOn_Evade_B::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CLockOn_Evade_B::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CLockOn_Evade_B* CLockOn_Evade_B::Create()
{
	CLockOn_Evade_B* pInstance = new CLockOn_Evade_B();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : LockOn_Evade_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLockOn_Evade_B::Free()
{
	__super::Free();
}
