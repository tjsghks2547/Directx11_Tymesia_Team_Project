#include "pch.h"
#include "Rock_On_Run_B.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_B::CRock_On_Run_B()
{
}

HRESULT CRock_On_Run_B::Initialize()
{
	return S_OK;
}

void CRock_On_Run_B::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CRock_On_Run_B::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_B::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_B* CRock_On_Run_B::Create()
{
	CRock_On_Run_B* pInstance = new CRock_On_Run_B();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_B::Free()
{
	__super::Free();
}
