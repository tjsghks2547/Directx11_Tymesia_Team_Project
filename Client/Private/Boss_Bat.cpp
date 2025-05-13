#include "pch.h"
#include "Boss_Bat.h"
#include "GameInstance.h"
#include "Body_Bat.h"
#include "Decorative_Spike.h"
#include "Locked_On.h"
#include "Player.h"
#include "UI_Boss_HP_Bar.h"
#include "Projectile_Air.h"
#include "Bat_Spike.h"
#include "Animation.h"
#include "Boss_Bat_Camera.h"

CBoss_Bat::CBoss_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Bat::CBoss_Bat(const CBoss_Bat& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CBoss_Bat::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Bat::Initialize(void* pArg)
{
	m_Is_Boss = true;
	m_fRotateSpeed = 180.f;
	m_fRootDistance = 4.5f;
	m_fActive_Distance = 25.f;

	Load_Spike_SpawnPoints();

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

	m_pState_Manager = CState_Machine<CBoss_Bat>::Create();
	if (m_pState_Manager == nullptr)
		return E_FAIL;

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 3.f,3.f,3.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });

	m_pGameInstance->Add_Actor_Scene(m_pActor);

	m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_SCYTHE;

	return S_OK;
}

void CBoss_Bat::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Bat::Update(_float fTimeDelta)
{
	if (m_iMonster_State == STATE_INTRO)	
		m_fNaviOffset = 1.f;	

	else
		m_fNaviOffset = 0.f;	
	

	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 300.f,100.f,1.f });
}

void CBoss_Bat::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Bat::Render()
{
	return S_OK;
}

void CBoss_Bat::State_Update(_float fTimeDelta)
{
	m_pState_Manager->State_Update(fTimeDelta, this);
}

void CBoss_Bat::Active()
{
	m_iPhase = PHASE_TWO;
	m_pState_Manager->ChangeState(new CBoss_Bat::Intro_State(), this);
}

