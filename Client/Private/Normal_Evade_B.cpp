#include "pch.h"
#include "Normal_Evade_B.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_B::CNormal_Evade_B()
{
}

HRESULT CNormal_Evade_B::Initialize()
{
	return S_OK;
}

void CNormal_Evade_B::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);


	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = vLookFront * -1.f;
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


	float dotResult = XMVectorGetX(XMVector3Dot(vLookBack, PlayerLook));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, vLookBack);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);

}

void CNormal_Evade_B::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_B::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_B* CNormal_Evade_B::Create()
{
	CNormal_Evade_B* pInstance = new CNormal_Evade_B();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_B::Free()
{
	__super::Free();
}
