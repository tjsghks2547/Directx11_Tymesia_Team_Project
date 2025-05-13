#include "pch.h"
#include "Boss_Urd.h"
#include "Locked_On.h"
#include "Animation.h"
#include "GameInstance.h"
#include "Player.h"
#include "Body_Urd.h"
#include "Weapon_Urd_Sword.h"
#include "Stand_Stack_Sword.h"
#include "UI_Boss_HP_Bar.h"
#include "Boss_Urd_Camera.h"		

CBoss_Urd::CBoss_Urd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Urd::CBoss_Urd(const CBoss_Urd& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CBoss_Urd::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Urd::Initialize(void* pArg)
{
	m_Is_Boss = true;
	m_fRotateSpeed = 180.f;
	m_fRootDistance = 1.f;
	m_fActive_Distance = 15.f;
	m_iParryReadyHits = 3;

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
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pState_Manager = CState_Machine<CBoss_Urd>::Create();
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

	m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_JAVELINSWORD;

	return S_OK;
}

void CBoss_Urd::Priority_Update(_float fTimeDelta)
{
	if (m_bNeed_Decide_Step_Num && !m_bPatternProgress)
	{
		m_bNeed_Decide_Step_Num = false;
		if (m_iPhase == PHASE_ONE)
			m_iCheck_Step_Num = (rand() % 2) + 1;
		else if (m_iPhase == PHASE_TWO)
			m_iCheck_Step_Num = (rand() % 1) + 1;
	}

	if (m_iMonster_State != STATE_INTRO && !m_bExecution_Progress)
		RotateDegree_To_Player();
	
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Urd::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
		m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Urd::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Urd::Render()
{
	return S_OK;
}

void CBoss_Urd::State_Update(_float fTimeDelta)
{
	m_pState_Manager->State_Update(fTimeDelta, this);
}

void CBoss_Urd::PatternCreate()
{
	if (m_iPhase == PHASE_TWO)
		m_fSpecial_Skill_CoolTime += m_fTimeDelta;

	if (!m_bPatternProgress && !m_bSpecial_Skill_Progress && m_bActive && !m_bNeed_Decide_Step_Num && !m_IsStun)
	{
		if (m_iStep_Count < m_iCheck_Step_Num)
		{
			if (m_fDistance > 3.f)
			{
				m_pState_Manager->ChangeState(new CBoss_Urd::Step_Front_State(), this);
			}
			else
			{
				_uint iRandom = rand() % 2;
				switch (iRandom)
				{
				case 0:
					m_pState_Manager->ChangeState(new CBoss_Urd::Step_Right_State(), this);
					break;
				case 1:
					m_pState_Manager->ChangeState(new CBoss_Urd::Step_Left_State(), this);
					break;
				}
			}
		}

		else if (!m_bPattern70 && m_fMonsterCurHP <= 70.f)
		{
			m_bCan_Hit_Motion = false;
			m_bPattern70 = true;
			Stack_Skill_Create();
		}
		else if (!m_bPattern50 && m_fMonsterCurHP <= 50.f)
		{
			m_bCan_Hit_Motion = false;
			m_bPattern50 = true;
			Stack_Skill_Create();
		}
		else if (!m_bPattern30 && m_fMonsterCurHP <= 30.f)
		{
			m_bCan_Hit_Motion = false;
			m_bPattern30 = true;
			Stack_Skill_Create();
		}
		else
		{
			m_bCan_Hit_Motion = false;
			if (m_iSword_Stack_Count >= STACK_END && m_iPhase == PHASE_TWO)
			{
				m_iSword_Stack_Count = STACK_ONE;
				m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Special_Skill(), this);
			}
			else if (m_fSpecial_Skill_CoolTime >= 20.f)
			{
				m_fSpecial_Skill_CoolTime = 0.f;
				Stack_Skill_Create();
			}
			else
				Near_Pattern_Create();
		}
	}
}

void CBoss_Urd::Active()
{
	m_iPhase = PHASE_ONE;
	m_pState_Manager->ChangeState(new CBoss_Urd::Intro_State(), this);
}

void CBoss_Urd::Stun()
{
	m_pState_Manager->ChangeState(new CBoss_Urd::Stun_State(), this);
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;

#pragma region Effect
	m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
	m_pGameInstance->Set_SlowWorld(true);
	m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

HRESULT CBoss_Urd::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Urd::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Urd::BODY_URD_DESC BodyDesc{};
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Urd"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Body"), &BodyDesc)))
		return E_FAIL;


	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Urd"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CWeapon_Urd_Sword::URD_SWORD_DESC		Weapon_Desc{};
	Weapon_Desc.pParent = this;
	Weapon_Desc.pIntroSocketMatrix = m_pModelCom->Get_BoneMatrix("AnimTargetPoint");
	Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
	Weapon_Desc.pParentState = &m_iMonster_State;
	Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Desc.pParentModel = m_pModelCom;
	Weapon_Desc.bChange_Socket = &m_bChange_Socket;
	Weapon_Desc.bCollider_Change = &m_bStand_Stack_Sword_ColliderOn;
	Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Urd_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Sword"), &Weapon_Desc)))
		return E_FAIL;

	for (_uint i = 0; i < 3; i++)
	{
		wstring strName = L"Part_Stack_Sword" + to_wstring(i);
		string SocketName = string("SK_W_UrdSword0") + to_string(i + 2) + "_Point";
		_uint iStackNum = i;

		CStand_Stack_Sword::STAND_STACK_SWORD_DESC		Stack_Sword_Desc{};
		Stack_Sword_Desc.iCurLevel = iLevel;
		Stack_Sword_Desc.pParent = this;
		Stack_Sword_Desc.pParentModel = m_pModelCom;
		Stack_Sword_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
		Stack_Sword_Desc.pPlayerPos = &m_vPlayerPos;
		Stack_Sword_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix(SocketName.c_str());
		Stack_Sword_Desc.bNeed_Memory_Position = &m_bNeed_Memory_Position[i];
		Stack_Sword_Desc.bIs_Equipped_To_LeftHand = &m_bIs_Equipped_To_LeftHand[i];
		Stack_Sword_Desc.bIs_Stand_In_Ground = &m_bIs_Stand_In_Ground[i];
		Stack_Sword_Desc.iStack_Number = iStackNum;
		Stack_Sword_Desc.iCurrent_StackCount = &m_iSword_Stack_Count;
		Stack_Sword_Desc.bCollider_Change = &m_bStand_Stack_Sword_ColliderOn;
		Stack_Sword_Desc.iAttack = &m_iMonster_Attack_Power;
		Stack_Sword_Desc.pParentState = &m_iMonster_State;
		Stack_Sword_Desc.fSpeedPerSec = 10.f;
		Stack_Sword_Desc.fRotationPerSec = 0.f;

		if (FAILED(__super::Add_PartObject(strName.c_str(), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Stack_Sword"), &Stack_Sword_Desc)))
			return E_FAIL;
	}

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("pelvis-Spine2");
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
	pBoss_HP_Bar.sBossName = TEXT("우르드");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;


	CBoss_Urd_Camera::CAMERA_DESC Boss_Urd_Camera_Desc = {};

	Boss_Urd_Camera_Desc.pParent = this;
	Boss_Urd_Camera_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("camera");
	Boss_Urd_Camera_Desc.pParentState = &m_iMonster_State;
	Boss_Urd_Camera_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Boss_Urd_Camera_Desc.pParentModel = m_pModelCom;
	Boss_Urd_Camera_Desc.fSpeedPerSec = 0.f;
	Boss_Urd_Camera_Desc.fRotationPerSec = 0.f;
	Boss_Urd_Camera_Desc.iCurLevel = iLevel;
	Boss_Urd_Camera_Desc.pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (FAILED(__super::Add_PartObject(TEXT("Part_Urd_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Camera"), &Boss_Urd_Camera_Desc)))
		return E_FAIL;



	return S_OK;
}

void CBoss_Urd::Near_Pattern_Create()
{

	_uint iRandomPattern = {};

	iRandomPattern = rand() % 4;

	while (true)
	{
		if ((iRandomPattern == m_iNearPatternIndex))
		{
			iRandomPattern = rand() % 4;
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
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_A(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_B(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_C(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_D(), this);
		break;
	}

	m_fDelayTime = 0.f;
	m_iStep_Count = 0;
	m_bPatternProgress = true;
}

void CBoss_Urd::Stack_Skill_Create()
{
	_uint iRandom = rand() % 2;

	switch (iRandom)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Stack_Skill_01(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Stack_Skill_02(), this);
		break;
	}

	m_fDelayTime = 0.f;
	m_iStep_Count = 0;
	m_bPatternProgress = true;
}

void CBoss_Urd::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	/* 플레이어 무기와의 충돌 */
	if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
	{
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;

		if (m_iHitCount >= m_iParryReadyHits)
		{
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> ParryCount_Random(3, 5);
			uniform_int_distribution<> Random_Pattern(0, 1);
			m_iParryReadyHits = ParryCount_Random(gen);
			_uint iRandom = Random_Pattern(gen);
			if (iRandom == 0)
			{
				m_pState_Manager->ChangeState(new CBoss_Urd::Parry_State(), this);
			}
			else
			{
				Near_Pattern_Create();
			}
			m_iStep_Count = 0;
			m_iHitCount = 0;
			m_bCan_Hit_Motion = false;
			m_bPatternProgress = true;
			m_fDelayTime = 0.f;
			return;
		}

		if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= *m_Player_Attack / 100.f;
			m_fShieldHP -= (*m_Player_Attack /	20.f);
		}
		else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 9.f;
			m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 70.f;
			if (m_fMonsterCurHP <= m_fShieldHP)
			{
				m_fMonsterCurHP = m_fShieldHP;
			}
		}

		if (m_bCan_Hit_Motion &&
			!m_bPatternProgress &&
			!m_bSpecial_Skill_Progress &&
			m_fMonsterCurHP > 0.f &&
			m_iHitCount < m_iParryReadyHits)
		{
			_uint iRandom = rand() % 2;
			while (true)
			{
				if (iRandom == m_iHit_Motion_Index)
					iRandom = rand() % 2;
				else
				{
					m_iHit_Motion_Index = iRandom;
					break;
				}
			}
			m_pState_Manager->ChangeState(new CBoss_Urd::Hit_State(m_iHit_Motion_Index), this);
		}

#pragma region Sound
		_uint iRandSoundFileNum = {};
		iRandSoundFileNum = rand() % 4;

		switch (iRandSoundFileNum)
		{
		case 0:
			m_pGameInstance->Play_Sound(L"Armor_HitByPlayer1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.25f);
			break;
		case 1:
			m_pGameInstance->Play_Sound(L"Armor_HitByPlayer2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.25f);
			break;
		case 2:
			m_pGameInstance->Play_Sound(L"Armor_HitByPlayer3.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.25f);
			break;
		case 3:
			m_pGameInstance->Play_Sound(L"Armor_HitByPlayer4.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.25f);
			break;
		}
#pragma endregion
	}
}

void CBoss_Urd::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Urd::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Urd* CBoss_Urd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Urd* pInstance = new CBoss_Urd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Urd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Urd::Clone(void* pArg)
{
	CBoss_Urd* pInstance = new CBoss_Urd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Urd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Urd::Free()
{
	__super::Free();

	Safe_Release(m_pState_Manager);
}

void CBoss_Urd::Intro_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 24;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pGameInstance->Set_Boss_Active(true);
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_OCEAN, TEXT("Layer_PlayerScreen"), false);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);

	pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
}

void CBoss_Urd::Intro_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_INTRO
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Intro_01"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Intro_01.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Intro_02"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Intro_02.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Dust"))
				{
					_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_INTRO_DUST, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);

					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Explosion"))
				{
					_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);

					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);

					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 575.f && !pObject->m_bChange_Socket)
		{
			pObject->m_bChange_Socket = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Intro_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bHP_Bar_Active = true;
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_OCEAN, TEXT("Layer_PlayerScreen"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_OCEAN, TEXT("Layer_Dialogue"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 1, 41);

	pObject->m_pGameInstance->PlayBGM(TEXT("urd_music_2_A.ogg"), 0.3f);
}

void CBoss_Urd::Idle_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 19;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_bNeed_Decide_Step_Num = true;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Idle_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	pObject->RotateDegree_To_Player();
}

void CBoss_Urd::Idle_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}