void CBoss_Bat::Stun()
{
	m_pState_Manager->ChangeState(new CBoss_Bat::Stun_State(), this);
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;

#pragma region Effect
	m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
	m_pGameInstance->Set_SlowWorld(true);
	m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

HRESULT CBoss_Bat::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Bat::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Bat::BODY_BAT_DESC BodyDesc{};
	BodyDesc.pParent = this;
	BodyDesc.iAttack = &m_iMonster_Attack_Power;
	BodyDesc.pParentState = &m_iMonster_State;
	BodyDesc.bDead = &m_bDead;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Bat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat_Body"), &BodyDesc)))
		return E_FAIL;


	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Bat"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("Bip001-Pelvis");
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
	pBoss_HP_Bar.sBossName = TEXT("매달린 여왕");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;

	CBoss_Bat_Camera::CAMERA_DESC Boss_Bat_CameraDesc = {};

	Boss_Bat_CameraDesc.pParent = this;
	Boss_Bat_CameraDesc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("camera");
	Boss_Bat_CameraDesc.pParentState = &m_iMonster_State;
	Boss_Bat_CameraDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Boss_Bat_CameraDesc.pParentModel = m_pModelCom;
	Boss_Bat_CameraDesc.fSpeedPerSec = 0.f;
	Boss_Bat_CameraDesc.fRotationPerSec = 0.f;
	Boss_Bat_CameraDesc.iCurLevel = pDesc->iCurLevel;
	Boss_Bat_CameraDesc.pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (FAILED(__super::Add_PartObject(TEXT("Part_Bat_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat_Camera"), &Boss_Bat_CameraDesc)))
		return E_FAIL;

	_float3 fRotateDegree[6] = {};
	_float4 fPosition[6] = {};

	fRotateDegree[0] = { 25.f,00.f,10.f };
	fPosition[0] = { 100.f,1.f,-50.f,1.f };

	fRotateDegree[1] = { 40.f,00.f,10.f };
	fPosition[1] = { 100.f,1.f,-50.f,1.f };

	fRotateDegree[2] = { -40.f,00.f,-10.f };
	fPosition[2] = { 100.f,1.f,50.f,1.f };

	fRotateDegree[3] = { -20.f,00.f,-0.f };
	fPosition[3] = { 90.f,1.f,10.f,1.f };

	fRotateDegree[4] = { 10.f,00.f,-35.f };
	fPosition[4] = { 50.f,1.f,10.f,1.f };

	fRotateDegree[5] = { -20.f,00.f,-35.f };
	fPosition[5] = { 10.f,1.f,20.f,1.f };


	for (_uint i = 0; i < 6; i++)
	{
		wstring strName = L"Part_Decorative_Spike" + to_wstring(i);
		CDecorative_Spike::DECORATIVE_SPIKE_DESC Deco_Desc{};
		Deco_Desc.fPosition = _float4(1.f, 1.f, 1.f, 1.f);
		Deco_Desc.pRotate_Degree = &fRotateDegree[i];
		Deco_Desc.pPosition = &fPosition[i];
		Deco_Desc.pRender = &m_bCristal_Create;
		Deco_Desc.fScaling = _float3(0.4f, 0.001f, 0.4f);
		Deco_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
		Deco_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
		Deco_Desc.pParentState = &m_iMonster_State;
		Deco_Desc.fSpeedPerSec = 0.f;
		Deco_Desc.fRotationPerSec = 0.f;

		if (FAILED(__super::Add_PartObject(strName.c_str(), LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Spike"), &Deco_Desc)))
			return E_FAIL;
	}

	CProjectile_Air::PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pParent = this;
	ProjectileDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	ProjectileDesc.iCurLevel = iLevel;
	ProjectileDesc.fDelete_Time = &m_fAir_Delete_Time;
	ProjectileDesc.iDamage = &m_iMonster_Attack_Power;
	ProjectileDesc.fSpeedPerSec = 30.f;
	ProjectileDesc.fRotationPerSec = 0.f;
	ProjectileDesc.fPosition = m_vSpawnPoint;

	if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Air"), PROJECTILE_AIR, &ProjectileDesc)))
		return E_FAIL;

	_uint iRandom = rand() % 3;
	switch (iRandom)
	{
	case 0:
		m_vSpike_Positions = m_vSpike_Spawn_Positions_To_Three[0];
		break;
	case 1:
		m_vSpike_Positions = m_vSpike_Spawn_Positions_To_Three[1];
		break;
	case 2:
		m_vSpike_Positions = m_vSpike_Spawn_Positions_To_Three[2];
		break;
	}

	for (_uint i = 0; i < 10; i++)
	{
		wstring strName = L"Part_Bat_Spike" + to_wstring(i);
		CBat_Spike::BAT_SPIKE_DESC Spike_Desc{};
		Spike_Desc.fPosition = _float4(1.f, 1.f, 1.f, 1.f);
		Spike_Desc.fScaling = _float3(0.0025f, 0.0025f, 0.0025f);
		Spike_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
		Spike_Desc.pParentState = &m_iMonster_State;
		Spike_Desc.bRender = &m_bSummon_Spike;
		Spike_Desc.pPivot_Position = &m_vSpike_Positions[i];
		Spike_Desc.fSpeedPerSec = 0.f;
		Spike_Desc.fRotationPerSec = 0.f;

		if (FAILED(__super::Add_PartObject(strName.c_str(), LEVEL_STATIC, TEXT("Prototype_GameObject_Bat_Spike"), &Spike_Desc)))
			return E_FAIL;
	}

	return S_OK;
}

void CBoss_Bat::PatternCreate()
{
	if (m_iPhase == PHASE_TWO && m_bActive && !m_bSpecial_Skill_Progress)
	{
		m_fRecovery_Skill_CoolTime += m_fTimeDelta;
		m_fSpecial_Skill_CoolTime += m_fTimeDelta;
	}

	if (!m_bPatternProgress && !m_bSpecial_Skill_Progress && m_bActive && !m_IsStun)
	{
		m_fDelayTime += 5 * m_fTimeDelta;
		if (m_fDelayTime >= m_fCoolTime)
		{
			if (m_fRecovery_Skill_CoolTime >= 30.f && !m_bCristal_Create)
			{
				m_pState_Manager->ChangeState(new CBoss_Bat::Recovery_State(), this);
			}
			else if (m_fSpecial_Skill_CoolTime >= 60.f && m_bCristal_Create)
			{
				m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Special(), this);
			}
			else if (m_fDistance >= 10.f)
				Far_Pattern_Create();
			else
			{
				Near_Pattern_Create();
			}

			m_fDelayTime = 0.f;
			m_bPatternProgress = true;
		}
	}
}

void CBoss_Bat::Far_Pattern_Create()
{
	_uint iRandomPattern = {};

	if (m_bSummon_Spike)
		iRandomPattern = rand() % 4;
	else
		iRandomPattern = rand() % 3;


	while (true)
	{
		if (iRandomPattern == m_iFarPatternIndex)
		{
			if (m_bSummon_Spike)
				iRandomPattern = rand() % 4;
			else
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
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_E(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_G(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_H(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_F(), this);
		break;
	}
}

void CBoss_Bat::Near_Pattern_Create()
{
	_uint iRandomPattern = {};

	if (m_bSummon_Spike)
		iRandomPattern = rand() % 8;
	else
		iRandomPattern = rand() % 9;

	while (true)
	{
		if (iRandomPattern == m_iNearPatternIndex)
		{
			if (m_bSummon_Spike)
				iRandomPattern = rand() % 8;
			else
				iRandomPattern = rand() % 9;
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
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_A(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_B(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_C(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_D(), this);
		break;
	case 4:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_E(), this);
		break;
	case 5:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_G(), this);
		break;
	case 6:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_I(), this);
		break;
	case 7:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_H(), this);
		break;
	case 8:
		m_pState_Manager->ChangeState(new CBoss_Bat::Attack_Combo_F(), this);
		break;
	}
}

void CBoss_Bat::Load_Spike_SpawnPoints()
{
	string strDataPath = "../Bin/DataFiles/SpawnPoint/Spawn_Bat_Spike.txt";
	_tchar		szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);
	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load SpawnPoint File!");
		return;
	}

	DWORD dwByte = 0;
	_uint iSize = {};

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iSize; i++)
	{
		_float4 vMonsterPos = {};
		ReadFile(hFile, &vMonsterPos, sizeof(_float4), &dwByte, nullptr);
		m_vSpike_Spawn_Positions.push_back(vMonsterPos);
	}

	CloseHandle(hFile);

	_uint iTest = {};
	m_vSpike_Spawn_Positions_To_Three[0].resize(10);
	m_vSpike_Spawn_Positions_To_Three[1].resize(10);
	m_vSpike_Spawn_Positions_To_Three[2].resize(10);

	for (_uint i = 0; i < 3; i++)
	{
		for (_uint j = 0; j < 10; j++)
		{
			m_vSpike_Spawn_Positions_To_Three[i][j] = m_vSpike_Spawn_Positions[iTest];
			iTest += 1;
		}
	}

}

void CBoss_Bat::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	/* 플레이어 무기와의 충돌 */
	if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
	{
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;
		if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= *m_Player_Attack / 85.f;
			m_fShieldHP -= (*m_Player_Attack / 20.f);
		}
		else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 8.f;
			m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 70.f;
			if (m_fMonsterCurHP <= m_fShieldHP)
			{
				m_fMonsterCurHP = m_fShieldHP;
			}
		}

		_uint iRandom = rand() % 3;
		switch (iRandom)
		{
		case 0:
			m_pGameInstance->Play_Sound(TEXT("Bat_HitSound01.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 2.f);
			break;
		case 1:
			m_pGameInstance->Play_Sound(TEXT("Bat_HitSound02.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 2.f);
			break;
		case 2:
			m_pGameInstance->Play_Sound(TEXT("Bat_HitSound03.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 2.f);
			break;
		}
	}
}

void CBoss_Bat::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Bat::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Bat* CBoss_Bat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Bat* pInstance = new CBoss_Bat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Bat::Clone(void* pArg)
{
	CBoss_Bat* pInstance = new CBoss_Bat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Bat::Free()
{
	__super::Free();

	Safe_Release(m_pState_Manager);
}

#pragma region Intro_State

void CBoss_Bat::Intro_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 21;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pGameInstance->Set_Boss_Active(true);
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), false);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);

	pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	pObject->m_pGameInstance->PlayBGM(TEXT("Blood_Bat_-_Intro.ogg"), 0.15f);
}

void CBoss_Bat::Intro_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{

#pragma region EFFECT_INTRO
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Intro"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Intro.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Scream"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("head");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_RED_DUST_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_JumpDown"))
				{
					const _float4x4* matLHand = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_LEFT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matLHand);

					const _float4x4* matRHand = pObject->m_pModelCom->Get_BoneMatrix("hand_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_RIGHT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRHand);
				}
			}
		}
	}
