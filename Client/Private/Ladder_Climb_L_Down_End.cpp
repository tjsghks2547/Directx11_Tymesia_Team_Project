#include "pch.h"
#include "Ladder_Climb_L_Down_End.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CLadder_Climb_L_Down_End::CLadder_Climb_L_Down_End()
{
}

HRESULT CLadder_Climb_L_Down_End::Initialize()
{
	return S_OK;
}

void CLadder_Climb_L_Down_End::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_L_Down_End::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_L_Down_End::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CLadder_Climb_L_Down_End* CLadder_Climb_L_Down_End::Create()
{
	CLadder_Climb_L_Down_End* pInstance = new CLadder_Climb_L_Down_End();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Ladder_Climb_L_Down_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder_Climb_L_Down_End::Free()
{
	__super::Free();
}
