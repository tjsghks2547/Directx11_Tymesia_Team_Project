#include "pch.h"
#include "Boss_Magician.h"
#include "Body_Magician.h"
#include "Weapon_Magician_Sword.h"
#include "Weapon_Cane.h"
#include "Projectile_Card.h"
#include "Projectile_Intro_Card.h"
#include "UI_Boss_HP_Bar.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Locked_On.h"
#include "Player.h"
#include "Boss_Magician_Camera.h"	

CBoss_Magician::CBoss_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Magician::CBoss_Magician(const CBoss_Magician& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CBoss_Magician::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Magician::Initialize(void* pArg)
{
	m_Is_Boss = true;
	m_fRotateSpeed = 180.f;
	m_fRootDistance = 1.f;
	m_fActive_Distance = 15.f;
	m_fCard_Delete_Time = 2.f;
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

	m_pState_Manager = CState_Machine<CBoss_Magician>::Create();
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

	m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_CANESWORD;


	LIGHT_DESC LightDesc{};
	ZeroMemory(&LightDesc, sizeof(LightDesc));

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
	LightDesc.fRange = 3.f;
	LightDesc.iCurrentLevel = LEVEL_SEAOFTREES;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Magician::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Magician::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
		m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Magician::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Magician::Render()
{
	return S_OK;
}

HRESULT CBoss_Magician::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Magician::Load_SporeObject()
{
	_ulong dwByte = {};

	string strDataPath = "../Bin/DataFiles/SpawnPoint/Sea_of_Trees/SpawnPoint";

	strDataPath = strDataPath + to_string(9) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iSize; i++)
	{
		CGameObject::GAMEOBJECT_DESC pDesc = {};
		ReadFile(hFile, &pDesc._fPosition, sizeof(_float4), &dwByte, nullptr);
		pDesc.iCurLevel = m_pGameInstance->Get_Current_Level_Index();

		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Building_Circus_Balloon"), m_pGameInstance->Get_Current_Level_Index(), TEXT("Layer_Monster_Building"), &pDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CBoss_Magician::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Magician::BODY_MAGICIAN_DESC BodyDesc{};
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.IsDissolveOn = &m_IsDissolveOn;
	BodyDesc.IsDissolveOff = &m_IsDissolveOff;
	BodyDesc.bRender = &m_bIntro_Body_Render;
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Magician"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Body"), &BodyDesc)))
		return E_FAIL;


	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Magician"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CWeapon_Magician_Sword::MAGICIAN_SWORD_DESC		Weapon_Desc{};
	Weapon_Desc.pParent = this;
	Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
	Weapon_Desc.pParentState = &m_iMonster_State;
	Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Desc.pParentModel = m_pModelCom;
	Weapon_Desc.IsDissolveOn = &m_IsDissolveOn;
	Weapon_Desc.IsDissolveOff = &m_IsDissolveOff;
	Weapon_Desc.bRender = &m_bIntro_Body_Render;
	Weapon_Desc.Is_Change_Sword_Bone = &Is_Change_Sword_Bone;
	Weapon_Desc.bCane_Collider_On = &m_bCane_Collider_On;
	Weapon_Desc.bSpecial_Skill_Progress = &m_bSpecial_Skill_Progress;
	Weapon_Desc.bCatch_Special_Attack = &m_bCatch_Special_Attack;
	Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Sword"), &Weapon_Desc)))
		return E_FAIL;

	CWeapon_Cane::MAGICIAN_CANE_DESC		Weapon2_Desc{};
	Weapon2_Desc.pParent = this;
	Weapon2_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_Cane");
	Weapon2_Desc.pParentState = &m_iMonster_State;
	Weapon2_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon2_Desc.pParentModel = m_pModelCom;
	Weapon2_Desc.IsDissolveOn = &m_IsDissolveOn;
	Weapon2_Desc.IsDissolveOff = &m_IsDissolveOff;
	Weapon2_Desc.bRender = &m_bIntro_Body_Render;
	Weapon2_Desc.bCane_Collider_On = &m_bCane_Collider_On;
	Weapon2_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon2_Desc.fSpeedPerSec = 0.f;
	Weapon2_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Cane"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Cane"), &Weapon2_Desc)))
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
	pBoss_HP_Bar.sBossName = TEXT("오두르");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;

	CBoss_Magician_Camera::CAMERA_DESC Magician_CameraDesc = {};

	Magician_CameraDesc.pParent = this;
	Magician_CameraDesc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("camera");
	Magician_CameraDesc.pParentState = &m_iMonster_State;
	Magician_CameraDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Magician_CameraDesc.pParentModel = m_pModelCom;
	Magician_CameraDesc.fSpeedPerSec = 0.f;
	Magician_CameraDesc.fRotationPerSec = 0.f;
	Magician_CameraDesc.iCurLevel = pDesc->iCurLevel;
	Magician_CameraDesc.pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Camera"), &Magician_CameraDesc)))
		return E_FAIL;

	CProjectile_Card::PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pParent = this;
	ProjectileDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	ProjectileDesc.iCurLevel = iLevel;
	ProjectileDesc.fDelete_Time = &m_fCard_Delete_Time;
	ProjectileDesc.iDamage = &m_iMonster_Attack_Power;
	ProjectileDesc.fSpeedPerSec = 30.f;
	ProjectileDesc.fRotationPerSec = 0.f;
	ProjectileDesc.fPosition = m_vSpawnPoint;

	for (_uint i = 0; i < 20; i++)
	{
		if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Card"), PROJECTILE_CARD, &ProjectileDesc)))
			return E_FAIL;
	}

	CProjectile_Intro_Card::PROJECTILE_INTRO_CARD_DESC Projectile_Intro_Desc = {};
	Projectile_Intro_Desc.pParent = this;
	Projectile_Intro_Desc.bActive = &m_bActive;
	Projectile_Intro_Desc.bRender = &m_bIntro_Card_Render;
	Projectile_Intro_Desc.bChangeModel = &m_bIntro_Card_Change;
	Projectile_Intro_Desc.pParentModel = m_pModelCom;
	Projectile_Intro_Desc.pParentState = &m_iMonster_State;
	Projectile_Intro_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Projectile_Intro_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("AnimTargetPoint");

	if (FAILED(__super::Add_PartObject(TEXT("Part_Projectile_Intro"), LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Intro_Card"), &Projectile_Intro_Desc)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Magician::State_Update(_float fTimeDelta)
{
	m_pState_Manager->State_Update(fTimeDelta, this);
}

void CBoss_Magician::PatternCreate()
{
	if (m_iPhase == PHASE_TWO && m_bActive)
	{
		m_fSpecial_Skill_CoolTime += m_fTimeDelta;
		m_fSlash_Skill_CoolTime += m_fTimeDelta;
	}

	if (!m_bPatternProgress && m_bActive)
	{
		m_fDelayTime += 1 * m_fTimeDelta;
		if (m_fDelayTime >= m_fCoolTime)
		{
			m_bCan_Hit_Motion = false;

			if (m_fSpecial_Skill_CoolTime >= 30.f)
				m_pState_Manager->ChangeState(new CBoss_Magician::Attack_Special(), this);
			else if (m_fSlash_Skill_CoolTime >= 40.f)
				m_pState_Manager->ChangeState(new CBoss_Magician::Attack_Slash(), this);
			else if (m_fDistance >= 5.f)
				Far_Pattern_Create();
			else
				Near_Pattern_Create();

			m_fDelayTime = 0.f;
			m_bPatternProgress = true;
		}
	}

}

void CBoss_Magician::Active()
{
	m_iPhase = PHASE_ONE;
	m_pState_Manager->ChangeState(new CBoss_Magician::Intro_State(), this);
}

void CBoss_Magician::Stun()
{
	m_pState_Manager->ChangeState(new CBoss_Magician::Stun_State(), this);
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;

#pragma region Effect
	m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
	m_pGameInstance->Set_SlowWorld(true);
	m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
	m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_SWORD);
	m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_CANE);
	m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SWORD_DUST);
#pragma endregion
}