#pragma endregion
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
	}
}

void CBoss_Bat::Intro_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bHP_Bar_Active = true;
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_Dialogue"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 1, 31);

	pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	pObject->m_pGameInstance->PlayBGM(TEXT("Blood_Bat_-_loop.ogg"), 0.15f);
}

#pragma endregion 

#pragma region Idle_State

void CBoss_Bat::Idle_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 16;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Idle_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
	pObject->RotateDegree_To_Player();
}

void CBoss_Bat::Idle_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion 

#pragma region Move_State

void CBoss_Bat::Move_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 33;
	pObject->m_bPatternProgress = false;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CBoss_Bat::Move_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}
		}
	}

	pObject->RotateDegree_To_Player();
	if (m_iIndex == 33 && pObject->m_fDistance > pObject->m_fRootDistance)
		pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
}

void CBoss_Bat::Move_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion 

#pragma region Stun_State

void CBoss_Bat::Stun_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 27;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_BAT;	
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->RotateDegree_To_Player();

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Bat::Stun_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{

	if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 26;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}

	if ((m_iIndex == 27 || m_iIndex == 26) && (*pObject->m_Player_State) == CPlayer::STATE_BAT_EXECUTION)	
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Execution_State(), pObject);
	}
}

void CBoss_Bat::Stun_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Execution_State

