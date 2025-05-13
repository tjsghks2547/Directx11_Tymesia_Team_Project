#include "pch.h"
#include "HArmorLV1_Execution.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHArmorLV1_Execution::CHArmorLV1_Execution()
{
}

HRESULT CHArmorLV1_Execution::Initialize()
{
	return S_OK;
}

void  CHArmorLV1_Execution::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CHArmorLV1_Execution::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void  CHArmorLV1_Execution::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHArmorLV1_Execution* CHArmorLV1_Execution::Create()
{
	CHArmorLV1_Execution* pInstance = new  CHArmorLV1_Execution();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : HArmorLV1_Execution");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHArmorLV1_Execution::Free()
{
	__super::Free();
}