_vector CBoss_Magician::Bezier_Move(_vector _vStartPos, _vector vCurvePos, _vector vEndPos, _float fTime)
{
	_vector vResult = (1 - fTime) * (1 - fTime) * _vStartPos +
		2 * (1 - fTime) * fTime * vCurvePos +
		fTime * fTime * vEndPos;

	vResult = XMVectorSetY(vResult, XMVectorGetY(_vStartPos));

	return vResult;
}


void CBoss_Magician::Near_Pattern_Create()
{
	_uint iRandomPattern = {};

	if (m_iPhase == PHASE_ONE)
		iRandomPattern = rand() % 4;
	else
		iRandomPattern = rand() % 5;

	while (true)
	{
		if ((iRandomPattern == m_iNearPatternIndex) || (!m_bCanDissapear && iRandomPattern == 3))
		{
			if (m_iPhase == PHASE_ONE)
				iRandomPattern = rand() % 4;
			else
				iRandomPattern = rand() % 5;
		}
		else
		{
			m_iNearPatternIndex = iRandomPattern;
			m_bCanDissapear = true;
			break;
		}
	}
	switch (m_iNearPatternIndex)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboA(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboB(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboC(), this);
		break;
	case 3:
		m_bCanDissapear = false;
		m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1, true), this);
		break;
	case 4:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboD(), this);
		break;
	}
}

void CBoss_Magician::Far_Pattern_Create()
{
	_uint iRandomPattern = {};
	if (m_iPhase == PHASE_ONE)
		iRandomPattern = rand() % 3;
	else
		iRandomPattern = rand() % 4;

	while (true)
	{
		if ((iRandomPattern == m_iFarPatternIndex) || (!m_bCanDissapear && iRandomPattern == 0))
		{
			if (m_iPhase == PHASE_ONE)
				iRandomPattern = rand() % 3;
			else
				iRandomPattern = rand() % 4;
		}
		else
		{
			m_iFarPatternIndex = iRandomPattern;
			m_bCanDissapear = true;
			break;
		}
	}

	switch (m_iFarPatternIndex)
	{
	case 0:
		m_bCanDissapear = false;
		m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(0), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician::Shoot_ComboA(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician::Shoot_ComboB(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboD(), this);
		break;
	}
}



void CBoss_Magician::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
	{
		m_fDelayTime -= m_fTimeDelta;
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
				uniform_int_distribution<> Random_Parry_Attack(0, 2);
				_uint iRandom_Parry_Attack = Random_Pattern(gen);
				switch (iRandom_Parry_Attack)
				{
				case 0:
					m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_A(), this);
					break;
				case 1:
					m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_B(), this);
					break;
				case 2:
					m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_C(), this);
					break;
				}
			}
			else
			{
				Near_Pattern_Create();
			}
			m_iHitCount = 0;
			m_bCan_Hit_Motion = false;
			m_bPatternProgress = true;
			m_fDelayTime = 0.f;
			return;
		}

		if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= *m_Player_Attack / 70.f;
			m_fShieldHP -= (*m_Player_Attack / 15.f);
		}
		else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
		{
			m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 5.f;
			m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 70.f;
			if (m_fMonsterCurHP <= m_fShieldHP)
			{
				m_fMonsterCurHP = m_fShieldHP;
			}
		}

		if (m_bCan_Hit_Motion &&
			m_iMonster_State != STATE_SPECIAL_ATTACK &&
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
			m_pState_Manager->ChangeState(new CBoss_Magician::Hit_State(m_iHit_Motion_Index), this);
		}

		if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
		{
			_uint iRandSoundFileNum = {};
			iRandSoundFileNum = rand() % 4;

			switch (iRandSoundFileNum)
			{
			case 0:
				m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
				break;
			case 1:
				m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
				break;
			case 2:
				m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
				break;
			case 3:
				m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
				break;
			}
		}
	}
}