void CBoss_Bat::Execution_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 30;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_pModelCom->Set_Continuous_Ani(true);

	_float teleportDistance = 5.5f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

	/* 선환 추가 */
	pObject->m_pModelCom->Get_VecAnimation().at(30)->SetLerpTime(0.f);	
	pObject->m_pModelCom->Set_LerpFinished(true);	
	/* =========== */

	pObject->m_pTransformCom->LookAt(vPlayerPos);
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->m_pTransformCom->LookAt(vPlayerPos);

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

	pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	pObject->m_pGameInstance->PlayBGM(TEXT("Blood_Bat_-_End.ogg"), 0.15f);
}

void CBoss_Bat::Execution_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{

#pragma region EFFECT_EXECUTION
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Execution1"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Execution01.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Execution2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Execution02.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}

			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Blood_1"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("head");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_RED_DUST_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_2"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("head");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_SALIVIA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         


					pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_VARG_DEAD_BLINK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_DUST_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_DEAD, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
					iter.isPlay = true;

				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Dead_State(), pObject);
	}
}

void CBoss_Bat::Execution_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;

	pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
}

#pragma endregion

#pragma region Dead_State

void CBoss_Bat::Dead_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 29;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iMonster_State = STATE_DEAD;
	pObject->m_bPatternProgress = true;
	pObject->m_fDelayTime = 0.f;
	pObject->m_bHP_Bar_Active = false;
	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

#pragma region Boss죽을시효과+UI
	pObject->m_pGameInstance->Set_Boss_Dead(true);
	pObject->m_pGameInstance->Set_Boss_Active(false);
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_Landing"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);
	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);

	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 33);



	// 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
	dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1999);
	pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1999);
	// 몬스터 사망 시 아이템 드랍 추가하기
	pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
	pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
	pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
	pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
	pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion









}

