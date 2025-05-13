#include "pch.h"
#include "Rock_On_Run_BR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_BR::CRock_On_Run_BR()
{
}

HRESULT CRock_On_Run_BR::Initialize()
{
	return S_OK;
}

void CRock_On_Run_BR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f;
	_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-45.f));

	_vector vBLdIR = XMVector3TransformNormal(vBackDir, rotationMartix);

	pGameObject->Get_Transfrom()->Go_Dir(vBLdIR, pNavigation, fTimeDelta * m_fWalkSpeed);
}

void CRock_On_Run_BR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_BR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_BR* CRock_On_Run_BR::Create()
{
	CRock_On_Run_BR* pInstance = new CRock_On_Run_BR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRock_On_Run_BR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_BR::Free()
{
	__super::Free();
}