void CBoss_Magician::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Magician::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Magician* CBoss_Magician::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Magician* pInstance = new CBoss_Magician(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Magician::Clone(void* pArg)
{
	CBoss_Magician* pInstance = new CBoss_Magician(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Magician::Free()
{
	m_pGameInstance->Delete_Dynamic_Light(m_pTransformCom);

	__super::Free();

	Safe_Release(m_pState_Manager);
}

#pragma region Intro_State
void CBoss_Magician::Intro_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 75;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bIntro_Card_Render = true;
	pObject->m_bCardActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pGameInstance->Set_Boss_Active(true);
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_PlayerScreen"), false);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);

	pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
}

void CBoss_Magician::Intro_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Intro"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Intro.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

			}
		}
	}

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 180.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 210.f)
		{
			if (!pObject->m_bIntro_Card_Change)
			{
				pObject->m_pGameInstance->Play_Sound(TEXT("Magician_TrapCard.wav"), CHANNELID::SOUND_EFFECT, 0.5f);
			}

			pObject->m_bIntro_Card_Change = true;
		}
		else
			pObject->m_bIntro_Card_Change = false;


		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 300.f && !pObject->m_bIntro_Body_Render)
		{
			pObject->m_bIntro_Body_Render = true;
		}

		if (pObject->m_pModelCom->GetAniFinish())
			pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
	}
}

void CBoss_Magician::Intro_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCardActive = false;
	pObject->m_bHP_Bar_Active = true;
	pObject->Delete_PartObject(TEXT("Part_Projectile_Intro"));
	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_PlayerScreen"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	pObject->m_pGameInstance->UIGroup_Render_OnOff(LEVEL_SEAOFTREES, TEXT("Layer_Dialogue"), true);
	pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
	pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 1, 11);

	pObject->m_pGameInstance->PlayBGM(TEXT("charmer_music_A.ogg"), 0.3f);
}
#pragma endregion

#pragma region Idle_State
void CBoss_Magician::Idle_State::State_Enter(CBoss_Magician* pObject)
{
	// 23도 Idle2임
	m_iIndex = 22;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Idle_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
	}

}

void CBoss_Magician::Idle_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Move_State
void CBoss_Magician::Move_State::State_Enter(CBoss_Magician* pObject)
{
	//나중에 플레이어 위치받아올듯
	if (pObject->m_fDistance > 1.5f)
		m_iIndex = 69;
	else if (pObject->m_fDistance <= 1.5f)
	{
		iRandomMove = rand() % 3;
		switch (iRandomMove)
		{
		case 0:
			m_iIndex = 67;
			break;
		case 1:
			m_iIndex = 71;
			break;
		case 2:
			m_iIndex = 73;
			break;
		}
	}
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CBoss_Magician::Move_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Walk"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Walk.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}
		}
	}

	pObject->RotateDegree_To_Player();
	if (m_iIndex == 69 && pObject->m_fDistance > pObject->m_fRootDistance)
	{
		pObject->m_pTransformCom->Go_Straight(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else if (m_iIndex == 67)
	{
		pObject->m_pTransformCom->Go_Backward_With_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else if (m_iIndex == 73)
	{
		pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
		pObject->m_pTransformCom->Go_Right_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else
	{
		pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
		pObject->m_pTransformCom->Go_Left_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
}

void CBoss_Magician::Move_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Stun_State
void CBoss_Magician::Stun_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 56;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_MAGICIAN;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iHitCount = 0;
	pObject->RotateDegree_To_Player();

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

	pObject->m_IsDissolveOff = false;
	pObject->m_IsDissolveOn = false;
	pObject->Is_Change_Sword_Bone = false;

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Magician::Stun_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{

#pragma region Effect_Stun
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
		
			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Stun"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_STUN, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion
	if (m_iIndex == 56 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 54;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}

	if ((m_iIndex == 54 || m_iIndex == 56) &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
		(*pObject->m_Player_State) == CPlayer::STATE_MAGICIAN_Execution)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::ExeCution_State(), pObject);
	}

}

void CBoss_Magician::Stun_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Shoot_ComboA
void CBoss_Magician::Shoot_ComboA::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 34;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 50;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Shoot_ComboA::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Shoot
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Shoot"))
				{
					_uint iRandom = rand() % 2;
					switch (iRandom)
					{
					case 0:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_CardShotA.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					case 1:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_CardShotB.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					}
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Shoot2"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					for (_uint i = 0; i < 2; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SHOOT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);

					}
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Shoot3"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					for (_uint i = 0; i < 2; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SHOOT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);
					}
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion


	pObject->RotateDegree_To_Player();
	if (m_iIndex == 34 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f)
		{
			m_iIndex = 35;
			m_bIs_Fired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f &&
			!m_bIs_Fired)
		{
			m_bIs_Fired = true;
			_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
			_float fPosY = XMVectorGetY(vPos);
			_float fPlayerPosY = XMVectorGetY(vPlayerPos);
			fPosY += 0.5f;
			fPlayerPosY += 0.5f;
			vPos = XMVectorSetY(vPos, fPosY);
			vPlayerPos = XMVectorSetY(vPlayerPos, fPlayerPosY);
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, vPlayerPos);
		}
	}

	if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f &&
			!m_bIs_Fired)
		{
			m_bIs_Fired = true;
			_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
			_float fPosY = XMVectorGetY(vPos);
			_float fPlayerPosY = XMVectorGetY(vPlayerPos);
			fPosY += 0.5f;
			fPlayerPosY += 0.5f;
			vPos = XMVectorSetY(vPos, fPosY);
			vPlayerPos = XMVectorSetY(vPlayerPos, fPlayerPosY);
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, vPlayerPos);
		}
	}
}
void CBoss_Magician::Shoot_ComboA::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion

