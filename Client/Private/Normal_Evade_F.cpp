#include "pch.h"
#include "Normal_Evade_F.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_F::CNormal_Evade_F()
{
}

HRESULT CNormal_Evade_F::Initialize()
{
	return S_OK;
}

void CNormal_Evade_F::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

	float dotResult = XMVectorGetX(XMVector3Dot(vLookFront, PlayerLook));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, vLookFront);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
}

void CNormal_Evade_F::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_F::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_F* CNormal_Evade_F::Create()
{
	CNormal_Evade_F* pInstance = new CNormal_Evade_F();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_F::Free()
{
	__super::Free();
}
