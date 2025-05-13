#include "pch.h"
#include "LockOn_Evade_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLockOn_Evade_R::CLockOn_Evade_R()
{
}

HRESULT CLockOn_Evade_R::Initialize()
{
	return S_OK;
}

void CLockOn_Evade_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLockOn_Evade_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CLockOn_Evade_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CLockOn_Evade_R* CLockOn_Evade_R::Create()
{
	CLockOn_Evade_R* pInstance = new CLockOn_Evade_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : LockOn_Evade_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLockOn_Evade_R::Free()
{
	__super::Free();
}
