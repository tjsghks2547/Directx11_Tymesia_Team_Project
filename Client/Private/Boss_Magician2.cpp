#include "pch.h"
#include "Boss_Magician2.h"
#include "Body_Magician2.h"
#include "Weapon_Magician2_Sword.h"
#include "UI_Boss_HP_Bar.h"
#include "Decorative_Tonic.h"
#include "Locked_On.h"
#include "Player.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Boss_Mutation_Magician_Camera.h"	

CBoss_Magician2::CBoss_Magician2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Magician2::CBoss_Magician2(const CBoss_Magician2& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CBoss_Magician2::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Magician2::Initialize(void* pArg)
{
	m_Is_Boss = true;
	m_fRotateSpeed = 180.f;
	m_fRootDistance = 1.f;
	m_fActive_Distance = 15.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
	m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
	m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();

	m_pTransformCom->Scaling(_float3{ 0.0025f,0.0025f,0.0025f });	
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pState_Manager = CState_Machine<CBoss_Magician2>::Create();
	if (m_pState_Manager == nullptr)
		return E_FAIL;


	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.5f,0.5f,0.2f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	settingColliderGroup = GROUP_TYPE::PLAYER;
	m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,0.f,5.f,1.f });

	m_pGameInstance->Add_Actor_Scene(m_pActor);

	m_fCoolTime = 0.2f;

	m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_CANESWORD;

	m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);

	return S_OK;
}

void CBoss_Magician2::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Magician2::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
		m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

}

void CBoss_Magician2::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Magician2::Render()
{
	return S_OK;
}

void CBoss_Magician2::State_Update(_float fTimeDelta)
{
	m_pState_Manager->State_Update(fTimeDelta, this);
}

void CBoss_Magician2::PatternCreate()
{

	if (m_bActive)
		m_fSpecial_Skill_CoolTime += m_fTimeDelta;

	if (!m_bPatternProgress && !m_bSpecial_Skill_Progress && m_bActive)
	{
		m_fDelayTime += 1 * m_fTimeDelta;
		if (m_fDelayTime >= m_fCoolTime)
		{
			if (m_fSpecial_Skill_CoolTime >= 60.f)
				m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_Special(), this);
			else if (m_fDistance >= 10.f)
				Far_Pattern_Create();
			else
				Near_Pattern_Create();

			m_fDelayTime = 0.f;
			m_bPatternProgress = true;
		}
	}

}

void CBoss_Magician2::Active()
{
	m_iPhase = 1;
	m_pState_Manager->ChangeState(new CBoss_Magician2::Intro_State(), this);
}

