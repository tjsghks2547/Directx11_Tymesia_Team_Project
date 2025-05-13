#include "pch.h"
#include "LockOn_Evade_F.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLockOn_Evade_F::CLockOn_Evade_F()
{
}

HRESULT CLockOn_Evade_F::Initialize()
{
	return S_OK;
}

void CLockOn_Evade_F::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLockOn_Evade_F::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CLockOn_Evade_F::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CLockOn_Evade_F* CLockOn_Evade_F::Create()
{
	CLockOn_Evade_F* pInstance = new CLockOn_Evade_F();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : LockOn_Evade_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLockOn_Evade_F::Free()
{
	__super::Free();
}
