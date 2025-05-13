#include "pch.h"
#include "Normal_Evade_BL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_BL::CNormal_Evade_BL()
{
}

HRESULT CNormal_Evade_BL::Initialize()
{
	return S_OK;
}

void CNormal_Evade_BL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);


	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = vLookFront * -1.f;
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


	_matrix rotationMartix = XMMatrixRotationAxis(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(45.f));
	_vector vLeftLook45Degree = XMVector3Transform(vLookBack, rotationMartix);

	float dotResult = XMVectorGetX(XMVector3Dot(vLeftLook45Degree, PlayerLook));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, vLeftLook45Degree);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
}

void CNormal_Evade_BL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_BL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_BL* CNormal_Evade_BL::Create()
{
	CNormal_Evade_BL* pInstance = new CNormal_Evade_BL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_BL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_BL::Free()
{
	__super::Free();
}