void CBoss_Magician2::Stun()
{
	m_pState_Manager->ChangeState(new CBoss_Magician2::Stun_State(), this);
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;

#pragma region Effect
	m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
	m_pGameInstance->Set_SlowWorld(true);
	m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

HRESULT CBoss_Magician2::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Magician2::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Magician2::BODY_MAGICIAN2_DESC BodyDesc{};
	BodyDesc.bMutation_Active = &m_bIntro_Mutation_Active;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.pParentState = &m_iMonster_State;
	BodyDesc.bMutation_Active = &m_bIntro_Mutation_Active;
	BodyDesc.bDead = &m_bDead;
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Magician2"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2_Body"), &BodyDesc)))
		return E_FAIL;

	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Magician2"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CWeapon_Magician2_Sword::MAGICIAN2_SWORD_DESC		Weapon_Desc{};
	Weapon_Desc.pParent = this;
	Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
	Weapon_Desc.pParentState = &m_iMonster_State;
	Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Desc.pParentModel = m_pModelCom;
	Weapon_Desc.bSpecial_Skill_Progress = &m_bSpecial_Skill_Progress;
	Weapon_Desc.bCatch_Special_Attack = &m_bCatch_Special_Attack;

	Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician2_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2_Sword"), &Weapon_Desc)))
		return E_FAIL;

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
	Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Locked_On_Desc.pParentState = &m_iMonster_State;
	Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
	Locked_On_Desc.fSpeedPerSec = 0.f;
	Locked_On_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
		return E_FAIL;

	CUI_Boss_HP_Bar::UI_BOSS_HP_BAR_DESC pBoss_HP_Bar = {};
	pBoss_HP_Bar.fMaxHP = &m_fMonsterMaxHP;
	pBoss_HP_Bar.fCurHP = &m_fMonsterCurHP;
	pBoss_HP_Bar.fShieldHP = &m_fShieldHP;
	pBoss_HP_Bar.bBoss_HP_Bar_Active = &m_bHP_Bar_Active;
	pBoss_HP_Bar.bBossDead = &m_bDead;
	pBoss_HP_Bar.iPhase = &m_iPhase;
	pBoss_HP_Bar.iCurLevel = iLevel;
	pBoss_HP_Bar.sBossName = TEXT("변이된 오두르");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;


	CBoss_Mutation_Magician_Camera::CAMERA_DESC Mutation_Magician_Camera_Desc = {};

	Mutation_Magician_Camera_Desc.pParent = this;
	Mutation_Magician_Camera_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("camera");
	Mutation_Magician_Camera_Desc.pParentState = &m_iMonster_State;
	Mutation_Magician_Camera_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Mutation_Magician_Camera_Desc.pParentModel = m_pModelCom;
	Mutation_Magician_Camera_Desc.fSpeedPerSec = 0.f;
	Mutation_Magician_Camera_Desc.fRotationPerSec = 0.f;
	Mutation_Magician_Camera_Desc.iCurLevel = pDesc->iCurLevel;
	Mutation_Magician_Camera_Desc.pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Mutation_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Mutation_Magician_Camera"), &Mutation_Magician_Camera_Desc)))
		return E_FAIL;

	CDecorative_Tonic::DECORATIVE_TONIC_DESC Tonic_Desc = {};
	Tonic_Desc.pParentState = &m_iMonster_State;
	Tonic_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("AnimTargetPoint");
	Tonic_Desc.pHandSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
	Tonic_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Tonic_Desc.bChange = &m_bIntro_Change_Socket;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Decorative_Tonic"), LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Tonic"), &Tonic_Desc)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Magician2::Near_Pattern_Create()
{
	_uint iRandomPattern = {};

	iRandomPattern = rand() % 7;

	while (true)
	{
		if ((iRandomPattern == m_iNearPatternIndex))
		{
			iRandomPattern = rand() % 7;
		}
		else
		{
			m_iNearPatternIndex = iRandomPattern;
			break;
		}
	}

	switch (m_iNearPatternIndex)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboA(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboB(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboC(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboE(), this);
		break;
	case 4:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboG(), this);
		break;
	case 5:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboH(), this);
		break;
	case 6:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboI(), this);
		break;
	}
}

void CBoss_Magician2::Far_Pattern_Create()
{
	_uint iRandomPattern = {};

	iRandomPattern = rand() % 3;

	while (true)
	{
		if ((iRandomPattern == m_iFarPatternIndex))
		{
			iRandomPattern = rand() % 3;
		}
		else
		{
			m_iFarPatternIndex = iRandomPattern;
			break;
		}
	}

	switch (m_iFarPatternIndex)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboF(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_ComboJ(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician2::Run_State(), this);
		break;
	}

}

void CBoss_Magician2::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
	{
		_uint iNoDamage = 1;
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;
		m_fMonsterCurHP -= *m_Player_Attack / 70.f;
		m_fShieldHP -= (*m_Player_Attack / 15.f);

		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			m_pGameInstance->Play_Sound(TEXT("Magician2_HitSound.wav"), CHANNELID::SOUND_MONSTER_WEAPON, 30.f);
			break;
		case 1:
			m_pGameInstance->Play_Sound(TEXT("Magician2_HitSound2.wav"), CHANNELID::SOUND_MONSTER_WEAPON, 30.f);
			break;
		}
	}
	if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
	{
		_uint iNoDamage = 1;
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;
		m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 5.f;
		m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 70.f;
		if (m_fMonsterCurHP <= m_fShieldHP)
		{
			m_fMonsterCurHP = m_fShieldHP;
		}
	}
}