void CBoss_Urd::Stun_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 40;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_URD;	
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->RotateDegree_To_Player();
	pObject->m_iHitCount = 0;

	//pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Stun_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 39;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}

	if ((m_iIndex == 39 || m_iIndex == 40) && (*pObject->m_Player_State) == CPlayer::STATE_URD_EXECUTION)	
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::ExeCution_State(), pObject);
	}
}

void CBoss_Urd::Stun_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Urd::ExeCution_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 41;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_pModelCom->Set_Continuous_Ani(true);

	_float teleportDistance = 1.f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->RotateDegree_To_Player();

	/* 선환 추가 */
	pObject->m_pModelCom->Get_VecAnimation().at(41)->SetLerpTime(0.f);
	pObject->m_pModelCom->Set_LerpFinished(true);
	pObject->m_pModelCom->Get_VecAnimation().at(41)->Set_StartOffSetTrackPosition(45.f);
	/* ============ */
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
	pObject->m_bExecution_Progress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_EXECUTION
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Execution_01"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Execution_01.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Execution_02"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Execution_02.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Blood_1"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_EXECUTION_BLOOD_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_2"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_EXECUTION_BLOOD_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 41 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_iPhase == PHASE_ONE && pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
			pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);
			pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
		}
		else if (pObject->m_iPhase == PHASE_TWO && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 210.f)
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Dead_State(), pObject);
		}
	}

}

