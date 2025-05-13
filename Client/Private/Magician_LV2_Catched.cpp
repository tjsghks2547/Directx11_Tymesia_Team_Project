#include "pch.h"
#include "Magician_LV2_Catched.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CMagician_LV2_Catched::CMagician_LV2_Catched()
{
}

HRESULT CMagician_LV2_Catched::Initialize()
{
	return S_OK;
}

void CMagician_LV2_Catched::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir)) * -1.f;
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);


	float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, MonsterDir);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f)
	{
		Radian = -Radian;
	}

	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, MonsterPos);
}

void CMagician_LV2_Catched::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CMagician_LV2_Catched::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CMagician_LV2_Catched* CMagician_LV2_Catched::Create()
{
	CMagician_LV2_Catched* pInstance = new CMagician_LV2_Catched();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMagician_LV2_Catched");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMagician_LV2_Catched::Free()
{
	__super::Free();
}