void CBoss_Magician2::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Magician2::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Magician2* CBoss_Magician2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Magician2* pInstance = new CBoss_Magician2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Magician2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Magician2::Clone(void* pArg)
{
	CBoss_Magician2* pInstance = new CBoss_Magician2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Magician2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Magician2::Free()
{
	__super::Free();

	Safe_Release(m_pState_Manager);
}

void CBoss_Magician2::Intro_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 18;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_PlayerScreen"), false);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);


}

void CBoss_Magician2::Intro_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	#pragma region Effect
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Intro"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_Intro.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}

		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Intro_1")) //Intro Effect
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_INTRO_FIRST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Intro_2")) //Intro Effect
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_INTRO, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
		}
	}
#pragma endregion


	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 1000.f && pObject->m_bIntro_Change_Socket)
		{
			pObject->m_bIntro_Change_Socket = false;
		}
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 1800.f && !pObject->m_bIntro_Mutation_Active)
		{
			pObject->m_bIntro_Mutation_Active = true;
		}

		if (pObject->m_pModelCom->GetAniFinish())
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);

	}
}

void CBoss_Magician2::Intro_State::State_Exit(CBoss_Magician2* pObject)	
{
	pObject->m_bHP_Bar_Active = true;
	pObject->Delete_PartObject(TEXT("Part_Decorative_Tonic"));
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_PlayerScreen"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_Dialogue"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 1, 21);

	pObject->m_pGameInstance->PlayBGM(TEXT("charmer_music_C.ogg"), 0.3f);

#pragma region Effect
	const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
	pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_BURST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
#pragma endregion
}

void CBoss_Magician2::Idle_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 14;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->Set_LerpFinished(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Idle_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{



	if (m_iIndex == 14 &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
		pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f &&
		pObject->m_fDistance >= 10.f)
	{
		pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
	}

}

void CBoss_Magician2::Idle_State::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
}

void CBoss_Magician2::Stun_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 24;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_MUTATION_MAGICIAN;
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Stun_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	if (m_iIndex == 24 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 23;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}
	if ((m_iIndex == 23 || m_iIndex == 24) &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
		(*pObject->m_Player_State) == CPlayer::STATE_MAGICIAN_MUTATION_Execution)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::ExeCution_State(), pObject);
	}
}

void CBoss_Magician2::Stun_State::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Magician2::ExeCution_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 0;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);

	_float teleportDistance = 1.3f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

	pObject->m_pTransformCom->LookAt(vPlayerPos);
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->m_pTransformCom->LookAt(vPlayerPos);

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);

	/* 선환 추가 */
	pObject->m_pModelCom->Get_VecAnimation().at(0)->SetLerpTime(0.f);	
	pObject->m_pModelCom->Set_LerpFinished(true);	
	/* ============ */
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Execution"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_Execution.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}

		}
	}

	if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish() && pObject->m_iMonster_State != STATE_DEAD)
	{
		//사실 여기로 들어오면 그냥 죽은거임 ㅇㅇ	
		pObject->m_iMonster_State = STATE_DEAD;
		pObject->m_bHP_Bar_Active = false;
#pragma region Boss죽을시효과+UI
		pObject->m_pGameInstance->Set_Boss_Dead(true);
		pObject->m_pGameInstance->Set_Boss_Active(false);
		pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_Landing"), true);
		pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);
		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);

		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
		pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 23);

		// 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
		dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1999);
		pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1999);
		// 몬스터 사망 시 아이템 드랍 추가하기
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_6, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_7, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion

#pragma region Effect
		pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_BURST);
		pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_DUST_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM, 1.f);
#pragma endregion

	}
}

void CBoss_Magician2::ExeCution_State::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bExecution_Progress = false;
	pObject->m_IsStun = false;
}


void CBoss_Magician2::Run_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 17;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Run_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Run"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_Run.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}

		}
	}

	pObject->RotateDegree_To_Player();

	if (m_iIndex == 17 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 15;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}

	if (m_iIndex == 15 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 2.f)
	{
		m_iIndex = 16;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}

	if (m_iIndex == 16 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}

}

