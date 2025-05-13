#include "pch.h"
#include "Ladder_Climb_L_Down.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"	

CLadder_Climb_L_Down::CLadder_Climb_L_Down()
{
}

HRESULT CLadder_Climb_L_Down::Initialize()
{
	return S_OK;
}

void CLadder_Climb_L_Down::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_vector PlayerPos = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	_vector MonsterDir = XMVector3Normalize(XMLoadFloat4(&m_vMonsterLookDir));
	_vector MonsterUp = XMVector3Normalize(XMLoadFloat4(&m_vMonsterUpDir));
	_vector MonsterPos = XMLoadFloat4(&m_vMonsterPos);

	if (m_vMonsterPos.y + 0.3f < PlayerPos.m128_f32[1])
	{
		dynamic_cast<CPlayer*>(pGameObject)->Set_PlayerState(CPlayer::STATE_LADDER_CLIMB_R_DOWN_END);
	}

}

void CLadder_Climb_L_Down::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CLadder_Climb_L_Down::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CLadder_Climb_L_Down* CLadder_Climb_L_Down::Create()
{
	CLadder_Climb_L_Down* pInstance = new CLadder_Climb_L_Down();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Ladder_Climb_L_Down");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLadder_Climb_L_Down::Free()
{
	__super::Free();
}