void CBoss_Bat::Dead_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
	if (m_iIndex == 29 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
#pragma region Boss죽을시효과+UI
		//pObject->m_pGameInstance->Set_Boss_Dead(true);
		//pObject->m_pGameInstance->Set_Boss_Active(false);
		//pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_Landing"), true);
		//pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);
		//pObject->m_pGameInstance->Set_All_UIObject_Condition_Open(pObject->m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_3Recall")), true);
		//
		//pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
		//pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 33);
		//
		//
		//
		//// 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
		//dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1999);
		//pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1999);
		//// 몬스터 사망 시 아이템 드랍 추가하기
		//pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		//pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		//pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		//pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
		//pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion


#pragma endregion
	}
}

void CBoss_Bat::Dead_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bActive = false;
}

#pragma endregion

#pragma region Attack_Combo_A (오른손 긁기 패턴(Index 4))

void CBoss_Bat::Attack_Combo_A::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 4;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 334;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region EFFECT_CLAW
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboAB_01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Attack2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboAB_02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matClaw = pObject->m_pModelCom->Get_BoneMatrix("thumb_01_r");
					_float4x4 matClawWorld = {};
					XMStoreFloat4x4(&matClawWorld, XMLoadFloat4x4(matClaw) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_CLAW_R, matClawWorld);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_BAT_HANDATTACK_RIGHT, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_ScratchBack"))
				{
					const _float4x4* matRHand = pObject->m_pModelCom->Get_BoneMatrix("hand_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_RIGHT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRHand);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 153.f)
		{
			m_iIndex = 6;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
	}
	if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}
		}
	}

}

void CBoss_Bat::Attack_Combo_A::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_B (왼손 긁기 패턴 (Index 0))

void CBoss_Bat::Attack_Combo_B::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 334;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region EFFECT_CLAW
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboAB_01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Attack2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboAB_02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matClaw = pObject->m_pModelCom->Get_BoneMatrix("thumb_01_l");
					_float4x4 matClawWorld = {};
					XMStoreFloat4x4(&matClawWorld, XMLoadFloat4x4(matClaw) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_CLAW_L, matClawWorld);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_BAT_HANDATTACK_LEFT, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_ScratchBack"))
				{
					const _float4x4* matLHand = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_LEFT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matLHand);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 306.f)
		{
			m_iIndex = 2;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
	}
	if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}
		}
	}
}

void CBoss_Bat::Attack_Combo_B::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_C (두번 물기 패턴 (Index 7))

void CBoss_Bat::Attack_Combo_C::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 7;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 354;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{

#pragma region EFFECT_BITE_TWICE
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboC_02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Attack2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboC_03.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start_1"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_SALIVIA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Start_2"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_SALIVIA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 7 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
			break;
		}
	}

}

void CBoss_Bat::Attack_Combo_C::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_D (한번 물기 패턴 (Index 8))

void CBoss_Bat::Attack_Combo_D::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 8;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 354;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region EFFECT_BITE_ONCE
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboC_01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_SALIVIA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 8 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Bat::Attack_Combo_D::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_E (뒤로 뺐다 돌진 패턴(Index 9))

void CBoss_Bat::Attack_Combo_E::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 9;
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 421;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_E::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region Effect_Rush
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboE.wav"), CHANNELID::SOUND_BOSS_ACTION, 1.f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Rush_Right_1"))
				{
					const _float4x4* matRHand = pObject->m_pModelCom->Get_BoneMatrix("hand_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_RIGHT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRHand);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Rush_Right_2"))
				{
					const _float4x4* matRHand = pObject->m_pModelCom->Get_BoneMatrix("hand_r");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_RIGHT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRHand);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Rush_Left_1"))
				{
					const _float4x4* matLHand = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_LEFT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matLHand);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Rush_Left_2"))
				{
					const _float4x4* matClaw = pObject->m_pModelCom->Get_BoneMatrix("thumb_01_l");
					_float4x4 matClawWorld = {};
					XMStoreFloat4x4(&matClawWorld, XMLoadFloat4x4(matClaw) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_CLAW_L, matClawWorld);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_BAT_HANDATTACK_LEFT, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));


					const _float4x4* matLHand = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_HAND_LEFT_PYUNGTA, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matLHand);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 9 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Bat::Attack_Combo_E::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Attack_Combo_F (기둥 소환 패턴(HellScream (Index 13)))