void CBoss_Magician2::Run_State::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
}

void CBoss_Magician2::Attack_ComboA::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 1;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	pObject->m_iMonster_Attack_Power = 71;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboA::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 55.f)
		{
			pObject->m_iPlayer_Hitted_State = PLAYER_HURT_KnockBackF;
			pObject->m_iMonster_Attack_Power = 114;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
		}
	}


#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboA.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Swing")) //1페이즈에서 애니메이션이 스왑되기에 여기에 하나더 추가
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Stab")) //1페이즈에서 애니메이션이 스왑되기에 여기에 하나더 추가
			{
				pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_STAB, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
				pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_STAB_DUST, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
				iter.isPlay = true;
			}

		}
	}

#pragma endregion

}

void CBoss_Magician2::Attack_ComboA::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboB::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 2;
	
	_uint iRandom = rand() % 2;
	switch (iRandom)
	{
	case 0:
		m_bAttack_V2 = true;
		break;
	case 1:
		m_bAttack_V2 = false;
		break;
	}
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 71;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboB::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	pObject->RotateDegree_To_Player();
	if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 113.f && m_bAttack_V2)
		{
			m_iIndex = 3;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 130.f)
		{
			pObject->m_iMonster_Attack_Power = 95;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
		}
	}
	else if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		pObject->m_iMonster_Attack_Power = 114;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
	}

#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				if (!m_bAttack_V2)
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboB_01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboB_02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);




					iter.isPlay = true;
				}


			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Stab"))
			{
				pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_STAB, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
				pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_STAB_DUST, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Impact"))
			{
				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MUTATION_IMPACT, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_MUTATION_COMBO_B, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Tentacle_FallBack"))
			{
				const _float4x4* matTentacle = pObject->m_pModelCom->Get_BoneMatrix("Bone_Tentacle03");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_TENTACLE_FALLBACK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matTentacle);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Charge_Blue"))
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE_BLUE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Charge"))
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Warning"))
			{
				pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
				const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
				_float4x4 matHeadWorld = {};
				XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}

}

void CBoss_Magician2::Attack_ComboB::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboC::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 4;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 71;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboC::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboC.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_SwingReverse"))
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGREVERSE_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_SwingAround"))
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGAROUND_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 65.f)
		{
			pObject->m_iMonster_Attack_Power = 95;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
		}
	}
}

void CBoss_Magician2::Attack_ComboC::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboE::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 6;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 71;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboE::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboE.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Tentacle_FallBack"))
			{
				const _float4x4* matTentacle = pObject->m_pModelCom->Get_BoneMatrix("Bone_Tentacle04");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_TENTACLE_FALLBACK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matTentacle);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}
}

void CBoss_Magician2::Attack_ComboE::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboF::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 7;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 114;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboF::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboF.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Tentacle"))
			{
				const _float4x4* matTentacle = pObject->m_pModelCom->Get_BoneMatrix("Bone_Tentacle06_End");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_TENTACLE_LEAPATTACK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matTentacle);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Tentacle_Smash"))
			{
				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_MUTATION_COMBO_F, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}
}

void CBoss_Magician2::Attack_ComboF::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboG::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 8;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 29;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboG::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
	pObject->RotateDegree_To_Player();

#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboG.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Burst"))
			{
				for (_uint i = 0; i < 2; i++)
				{
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MUTATION_BURST, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				}
				pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_BURST, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Charge_Blue"))
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE_BLUE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Warning"))
			{
				pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
				const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
				_float4x4 matHeadWorld = {};
				XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if(pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 70.f)
			pObject->RotateDegree_To_Player();

		if(pObject->m_pModelCom->GetAniFinish())
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);

	}
}

void CBoss_Magician2::Attack_ComboG::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Magician2::Attack_ComboH::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 9;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 142;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboH::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboH.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
			}
		}


		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Sweep"))
			{
				for (_uint i = 0; i < 2; i++)
				{
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MUTATION_SWEEP, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				}
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Charge"))
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}
}