void CBoss_Urd::ExeCution_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bExecution_Progress = false;
	if (pObject->m_iPhase == PHASE_ONE)
	{
		pObject->m_iPhase = PHASE_TWO;
		pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP;
		pObject->m_bCanRecovery = true;
		pObject->m_IsStun = false;

		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
		pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 44);

		pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
		pObject->m_pGameInstance->PlayBGM(TEXT("urd_music_2_B.ogg"), 0.3f);
	}
}

void CBoss_Urd::Step_Front_State::State_Enter(CBoss_Urd* pObject)
{
	iRandomMove = rand() % 4;

	switch (iRandomMove)
	{
	case 0:
		m_iIndex = 32;
		break;
	case 1:
		m_iIndex = 33;
		break;
	case 2:
		m_iIndex = 34;
		break;
	case 3:
		m_iIndex = 35;
		break;
	}
	pObject->m_iStep_Count += 1;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_bPatternProgress = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Front_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STEP
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Step"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Step.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Step"))
				{
					const _float4x4* matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_3, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_4, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion
	pObject->RotateDegree_To_Player();
	if ((m_iIndex == 32 || m_iIndex == 33 || m_iIndex == 34 || m_iIndex == 35) && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Front_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Step_Back_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 31;
	pObject->m_iStep_Count += 1;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Back_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STEP
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Step"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Step.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Step"))
				{
					const _float4x4* matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_3, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_4, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	pObject->RotateDegree_To_Player();
	if (m_iIndex == 31 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Back_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Step_Right_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 37;
	pObject->m_iStep_Count += 1;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Urd::Step_Right_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STEP
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Step"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Step.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Step"))
				{
					const _float4x4* matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_3, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_4, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	pObject->RotateDegree_To_Player();
	if (m_iIndex == 37 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Right_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Urd::Step_Left_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 36;
	pObject->m_iStep_Count += 1;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Left_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STEP
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Step"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Step.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Step"))
				{
					const _float4x4* matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("upperarm_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_3, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					matStep = pObject->m_pModelCom->Get_BoneMatrix("foot_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STEP_4, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matStep);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	pObject->RotateDegree_To_Player();
	if (m_iIndex == 36 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Left_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

#pragma region Attack_Combo_A (3번 연속 찌르기)

void CBoss_Urd::Attack_Combo_A::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 78;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STAB_TRIPLE
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack1"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Attack01.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Stab"))
				{
					const _float4x4* matWeapon_End = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_end");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STAB_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_End);
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_ATTACK_SMOKE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE_FAST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_iPhase == pObject->PHASE_ONE)
	{
		if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
	else if (pObject->m_iPhase == pObject->PHASE_TWO)
	{
		if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 73.f)
			{
				m_iIndex = 1;
				pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
				pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(5.f);
			}
		}

		if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 73.f)
			{
				_uint iRandom = rand() % 4;
				switch (iRandom)
				{
				case 0:
					pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_E(), pObject);
					break;
				case 1:
					pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_F(), pObject);
					break;
				case 2:
					pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_B(), pObject);
					break;
				case 3:
					pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_D(), pObject);
					break;
				}
			}
		}

	}
}