void CBoss_Bat::Attack_Combo_F::State_Enter(CBoss_Bat* pObject)
{
	_uint iRandom = rand() % 3;
	switch (iRandom)
	{
	case 0:
		pObject->m_vSpike_Positions = pObject->m_vSpike_Spawn_Positions_To_Three[0];
		break;
	case 1:
		pObject->m_vSpike_Positions = pObject->m_vSpike_Spawn_Positions_To_Three[1];
		break;
	case 2:
		pObject->m_vSpike_Positions = pObject->m_vSpike_Spawn_Positions_To_Three[2];
		break;
	}
	pObject->m_bSummon_Spike = false;
	m_iIndex = 13;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 0;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF; //임시 나중에 무슨 칼드는모션?으로 나오게해야함
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_F::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region Effect_Blood
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Spawn_Spike"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboF.wav"), CHANNELID::SOUND_BOSS_ACTION, 1.f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Blood"))
				{
					const _float4x4* matNeck = pObject->m_pModelCom->Get_BoneMatrix("neck_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_SESU, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matNeck);
					iter.isPlay = true;      // 
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 430.f && !pObject->m_bSummon_Spike)
		{
			pObject->m_bSummon_Spike = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}
		}
	}
}

void CBoss_Bat::Attack_Combo_F::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_G (점프뛰어서 짓누르는 패턴?(Index 17, 18(앞으로) 19(제자리)))

void CBoss_Bat::Attack_Combo_G::State_Enter(CBoss_Bat* pObject)
{
	if (pObject->m_bCristal_Create)
	{
		m_iIndex = 17;
		pObject->m_iMonster_Attack_Power = 621;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
		pObject->m_bCristal_Create = false;
	}
	else if (pObject->m_fDistance >= 10.f)
	{
		m_iIndex = 18;
		pObject->m_iMonster_Attack_Power = 341;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	}
	else
	{
		m_iIndex = 19;
		pObject->m_iMonster_Attack_Power = 341;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
	}
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_G::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region EFFECT_CLAW + CHEST
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					switch (m_iIndex)
					{
					case 17:
						pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboG_Chest.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						iter.isPlay = true;
						break;
					case 18:
					case 19:
						pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboG.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						iter.isPlay = true;
						break;
					}
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matClaw = pObject->m_pModelCom->Get_BoneMatrix("thumb_01_l");
					_float4x4 matClawWorld = {};
					XMStoreFloat4x4(&matClawWorld, XMLoadFloat4x4(matClaw) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_CLAW_L, matClawWorld);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_BAT_HANDATTACK_LEFT, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Smash"))
				{
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_BAT_HANDATTACK_JUMP_SMASH_CHEST_ATTACK, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_CHESTATTACK_LEFT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_CHESTATTACK_RIGHT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Bat::Attack_Combo_G::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_H (SonicBoom?아마 공기팡?(Index 22))

void CBoss_Bat::Attack_Combo_H::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 22;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 568;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_H::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboH.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}
		}
	}

	if (m_iIndex == 22 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (!m_IsFired)
			pObject->RotateDegree_To_Player();

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 230.f && !m_IsFired)
		{
			m_IsFired = true;
			_vector pPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector pPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
			_float fPosY = XMVectorGetY(pPos);
			_float fPlayerPosY = XMVectorGetY(pPlayerPos);
			fPosY += 0.5f;
			fPlayerPosY += 0.5f;
			pPos = XMVectorSetY(pPos, fPosY);
			pPlayerPos = XMVectorSetY(pPlayerPos, fPlayerPosY);

			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_AIR, pPos, pPlayerPos);
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}

		}
	}
}

void CBoss_Bat::Attack_Combo_H::State_Exit(CBoss_Bat* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_I (뒤로 점프뛰면서 플레이어 가드 시키는 패턴(Index = 24))

void CBoss_Bat::Attack_Combo_I::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 24;
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_Attack_Power = 0;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURXXLF; //임시
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Attack_Combo_I::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
#pragma region Effect_Start
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_AttackComboI.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BACKSLIP, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHead);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 24 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Bat::Attack_Combo_I::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Attack_Special (파티클 지리는 스페셜 스킬 패턴)