void CBoss_Magician2::Attack_ComboH::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboI::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 10;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 71;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboI::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboI.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_SwingBack"))
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SWINGBACK_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}
}

void CBoss_Magician2::Attack_ComboI::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_ComboJ::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 11;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 114;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_ComboJ::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Attack"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_AttackComboJ.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Impact")) //1페이즈에서 애니메이션이 스왑되기에 여기에 하나더 추가
			{
				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MUTATION_IMPACT, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));

				pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_MUTATION_COMBO_J, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_Charge"))
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
	}
}

void CBoss_Magician2::Attack_ComboJ::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Attack_Special::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 19;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bSpecial_Skill_Progress = true;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_MUTATION_MAGICIAN_CATCH;	
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Attack_Special::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Special_Attack_Start"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_Special_Attack_Start.wav"), CHANNELID::SOUND_BOSS_ACTION, 1.f);
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Sound_Special_Attack_Catch"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Mutation2_Catch_Fail.wav"), CHANNELID::SOUND_BOSS_ACTION, 1.f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Special")) //1페이즈에서 애니메이션이 스왑되기에 여기에 하나더 추가
			{
				for (_uint i = 0; i < 5; i++)
				{
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_VARG_ROAR, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());

					//pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_MUTATION_SP_ATTACK, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
				}
				iter.isPlay = true;
			}
			else if (!strcmp(iter.szName, "Effect_SpecialCharge"))
			{
				const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
				pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SPECIALCHARGE_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 145.f)
			pObject->RotateDegree_To_Player();

		if (pObject->m_bCatch_Special_Attack && *pObject->m_Player_State == CPlayer::STATE_HURT_MUTATION_MAGICIAN_CATCH)
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Catch_State(), pObject);
		}
		else if (pObject->m_pModelCom->GetAniFinish())
		{
			//안잡히고 끝났을때
			pObject->m_fSpecial_Skill_CoolTime = 0.f;
			pObject->m_bSpecial_Skill_Progress = false;
			pObject->m_bCan_Move_Anim = false;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
		}
	}
}

void CBoss_Magician2::Attack_Special::State_Exit(CBoss_Magician2* pObject)
{
}

void CBoss_Magician2::Catch_State::State_Enter(CBoss_Magician2* pObject)
{
	m_iIndex = 20;
	pObject->m_iMonster_Attack_Power = 238;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK2;

	_float offsetRight = 0.7f;
	_float offsetBack = 0.4f;

	// 1. 플레이어 위치에서 오른쪽으로 이동
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector vPlayerRight = XMVector3Normalize(pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT));
	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerRight, offsetRight));

	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->m_pTransformCom->LookAt(vPlayerPos);


	_vector vPlayerLook = XMVector3Normalize(pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	vNewPos = XMVectorAdd(vNewPos, XMVectorScale(vPlayerLook, -offsetBack));
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);

	_vector vFocusPoint = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, -offsetBack));
	pObject->m_pTransformCom->LookAt(vFocusPoint);

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician2::Catch_State::State_Update(_float fTimeDelta, CBoss_Magician2* pObject)
{
#pragma region Effect

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
		{
			if (!strcmp(iter.szName, "Sound_Special_Attack_Catch"))
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician2_Catch.wav"), CHANNELID::SOUND_BOSS_ACTION, 1.f);
				iter.isPlay = true;
			}
		}

		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_SPAttack")) //1페이즈에서 애니메이션이 스왑되기에 여기에 하나더 추가
			{
				const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
				for (_uint i = 0; i < 3; i++)
				{
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN2_WORLD_SPECIAL_SUCCESS, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
				}
				iter.isPlay = true;
			}
		}
	}

#pragma endregion

	if (m_iIndex == 20 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f && !m_bAttack)
		{
			m_bAttack = true;
			static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician2::Idle_State(), pObject);
		}
	}
}

void CBoss_Magician2::Catch_State::State_Exit(CBoss_Magician2* pObject)
{
	pObject->m_bCatch_Special_Attack = false;
	pObject->m_bSpecial_Skill_Progress = false;
	pObject->m_bCan_Move_Anim = false;
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