void CBoss_Urd::Attack_Combo_A::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion 


#pragma region Attack_Combo_B (올려치기 이후 찌르기)

void CBoss_Urd::Attack_Combo_B::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 7;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 185;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(0.f);
}

void CBoss_Urd::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SWING_STAB
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack5"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Attack05.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Swing"))
				{
					const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_WORLD_SWINGLEFT_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
					const _float4x4* matWeapon_End = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_end");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SWING_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_End);
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_ATTACK_SMOKE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;       // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Stab"))
				{
					const _float4x4* matWeapon_End = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_end");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_STAB_2_WIDE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_End);
					_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f;
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUST_NARROW, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);

					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE_FAST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_iPhase == PHASE_ONE)
	{
		if (m_iIndex == 7 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
	else if (pObject->m_iPhase == PHASE_TWO)
	{
		if (m_iIndex == 7 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f)
			{
				pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_E(), pObject);
			}
		}
	}
}

void CBoss_Urd::Attack_Combo_B::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion 


#pragma region Attack_Combo_C (대각선 이동 후에 찌르기)

void CBoss_Urd::Attack_Combo_C::State_Enter(CBoss_Urd* pObject)
{
	_uint iRandom = rand() % 2;
	switch (iRandom)
	{
	case 0:
		m_iIndex = 5;
		break;
	case 1:
		m_iIndex = 6;
		break;
	}
	pObject->m_iMonster_Attack_Power = 97;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
}