void CBoss_Bat::Attack_Special::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 23;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	pObject->m_bSpecial_Skill_Progress = true;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Bat::Attack_Special::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{

#pragma region EFFECT_SPECIAL
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Special_Attack1"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Special_Attack01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else if (!strcmp(iter.szName, "Sound_Special_Attack2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Special_Attack02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else if (!strcmp(iter.szName, "Sound_Special_Attack3"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Special_Attack03.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matWeapon = pObject->m_pModelCom->Get_BoneMatrix("head");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_BAT_RED_DUST_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         

					const _float4x4* matWeaponSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BAT_SPECIAL_ATTACK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeaponSpine);
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 23 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 275.f && !m_bChange_Attack_Power)
		{
			m_bChange_Attack_Power = true;
			pObject->m_bCristal_Create = false;
			pObject->m_iMonster_Attack_Power = 918;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}
		}
	}

}

void CBoss_Bat::Attack_Special::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_bSpecial_Skill_Progress = false;
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
}

#pragma endregion

#pragma region Recovery_State (피 회복하는 상태)

void CBoss_Bat::Recovery_State::State_Enter(CBoss_Bat* pObject)
{
	m_iIndex = 10;
	if (pObject->m_fMonsterCurHP >= 100.f || pObject->m_fShieldHP >= 100.f)
	{
		m_bFinish_Recovery = true;
	}
	else
		m_fRecoveryMax = pObject->m_fMonsterCurHP + m_fRecovery_Amount;

	pObject->m_bCristal_Create = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Bat::Recovery_State::State_Update(_float fTimeDelta, CBoss_Bat* pObject)
{
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 300.f && !m_bFinish_Recovery)
		{
			pObject->m_fMonsterCurHP += m_fCurrent_Recovery_Speed * fTimeDelta;
			pObject->m_fShieldHP += m_fCurrent_Recovery_Speed * fTimeDelta;

			if (pObject->m_fMonsterCurHP >= m_fRecoveryMax)
			{
				if (pObject->m_fMonsterCurHP >= pObject->m_fMonsterMaxHP || pObject->m_fShieldHP >= pObject->m_fMonsterMaxHP)
				{
					pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP;
					pObject->m_fShieldHP = pObject->m_fMonsterMaxHP;
				}
				m_bFinish_Recovery = true;
			}
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Bat::Move_State(), pObject);
				break;
			}
		}

	}

#pragma region EFFECT_BLOOD_SUCK
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Charge"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_Charge.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

				else if (!strncmp(iter.szName, "Sound_Wet", strlen("Sound_Wet")))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Bat_WetSound2.wav"), CHANNELID::SOUND_BOSS_ACTION, 2.f);
					iter.isPlay = true;
				}

			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Blood_Suck_1"))
				{
					const _float4x4* matMouth = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					_float4x4 matMouthWorld = {};
					XMStoreFloat4x4(&matMouthWorld, XMLoadFloat4x4(matMouth) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, matMouthWorld);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_Suck_2"))
				{
					const _float4x4* matMouth = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					_float4x4 matMouthWorld = {};
					XMStoreFloat4x4(&matMouthWorld, XMLoadFloat4x4(matMouth) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, matMouthWorld);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_Suck_3"))
				{
					const _float4x4* matMouth = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					_float4x4 matMouthWorld = {};
					XMStoreFloat4x4(&matMouthWorld, XMLoadFloat4x4(matMouth) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, matMouthWorld);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_Suck_4"))
				{
					const _float4x4* matMouth = pObject->m_pModelCom->Get_BoneMatrix("mouth");
					_float4x4 matMouthWorld = {};
					XMStoreFloat4x4(&matMouthWorld, XMLoadFloat4x4(matMouth) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					//pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matMouth);
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_BAT_BLOOD_SUCK, matMouthWorld);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Blood_Start"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_SURROND, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion
}

void CBoss_Bat::Recovery_State::State_Exit(CBoss_Bat* pObject)
{
	pObject->m_fRecovery_Skill_CoolTime = 0.f;
}

#pragma endregion