#pragma region Shoot_ComboB

void CBoss_Magician::Shoot_ComboB::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 33;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 40;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Shoot_ComboB::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Shoot
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Shoot"))
				{
					_uint iRandom = rand() % 2;
					switch (iRandom)
					{
					case 0:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_CardShotA.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					case 1:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_CardShotB.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					}
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Shoot1"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					for (_uint i = 0; i < 2; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SHOOT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);
					}
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Shoot2"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					for (_uint i = 0; i < 2; i++)
					{

						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SHOOT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);
					}
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Shoot3"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					for (_uint i = 0; i < 2; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SHOOT, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);
					}
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
	_float fPosY = XMVectorGetY(vPos);
	_float fPlayerPosY = XMVectorGetY(vPlayerPos);
	fPosY += 0.5f;
	fPlayerPosY += 0.5f;
	vPos = XMVectorSetY(vPos, fPosY);
	vPlayerPos = XMVectorSetY(vPlayerPos, fPlayerPosY);


	pObject->RotateDegree_To_Player();

	if (m_iIndex == 33 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 130.f)
		{
			m_iIndex = 32;
			m_bIs_Fired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
			pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
		}
		else if (!m_bIs_Fired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 109.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 110.f)
		{
			m_bIs_Fired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, false);
		}
	}
	if (m_iIndex == 32 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f)
		{
			m_iIndex = 31;
			m_bIs_Fired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
			pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
		}
		else if (!m_bIs_Fired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 41.f)
		{
			m_bIs_Fired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, true);
		}
	}

	if (m_iIndex == 31 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			m_iIndex = 30;
			m_bIs_Fired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
		else if (!m_bIs_Fired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 19.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 20.f)
		{
			m_bIs_Fired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, false);
		}
	}
	if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
		else if (!m_bIs_Fired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 41.f)
		{
			//아마 필살기 날릴 부분.
			m_bIs_Fired = true;
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, vPlayerPos);
		}
	}
}

void CBoss_Magician::Shoot_ComboB::State_Exit(CBoss_Magician* pObject)
{
}

#pragma endregion

#pragma region Attack_ComboA(한번 베고 지팡이 돌린뒤 쿡 찌르기)
void CBoss_Magician::Attack_ComboA::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 6;
	pObject->m_bCane_Collider_On = true;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_iMonster_Attack_Power = 48;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSF;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboA::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Swing + FirstSwing + CaneStab
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Attack_ComboA.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_FirstSwing"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_FIRSTSWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Swing"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_CaneStab"))
				{
					const _float4x4* matCane = pObject->m_pModelCom->Get_BoneMatrix("weapon_Cane");
					for (_uint i = 0; i < 2; i++)
					{
						pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_CANESTAB, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matCane);
					}

					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		m_iIndex = 5;
		pObject->m_iMonster_State = STATE_ATTACK;
		pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSL;
		pObject->RotateDegree_To_Player();
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		pObject->m_pModelCom->Get_VecAnimation().at(5)->Set_StartOffSetTrackPosition(25.f);
	}

	if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 100.f)
		{
			pObject->m_iMonster_Attack_Power = 95;
			pObject->m_iPlayer_Hitted_State = PLAYER_HURT_KnockBackF;
		}
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 250.f)
		{
			pObject->m_bCan_Hit_Motion = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}

	}
}

void CBoss_Magician::Attack_ComboA::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCane_Collider_On = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion

#pragma region Attack_ComboB(발차기 두번하고 베는 애니메이션(끝나면 뒤로 슉 하고 사라짐))
void CBoss_Magician::Attack_ComboB::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 25;
	pObject->m_iMonster_State = STATE_KICK_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 48;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSF;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboB::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Kick + Turning
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Attack_ComboB.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Kick"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_KICK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Turning"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_TURNING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 25 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1), pObject);

		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 100.f)
		{
			pObject->m_iMonster_State = STATE_ATTACK;
			pObject->RotateDegree_To_Player();
			pObject->Is_Change_Sword_Bone = true;
			pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
		}
	}
}