void CBoss_Urd::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STAB
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack3"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Attack03.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}
			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Stab"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					_float4x4 matWeapon_Calculate = {};
					XMStoreFloat4x4(&matWeapon_Calculate, XMLoadFloat4x4(matWeapon) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_URD_STAB_2, matWeapon_Calculate);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_ATTACK_SMOKE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE_FAST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if ((m_iIndex == 5 || m_iIndex == 6) && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Attack_Combo_C::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion 

#pragma region Attack_Combo_D (기다렸다가 빠르게 한번 찌르기 (초록색공격) 2페때는 한번 더찌름)

void CBoss_Urd::Attack_Combo_D::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 2;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 214;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_STAB
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack2"))
				{
					if (pObject->m_iPhase == PHASE_ONE)
					{
#pragma region Effect_Warning
						pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
						const _float4x4* matNeck = pObject->m_pModelCom->Get_BoneMatrix("neck_01");
						_float4x4 matNeckWorld = {};
						XMStoreFloat4x4(&matNeckWorld, XMLoadFloat4x4(matNeck) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
						pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matNeckWorld);
#pragma endregion


						pObject->m_pGameInstance->Play_Sound(L"Urd_Attack2_01.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						iter.isPlay = true;
					}
					else if (pObject->m_iPhase == PHASE_TWO)
					{
#pragma region Effect_Warning
						pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
						const _float4x4* matNeck = pObject->m_pModelCom->Get_BoneMatrix("neck_01");
						_float4x4 matNeckWorld = {};
						XMStoreFloat4x4(&matNeckWorld, XMLoadFloat4x4(matNeck) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
						pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matNeckWorld);
#pragma endregion

						pObject->m_pGameInstance->Play_Sound(L"Urd_Attack2_02.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						iter.isPlay = true;
					}
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Stab_Long"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					_float4x4 matWeapon_Calculate = {};
					XMStoreFloat4x4(&matWeapon_Calculate, XMLoadFloat4x4(matWeapon) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_URD_STAB_2, matWeapon_Calculate);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_ATTACK_SMOKE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE_FAST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_iPhase == PHASE_ONE)
	{
		if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->GetAniFinish())
				pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}

	else if (pObject->m_iPhase == PHASE_TWO)
	{
		if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f)
			{
				m_iIndex = 7;
				pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
				pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(80.f);
			}
		}

		if (m_iIndex == 7 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		{
			if (pObject->m_pModelCom->GetAniFinish())
			{
				pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
			}
		}
	}

}

void CBoss_Urd::Attack_Combo_D::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion 


#pragma endregion 

#pragma region Attack_Stack_Skill_01(한바퀴 점프 덤블링 하고 찍으면서 칼 꽂는 스킬)

