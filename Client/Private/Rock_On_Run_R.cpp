#include "pch.h"
#include "Rock_On_Run_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_R::CRock_On_Run_R()
{
}

HRESULT CRock_On_Run_R::Initialize()
{
	return S_OK;
}

void CRock_On_Run_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);

	pGameObject->Get_Transfrom()->Go_Dir(CamRight, pNavigation, fTimeDelta * m_fWalkSpeed);
}

void CRock_On_Run_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_R* CRock_On_Run_R::Create()
{
	CRock_On_Run_R* pInstance = new CRock_On_Run_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_R::Free()
{
	__super::Free();
}