void CBoss_Magician::Attack_ComboB::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Hit_Motion = true;
	pObject->Is_Change_Sword_Bone = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion

#pragma region Attack_ComboC(준비자세후 한번베고 발차기)
void CBoss_Magician::Attack_ComboC::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 4;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 48;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	pObject->RotateDegree_To_Player();
	pObject->m_bCane_Collider_On = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboC::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_FirstSwing + Kick
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Attack_ComboC.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_FirstSwing"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_FIRSTSWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Kick"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_KICK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 150.f)
		{
			pObject->m_bCane_Collider_On = false;
			pObject->RotateDegree_To_Player();
			pObject->m_iMonster_State = STATE_KICK_ATTACK;
			pObject->m_iPlayer_Hitted_State = PLAYER_HURT_KnockBackF;
		}
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 220.f)
		{
			pObject->m_bCan_Hit_Motion = true;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
	}
}

void CBoss_Magician::Attack_ComboC::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion

#pragma region Attack_ComboD((2페이지 패턴),달려가면서 플레이어쪽으로 여러 베기동작 실행)

void CBoss_Magician::Attack_ComboD::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 60;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 48;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(0.f);

}

void CBoss_Magician::Attack_ComboD::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_FirstSwing + Turning + Kick + Swing
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Attack"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Attack_ComboD.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_FirstSwing"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_FIRSTSWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Turning"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_TURNING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Kick"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_KICK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Swing"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SWING_DUST, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	//아마 이쪽에서 베지어 무빙 해야할듯?
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		m_fLinearTime += fTimeDelta;
		_vector vNextPos = pObject->Bezier_Move(XMLoadFloat4(&m_vStartPos), XMLoadFloat4(&m_vCurvePos), XMLoadFloat4(&m_vEndPos), m_fLinearTime);
		_vector vLook = XMVector3Normalize(vNextPos - XMLoadFloat4(&m_vStartPos));
		pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNextPos);
		pObject->m_pTransformCom->Look(vLook);
	}
	//베지어 아닐땐 플레이어 바라보게하기
	else
		pObject->RotateDegree_To_Player();

	if (!m_bFirst && m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 83.f)
			pObject->m_IsDissolveOn = true;
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 93.f)
		{
			m_bFirst = true;
			m_iIndex = 10;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
			//베지어 이동시키기
			XMStoreFloat4(&m_vStartPos, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			XMStoreFloat4(&m_vEndPos, XMLoadFloat4(&pObject->m_vPlayerPos));
			_vector vDir = XMVector3Normalize(XMLoadFloat4(&m_vEndPos) - XMLoadFloat4(&m_vStartPos));
			_vector vRight = XMVector3Cross(vDir, XMVectorSet(0.f, 1.f, 0.f, 0.f));
			vRight *= 4.f;
			if (rand() % 2 == 0)
				vRight *= -1;

			_vector vMidPoint = (XMLoadFloat4(&m_vStartPos) + XMLoadFloat4(&m_vEndPos)) * 0.5f;
			XMStoreFloat4(&m_vCurvePos, vMidPoint + vRight);
			XMStoreFloat4(&m_vCurvePos, XMVectorSetY(XMLoadFloat4(&m_vCurvePos), XMVectorGetY(XMLoadFloat4(&m_vStartPos))));
		}
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 220.f)
		{
			pObject->m_iMonster_Attack_Power = 29;
		}
	}
	//거리가 1.f이하로 가까워지거나 Linear타임 다 끝나면 다시 등장해서 공격하기 진행
	if ((m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 1.f) || m_fLinearTime >= 1.f)
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		m_iIndex = 60;
		m_fLinearTime = 0.f;
		pObject->m_pModelCom->Set_LerpFinished(true);
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(93.f);
	}

	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 430.f)
			pObject->Is_Change_Sword_Bone = true;
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 430.f)
		{
			pObject->m_bCan_Hit_Motion = true;
			pObject->Is_Change_Sword_Bone = false;
		}
	}

	//특정 프레임에 케인 콜라이더로 변경 했다가 다시 검 콜라이더로 돌아가기
	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 121.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 156.f)
		pObject->m_bCane_Collider_On = true;
	else
		pObject->m_bCane_Collider_On = false;

	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}

}

void CBoss_Magician::Attack_ComboD::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_IsDissolveOn = false;
	pObject->m_IsDissolveOff = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion

#pragma region Execution_State
void CBoss_Magician::ExeCution_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 63;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->m_bCan_Move_Anim = true;

	_float teleportDistance = 1.f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

	pObject->m_pTransformCom->LookAt(vPlayerPos);
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->m_pTransformCom->LookAt(vPlayerPos);


	/* 선환 추가 */
	pObject->m_pModelCom->Get_VecAnimation().at(63)->SetLerpTime(0.f);	
	pObject->m_pModelCom->Set_LerpFinished(true);
	/* ============ */
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Magician::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{

#pragma region EFFECT_EXECUTION
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Execution"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Execution.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Blood_1"))
				{
					const _float4x4* Spine_01 = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_EXECUTION_BLOOD_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), Spine_01);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
				else if (!strcmp(iter.szName, "Effect_Blood_2"))
				{
					const _float4x4* Spine_01 = pObject->m_pModelCom->Get_BoneMatrix("spine_03");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_EXECUTION_BLOOD_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), Spine_01);
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_EXECUTION_DUST_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), Spine_01);
					iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 63 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_iPhase == PHASE_ONE && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 150.f && !pObject->m_IsDissolveOn)
		{
			pObject->m_IsDissolveOn = true;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			if (pObject->m_iPhase == PHASE_ONE)
			{
				pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
				pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
				pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);
				pObject->m_pState_Manager->ChangeState(new Phase_Change_State(), pObject);
			}
			else
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Jump_State, pObject);
		}
	}
}