void CBoss_Urd::Attack_Stack_Skill_01::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 26;
	pObject->m_iMonster_Attack_Power = 351;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->RotateDegree_To_Player();
	pObject->m_bStand_Stack_Sword_ColliderOn = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Stack_Skill_01::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SKILL
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Skill1"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Skill1.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_URD_STACK_SWORD, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_WORLD, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Explosion"))
				{
					_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_SMALL, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_URD_STACK_SWORD);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_URD_NORMAL_DAGGER_SKILL, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());

					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f &&
			pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() < 60.f)
		{
			//왼손으로 바껴라
			pObject->m_bIs_Equipped_To_LeftHand[pObject->m_iSword_Stack_Count] = true;
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 60.f &&
			pObject->m_iSword_Stack_Count < pObject->STACK_END &&
			!m_bIsSpawn)
		{
			m_bIsSpawn = true;
			//칼 꽂혀있는 상태에 컴바인드매트릭스 기억해라
			pObject->m_bNeed_Memory_Position[pObject->m_iSword_Stack_Count] = true;
			//칼 왼손에있는거 끄고 땅에 꽂혀있어라 
			pObject->m_bIs_Equipped_To_LeftHand[pObject->m_iSword_Stack_Count] = false;
			pObject->m_bIs_Stand_In_Ground[pObject->m_iSword_Stack_Count] = true;
			pObject->m_iSword_Stack_Count++;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Attack_Stack_Skill_01::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bStand_Stack_Sword_ColliderOn = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}


#pragma endregion 

#pragma region Attack_Stack_Skill_02 (빠르게 바로 칼 꽂는 스킬)

void CBoss_Urd::Attack_Stack_Skill_02::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 27;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 311;
	pObject->RotateDegree_To_Player();
	pObject->m_bStand_Stack_Sword_ColliderOn = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(20.f);
}

void CBoss_Urd::Attack_Stack_Skill_02::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SKILL
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Skill2"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Skill1.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_URD_STACK_SWORD, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_WORLD, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Explosion"))
				{
					_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_EXPLOSION_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), vLook);
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_SMALL, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_URD_STACK_SWORD);

					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_URD_NORMAL_DAGGER_SKILL, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f &&
			pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() < 60.f)
		{
			//왼손으로 바껴라
			pObject->m_bIs_Equipped_To_LeftHand[pObject->m_iSword_Stack_Count] = true;
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 60.f &&
			pObject->m_iSword_Stack_Count < pObject->STACK_END &&
			!m_bIsSpawn)
		{
			m_bIsSpawn = true;
			//칼 꽂혀있는 상태에 컴바인드매트릭스 기억해라
			pObject->m_bNeed_Memory_Position[pObject->m_iSword_Stack_Count] = true;
			//칼 왼손에있는거 끄고 땅에 꽂혀있어라 
			pObject->m_bIs_Equipped_To_LeftHand[pObject->m_iSword_Stack_Count] = false;
			pObject->m_bIs_Stand_In_Ground[pObject->m_iSword_Stack_Count] = true;
			pObject->m_iSword_Stack_Count++;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Attack_Stack_Skill_02::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bStand_Stack_Sword_ColliderOn = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion 


#pragma region Attack_Special_Skill(스택 3개 이상되어서 이제 터져야할 때)

void CBoss_Urd::Attack_Special_Skill::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 30;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 999;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_bStand_Stack_Sword_ColliderOn = true;
	pObject->m_bSpecial_Skill_Progress = true;
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Special_Skill::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SPECIAL
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Special_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Special_Attack.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else if (!strcmp(iter.szName, "Sound_Special_Attack2"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Special_Attack2.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}
			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Continue"))
				{
					const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SP_START, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
					const _float4x4* matWeapon_l = pObject->m_pModelCom->Get_BoneMatrix("weapon_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SP_START_CONTINUE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_l);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_SP_1"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SP_READY, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}

				else if (!strcmp(iter.szName, "Effect_SP_2"))
				{
					for (_uint i = 0; i < 3; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_URD_SP_EXPLOSION, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					}

					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_MAIN, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_URD_SPECIAL_SKILL, *pObject->Get_Transfrom()->Get_WorldMatrix_Ptr());
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 310.f)
		{
			for (_uint i = STACK_ONE; i < STACK_END; i++)
			{
				pObject->m_bIs_Stand_In_Ground[i] = false;
			}
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}

	}
}

