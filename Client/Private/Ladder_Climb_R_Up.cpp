#include "pch.h"
#include "Ladder_Climb_R_Up.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"

CLadder_Climb_R_Up::CLadder_Climb_R_Up()
{

}

HRESULT CLadder_Climb_R_Up::Initialize()
{
	return S_OK;
}

void CLadder_Climb_R_Up::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerPos = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterUp = XMVector3Normalize(XMLoadFloat4(&m_vMonsterUpDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	if (m_vMonsterPos.y + 1.f < PlayerPos.m128_f32[1])
	{
		dynamic_cast<CPlayer*>(pGameObject)->Set_PlayerState(CPlayer::STATE_LADDER_CLIMB_L_UP_END);
	}
}

void CLadder_Climb_R_Up::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_R_Up::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CLadder_Climb_R_Up* CLadder_Climb_R_Up::Create()
{
	CLadder_Climb_R_Up* pInstance = new CLadder_Climb_R_Up();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Ladder_Climb_R_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder_Climb_R_Up::Free()
{
	__super::Free();
}
