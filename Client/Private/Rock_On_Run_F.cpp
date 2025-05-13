#include "pch.h"
#include "Rock_On_Run_F.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_F::CRock_On_Run_F()
{
}

HRESULT CRock_On_Run_F::Initialize()
{
	return S_OK;
}

void CRock_On_Run_F::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CRock_On_Run_F::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_F::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_F* CRock_On_Run_F::Create()
{
	CRock_On_Run_F* pInstance = new CRock_On_Run_F();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_F::Free()
{
	__super::Free();
}