void CBoss_Urd::Attack_Special_Skill::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
	pObject->m_bStand_Stack_Sword_ColliderOn = false;
	pObject->m_bSpecial_Skill_Progress = false;
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
}

#pragma endregion


#pragma region Attack_Combo_E(공격 끝난 뒤에 한번 더 회전하면서 베는 공격)

void CBoss_Urd::Attack_Combo_E::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 8;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 271;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_E::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SWING
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack6"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Attack06.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_WORLD_SWING_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Swing_2"))
				{
					const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_WORLD_SWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 8 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Attack_Combo_E::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion 

#pragma region Attack_Combo_F(공격 끝난 뒤에 한번 더 올려치기 공격)
void CBoss_Urd::Attack_Combo_F::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 9;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}


void CBoss_Urd::Attack_Combo_F::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region EFFECT_SWING
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack7"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Attack07.wav", CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("weapon_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_WORLD_SWING_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Swing_2"))
				{
					const _float4x4* matRoot = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_WORLD_SWINGRIGHT_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRoot);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 9 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Attack_Combo_F::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion 

CBoss_Urd::Hit_State::Hit_State(_uint iHit_Index)
{
	m_iHit_Index = iHit_Index;
}

void CBoss_Urd::Hit_State::State_Enter(CBoss_Urd* pObject)
{
	switch (m_iHit_Index)
	{
	case 0:
		m_iIndex = 15;
		break;
	case 1:
		m_iIndex = 16;
		break;
	}
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iHitCount++;
	pObject->RotateDegree_To_Player();
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Hit_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Hit"))
				{
					_uint iRandom = rand() % 3;
					switch (iRandom)
					{
					case 0:
						pObject->m_pGameInstance->Play_Sound(L"Urd_Hurt01.wav", CHANNELID::SOUND_ACTION, 0.5f);
						break;
					case 1:
						pObject->m_pGameInstance->Play_Sound(L"Urd_Hurt02.wav", CHANNELID::SOUND_ACTION, 0.5f);
						break;
					case 2:
						pObject->m_pGameInstance->Play_Sound(L"Urd_Hurt03.wav", CHANNELID::SOUND_ACTION, 0.5f);
						break;
					}
					iter.isPlay = true;
				}
			}
		}
	}

	if ((m_iIndex == 15 || m_iIndex == 16) && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CBoss_Urd::Hit_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bPatternProgress = false;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}

void CBoss_Urd::Parry_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 21;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->m_bPatternProgress = true;
	pObject->m_iMonster_State = MONSTER_STATE::STATE_PARRY;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Parry_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Parry"))
				{
					pObject->m_pGameInstance->Play_Sound(L"Urd_Parry.wav", CHANNELID::SOUND_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
			{
				if (!strcmp(iter.szName, "Effect_Parry"))
				{
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_PARRY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					iter.isPlay = true;
				}
			}
		}
	}

	if (m_iIndex == 21 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 4;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_A(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_B(), pObject);
			break;
		case 2:
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_C(), pObject);
			break;
		case 3:
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_D(), pObject);
			break;
		}
	}
}

void CBoss_Urd::Parry_State::State_Exit(CBoss_Urd* pObject)
{

}

void CBoss_Urd::Dead_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 10;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bHP_Bar_Active = false;
	pObject->m_iMonster_State = STATE_DEAD;

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Dead_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
#pragma region Effect_Dead

	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)
		{
			if (!strcmp(iter.szName, "Effect_Dead"))
			{
				pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_VARG_DEAD_BLINK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_DUST_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
				pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_SP_START_CONTINUE);
				iter.isPlay = true;
			}
		}
	}
#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_bDead = true;
		pObject->m_bActive = false;
#pragma region Boss죽을시효과+UI
		pObject->m_pGameInstance->Set_Boss_Dead(true);
		pObject->m_pGameInstance->Set_Boss_Active(false);
		pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_OCEAN, TEXT("Layer_Landing"), true);
		pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);
		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);


		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
		pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 43);

		// 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
		dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1999);
		pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1999);
		// 몬스터 사망 시 아이템 드랍 추가하기
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_5, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);

		pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
#pragma endregion
	}

}

void CBoss_Urd::Dead_State::State_Exit(CBoss_Urd* pObject)
{
}