void CBoss_Magician::ExeCution_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bExecution_Progress = false;
	if (pObject->m_iPhase == PHASE_ONE)
	{
		pObject->m_iPhase = PHASE_TWO;
		pObject->m_IsStun = false;
		pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP;
		pObject->m_bCanRecovery = true;

		pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
		pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 14);
	}
}

#pragma endregion

#pragma region Hit_State

CBoss_Magician::Hit_State::Hit_State(_uint iHit_Index)
{
	m_iHit_Index = iHit_Index;
}

void CBoss_Magician::Hit_State::State_Enter(CBoss_Magician* pObject)
{
	switch (m_iHit_Index)
	{
	case 0:
		m_iIndex = 18;
		break;
	case 1:
		m_iIndex = 19;
		break;
	}
	pObject->m_iHitCount++;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();
	pObject->Is_Change_Sword_Bone = false;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Hit_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Hurt"))
				{
					pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_CANE);
					pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_SWORD);
					pObject->m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SWORD_DUST);

					_uint iRandom = rand() % 3;
					switch (iRandom)
					{
					case 0:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Hurt_01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					case 1:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Hurt_02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					case 2:
						pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Hurt_03.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
						break;
					}
					iter.isPlay = true;
				}
			}
		}
	}

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CBoss_Magician::Hit_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bPatternProgress = false;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Dissappear_Move_State

CBoss_Magician::Dissappear_Move_State::Dissappear_Move_State(_uint iDissappear_Index, _bool bShootCard)
{
	m_iDissappear_Index = iDissappear_Index;
	m_bShootCard = bShootCard;
}

void CBoss_Magician::Dissappear_Move_State::State_Enter(CBoss_Magician* pObject)
{
	switch (m_iDissappear_Index)
	{
	case 0:
		//앞으로 이동
		m_iIndex = 70;
		pObject->m_bCan_Move_Anim = false;
		break;
	case 1:
		//뒤로 이동
		m_iIndex = 68;
		pObject->m_bCan_Move_Anim = true;
		break;
	case 2:
		//왼쪽 이동
		m_iIndex = 72;
		pObject->m_bCan_Move_Anim = true;
		break;
	case 3:
		//오른쪽으로 이동
		m_iIndex = 74;
		pObject->m_bCan_Move_Anim = true;
		break;
	default:
		break;
	}
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_IsDissolveOn = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

#pragma region Effect_Disappear_Start

	const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
	pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_DISAPPEAR_START, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);

#pragma endregion
}

void CBoss_Magician::Dissappear_Move_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Disappear_Dust
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Walk_Dissappear"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Dissappear.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}


			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_MovingDust"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_DISAPPEAR_MOVING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion


	//앞
	if (m_iIndex == 70 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 10;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		pObject->RotateDegree_To_Player();
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 1.5f)
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->Near_Pattern_Create();
	}
	//뒤
	if (m_iIndex == 68 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 8;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 8 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		//if (!m_bShootCard)
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}

	//왼
	if (m_iIndex == 72 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 11;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}

	if (m_iIndex == 74 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 13;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}


}

void CBoss_Magician::Dissappear_Move_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bCan_Hit_Motion = true;
}
#pragma endregion

#pragma region Dissappear_Idle_State

void CBoss_Magician::Dissappear_Idle_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 15;
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Dissappear_Idle_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
	}
}

void CBoss_Magician::Dissappear_Idle_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_pModelCom->Set_LerpFinished(true);
	pObject->m_IsDissolveOff = false;
}
#pragma endregion

#pragma region Attack_Special
void CBoss_Magician::Attack_Special::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 40;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	pObject->m_bSpecial_Skill_Progress = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_MAGICIAN_CATCH;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_Special::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Charge + Fail
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Special_Attack_Start"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Special_Attack_Start.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Special_Catch"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Special_Attack_Fail.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPATTACK_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Start"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPATTACK_FAIL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Fail"))
				{
					const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPATTACK_FAIL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matRootNode);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion


	if (m_iIndex == 39 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 15.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 200.f)
			pObject->Is_Change_Sword_Bone = true;
		else
		{
			pObject->RotateDegree_To_Player();
			pObject->Is_Change_Sword_Bone = false;
		}

		//스페셜스킬 잡혔을때
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 30.f && pObject->m_bCatch_Special_Attack)
		{
			m_bCatch_Success = true;
			pObject->m_pState_Manager->ChangeState(new Attack_Special_Catch(), pObject);
			return;
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 35.f)
		{
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
			pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 190.f)
		{
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
			pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
		}

		// 특별 컷씬 실패 시, 애니메이션이 끝났으면 Idle 또는 Move 상태로 전환
		if (pObject->m_pModelCom->Get_Current_Animation_Index() == 39 &&
			pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			if (iRandom == 0)
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			else
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
		}
	}
	else if (m_iIndex == 40 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 39;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
}

void CBoss_Magician::Attack_Special::State_Exit(CBoss_Magician* pObject)
{
	if (!m_bCatch_Success)
	{
		pObject->m_bCan_Hit_Motion = true;
		pObject->m_bCatch_Special_Attack = false;
		pObject->m_bSpecial_Skill_Progress = false;
		pObject->m_fSpecial_Skill_CoolTime = 0.f;
	}
}
#pragma endregion

