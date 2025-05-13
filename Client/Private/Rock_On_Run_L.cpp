#include "pch.h"
#include "Rock_On_Run_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_L::CRock_On_Run_L()
{
}

HRESULT CRock_On_Run_L::Initialize()
{
	return S_OK;
}

void CRock_On_Run_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	pGameObject->Get_Transfrom()->Go_Dir(CamLeft, pNavigation, fTimeDelta * m_fWalkSpeed);
}

void CRock_On_Run_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_L* CRock_On_Run_L::Create()
{
	CRock_On_Run_L* pInstance = new CRock_On_Run_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_L::Free()
{
	__super::Free();
}
