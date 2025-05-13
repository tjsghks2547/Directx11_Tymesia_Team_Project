#include "pch.h"
#include "Rock_On_Run_BL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_BL::CRock_On_Run_BL()
{
}

HRESULT CRock_On_Run_BL::Initialize()
{
	return S_OK;
}

void CRock_On_Run_BL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f;
	_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(45.f));

	_vector vBLdIR = XMVector3TransformNormal(vBackDir, rotationMartix);

	pGameObject->Get_Transfrom()->Go_Dir(vBLdIR, pNavigation, fTimeDelta * m_fWalkSpeed);
}

void CRock_On_Run_BL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_BL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_BL* CRock_On_Run_BL::Create()
{
	CRock_On_Run_BL* pInstance = new CRock_On_Run_BL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_BL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_BL::Free()
{
	__super::Free();
}