#pragma region Attack_Special_Catch
void CBoss_Magician::Attack_Special_Catch::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 41;
	pObject->m_iMonster_Attack_Power = 190;
	pObject->m_bCan_Move_Anim = true;  // 이런거 한번 만 (선환 범승 한번 체크 부탁함 )	

	_float teleportDistance = 1.f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);
	vPlayerRight = XMVector3Normalize(vPlayerRight);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));
	vNewPos = XMVectorAdd(vNewPos, XMVectorScale(vPlayerRight, 0.1f));

	pObject->m_pTransformCom->LookAt(vPlayerPos);
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->m_pTransformCom->LookAt(vPlayerPos);

	pObject->Is_Change_Sword_Bone = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}


void CBoss_Magician::Attack_Special_Catch::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Loop + Final
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Special_Catch"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Special_Catch.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Loop"))
				{
					const _float4x4* matWeapon_l = pObject->m_pModelCom->Get_BoneMatrix("weapon_l_Sword");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPATTACK_SUCCESS, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_l);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Final"))
				{
					const _float4x4* matWeapon_l = pObject->m_pModelCom->Get_BoneMatrix("weapon_l_Sword");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPATTACK_LASTCHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_l);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion


	if (m_iIndex == 41 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 255.f && !m_bAttack)
		{
			m_bAttack = true;
			static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 290.f)
		{
			pObject->Is_Change_Sword_Bone = false;
		}
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 380.f)
		{
			pObject->m_IsDissolveOn = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1), pObject);
		}
	}
}

void CBoss_Magician::Attack_Special_Catch::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
	pObject->m_bSpecial_Skill_Progress = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bCatch_Special_Attack = false;
}
#pragma endregion

#pragma region Attack_Slash
void CBoss_Magician::Attack_Slash::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 57;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_Attack_Power = 48;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK2;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_Slash::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{

#pragma region Effect_Charge + Burst
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Slash1"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Slash01.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);

#pragma region Effect_Warning

					pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
					const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
					_float4x4 matHeadWorld = {};
					XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
					pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Sound_Slash2"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Slash02.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}

			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Charge"))
				{
					const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_BURST_CHARGE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Burst_1"))
				{
					for (_uint i = 0; i < 2; i++)
					{
						const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
						pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_BURST_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					}
					iter.isPlay = true;
				}
				else if (!strcmp(iter.szName, "Effect_Burst_2"))
				{
					for (_uint i = 0; i < 2; i++)
					{
						const _float4x4* matRootNode = pObject->m_pModelCom->Get_BoneMatrix("RootNode");
						pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_BURST_2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK));
					}
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 246.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 340.f)
		{
			pObject->Is_Change_Sword_Bone = true;
		}
		else
		{
			pObject->RotateDegree_To_Player();
			pObject->Is_Change_Sword_Bone = false;
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 320.f)
		{
			pObject->m_iMonster_Attack_Power = 95;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
	}
}

void CBoss_Magician::Attack_Slash::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_fSlash_Skill_CoolTime = 0.f;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion

#pragma region Parry_Attack_A (뒤로 살짝 빠졌다가 앞으로 칼로 두번 긁는 패링)

void CBoss_Magician::Parry_Attack_A::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 26;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


#pragma region Effect_Parry
	pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_PARRY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
#pragma endregion
}

void CBoss_Magician::Parry_Attack_A::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{

			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Parry"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Parry_AttackA.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

		}
	}

	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 140.f &&
			pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 161.f)
		{
			pObject->m_iMonster_Attack_Power = 66;
			pObject->m_iMonster_State = STATE_PARRY_ATTACK;
			pObject->Is_Change_Sword_Bone = true;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 165.f &&
			pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 181.f)
		{
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSL;
		}

		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 185.f && pObject->Is_Change_Sword_Bone)
		{
			pObject->Is_Change_Sword_Bone = false;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}

	}
}

void CBoss_Magician::Parry_Attack_A::State_Exit(CBoss_Magician* pObject)
{
	pObject->Is_Change_Sword_Bone = false;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion

#pragma region Parry_Attack_B (발로 차는 패링)

void CBoss_Magician::Parry_Attack_B::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 27;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


#pragma region Effect_Parry
	pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_PARRY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
#pragma endregion
}

void CBoss_Magician::Parry_Attack_B::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Parry"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Parry_AttackB.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}
		}
	}

	if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 140.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 171.f)
		{
			pObject->m_iMonster_Attack_Power = 48;
			pObject->m_iMonster_State = STATE_PARRY_ATTACK;
			pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
		}

		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
	}
}

void CBoss_Magician::Parry_Attack_B::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}
#pragma endregion

#pragma region Parry_Attack_C (뒤로 점프하는 패링)

void CBoss_Magician::Parry_Attack_C::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 29;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


#pragma region Effect_Parry
	pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_PARRY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
#pragma endregion


}

void CBoss_Magician::Parry_Attack_C::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Parry"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Parry_AttackC.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}
		}
	}

	if (m_iIndex == 29 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboA(), pObject);
	}
}

void CBoss_Magician::Parry_Attack_C::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
}

#pragma endregion

#pragma region Dissappear_Jump_State

void CBoss_Magician::Dissappear_Jump_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 52;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iMonster_State = STATE_DEAD;
	pObject->m_bHP_Bar_Active = false;

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

	pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
}

void CBoss_Magician::Dissappear_Jump_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 35.f)
		{
			pObject->m_IsDissolveOn = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			if (!m_Is_Spawn)
			{
				m_Is_Spawn = true;
				pObject->Load_SporeObject();

				FOGPARAMS FogDesc{};
				FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
				FogDesc.fFogStartDistance = _float2(0.1f, 8.f);
				FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
				FogDesc.g_FogColor = _float4(0.223f, 0.1725f, 0.1019f, 1.f);

				pObject->m_pGameInstance->Set_FogFactors(FogDesc);
				pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);



				pObject->m_pGameInstance->Set_All_UIObject_Condition_Open((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), true);
				pObject->m_pGameInstance->Set_Condition((pObject->m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk")), 2, 13);

				pObject->m_pGameInstance->UIScene_UIObject_Render_OnOff(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_Quest_Poison"), true);
				pObject->m_pGameInstance->Set_All_UIObject_Condition_Open(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_Quest_Poison"), true);
				//CGameObject::GAMEOBJECT_DESC pDesc = {};
				//pDesc.iCurLevel = LEVEL_SEAOFTREES;
				//pDesc.fPosition = pObject->m_vSpawnPoint;
				//if (FAILED(pObject->m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2"), CATEGORY_BOSS, &pDesc)))
				//	return;

				pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);

				pObject->m_pGameInstance->PlayBGM(TEXT("Puppet_Combat_-_Loop.ogg"), 0.15f);
			}
			pObject->m_bDead = true;
			pObject->m_bActive = false;
		}
	}
}

void CBoss_Magician::Dissappear_Jump_State::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion


void CBoss_Magician::Phase_Change_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 51;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


	pObject->m_pGameInstance->Delete_Light_Type(LIGHT_DESC::TYPE_DIRECTIONAL);
	pObject->m_pGameInstance->Delete_Light_Type(LIGHT_DESC::TYPE_SPOT);
	pObject->m_pGameInstance->Delete_Dynamic_Light(pObject->m_pTransformCom);

	FOGPARAMS FogDesc{};
	FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
	FogDesc.fFogStartDistance = _float2(0.005f, 8.f);
	FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
	FogDesc.g_FogColor = _float4(0., 0.f, 0.f, 1.f);

	pObject->m_pGameInstance->Set_FogFactors(FogDesc);


	//LIGHT_DESC LightDesc{};

	//ZeroMemory(&LightDesc, sizeof(LightDesc));

	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vPosition = _float4(-43.540661f, 95.240334f, -103.446053f, 1.f);
	//LightDesc.fRange = 200.f;
	//LightDesc.iCurrentLevel = LEVEL_SEAOFTREES;

	//pObject->m_pGameInstance->Add_Light_MINRange(LightDesc, 0.1f, 100.f);

	pObject->m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
}

void CBoss_Magician::Phase_Change_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
#pragma region Effect_Phase_Change
	for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Sound_Phase_Change"))
				{
					pObject->m_pGameInstance->Play_Sound(TEXT("Magician_Phase_Change.wav"), CHANNELID::SOUND_BOSS_ACTION, 0.5f);
					iter.isPlay = true;
				}
			}

			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				if (!strcmp(iter.szName, "Effect_Sprinkle"))
				{
					const _float4x4* matHand_L = pObject->m_pModelCom->Get_BoneMatrix("hand_l");
					pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_MAGICIAN_SPRINKLE, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matHand_L);
					iter.isPlay = true;
				}
			}
		}
	}
#pragma endregion

	if (m_iIndex == 51 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f)
		{
			pObject->m_IsDissolveOn = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			m_iIndex = 49;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
	}
	if (m_iIndex == 49 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 10.f)
		{
			pObject->m_IsDissolveOn = false;
			pObject->m_IsDissolveOff = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			if (pObject->m_IsDissolveOff == true)
			{
				LIGHT_DESC LightDesc{};
				ZeroMemory(&LightDesc, sizeof(LightDesc));

				LightDesc.eType = LIGHT_DESC::TYPE_POINT;
				LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
				LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
				LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
				LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
				LightDesc.fRange = 5.f;
				LightDesc.iCurrentLevel = LEVEL_SEAOFTREES;

				pObject->m_pGameInstance->Add_Light(LightDesc, pObject->m_pTransformCom);


				pObject->m_pGameInstance->PlayBGM(TEXT("charmer_music_B.ogg"), 0.3f);
			}


			pObject->m_IsDissolveOff = false;
			m_iIndex = 42;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
	}

	if (m_iIndex == 42 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1), pObject);

			LIGHT_DESC LightDesc{};

			ZeroMemory(&LightDesc, sizeof(LightDesc));

			LightDesc.eType = LIGHT_DESC::TYPE_POINT;
			LightDesc.vDiffuse = _float4(200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 1.f);
			LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
			LightDesc.vSpecular = _float4(0.6f, 0.6f, 0.6f, 1.f);
			LightDesc.vPosition = _float4(-43.540661f, 95.240334f, -103.446053f, 1.f);
			LightDesc.fRange = 5.f;
			LightDesc.iCurrentLevel = LEVEL_SEAOFTREES;

			pObject->m_pGameInstance->Add_Light_Range(LightDesc, 50.f, 30.f);

		}
	}
}



void CBoss_Magician::Phase_Change_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}