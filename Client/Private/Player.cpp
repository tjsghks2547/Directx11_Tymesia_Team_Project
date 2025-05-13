#include "pch.h" 
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "RightWeapon.h"
#include "LeftWeapon.h"
#include "StateMgr.h"
#include "Animation.h"
#include "ClawWeapon.h"
#include "PlayerCamera.h"
#include "Weapon_Halberd.h"	
#include "Weapon_Scythe.h"	
#include "Player_Weapon_Axe.h"	
#include "ChairLamp.h"	
#include "VargKnife.h"
#include "Player_Weapon_Cane.h"
#include "Player_Weapon_Cane_Sword.h"
#include "Player_Weapon_GreadSword.h"
#include "Player_Weapon_JavelinSword.h"
#include "PlayerSkillMgr.h"	

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:CContainerObject(Prototype)
	, m_iState(Prototype.m_iState)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	strcpy_s(m_szName, "PLAYER");

	m_pStateMgr = CStateMgr::Create();
	if (m_pStateMgr == nullptr)
	{
		MSG_BOX("Failed to Created : StateMgr");
	}

	m_pPlayerSkillMgr = CPlayerSkillMgr::Create();
	if (m_pPlayerSkillMgr == nullptr)
	{
		MSG_BOX("Failed to Created : PlayerSkillMgr");
	}

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.2f,0.2f,0.2f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
	_uint settingColliderGroup = GROUP_TYPE::MONSTER | GROUP_TYPE::MONSTER_WEAPON | GROUP_TYPE::ITEM | GROUP_TYPE::OBJECT | GROUP_TYPE::DESTRUCT;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER, settingColliderGroup);
	m_pGameInstance->Add_Actor_Scene(m_pActor);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;


	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	_vector vTestPosition = XMVectorSetW(XMLoadFloat4(&pDesc->_fPosition), 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTestPosition); //NPC �� ��ġ	
	//m_pNavigationCom->Set_CurrentNaviIndex(vTestPosition);

	m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

	m_pTransformCom->Scaling(_float3{ 0.0025f, 0.0025f, 0.0025f });

	m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-90.f));

	/* �� ���� ���̵��� �ֱ� */
	m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 2.f);

	/* ����ٰ� �ش� �� ���� �ٸ��� ������ �ϸ� �� */
	if (pDesc->iCurLevel != LEVEL_HILL)
	{
		m_iState = STATE_START_WALK;
	}
	else
	{
		m_iState = STATE_LOBBY_IDLE_01;
	}


	m_iPhaseState |= PHASE_START;

	/* �÷��̾� ������ ����ϴ� �ִϸ��̼� �з� */
	Player_Setting_PartAni();

	/*�÷��̾� ���� ����Ǿ� �ִ°� �޾ƿ���*/
#pragma region UIDATA
	CUI_Manager::PLAYER_SAVE_STATE stGetData = {};

	stGetData = m_pGameInstance->Get_Player_State_SaveData();
	if (stGetData.iLevel != 0)
	{
		m_iLevel = stGetData.iLevel;

		m_iFullHp = stGetData.iFullHP;
		m_iCurrentHp = stGetData.iFullHP;

		m_iFullMp = stGetData.iFullMP;
		m_iCurrentMp = stGetData.iFullMP;

		m_fBonusSkillDamage = stGetData.fBonus_SkillDamage;
		m_iAttackPower = stGetData.iAttackPower;
		m_iClawAttackPower = stGetData.iClawAttackPower;

		m_iMemoryFragment = stGetData.iMemoryFragment;
	}

#pragma endregion UIDATA


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Get_DIKeyState(DIK_M) & 0x80) //��ȯ�� �����϶�� ���ܵ� FadeInOut Auto�� 
	//{
	//	m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_AUTO, 2.0f, 0.5f);
	//}

	//m_pPlayerSkillMgr->UnLockSkill(PLAYER_SKILL_AXE);	

#pragma region Mouse_Input

	if (!(m_iPhaseState & PHASE_INTERACTION) && !(m_iPhaseState & PHASE_START) && !(m_iPhaseState & PHASE_BOSS_INTRO) && !m_bUI_End)
	{	 // ���� ���� 
		Mouse_section(fTimeDelta);
#pragma endregion 

#pragma region KeyBoard Input
		Keyboard_section(fTimeDelta);
	}
#pragma endregion 
	__super::Priority_Update(fTimeDelta);

	if (m_iPreState == STATE::STATE_DEAD && m_iState != STATE::STATE_DEAD)
	{
		//m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(-90.f));		
		//_vector vTestPosition = { 83.19f, 5.3f, -117.27f, 1.f }; //���� �� ��ġ  // 3�� 19��		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fRespawnArea)); //NPC �� ��ġ				
		//m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_fRespawnArea));
		m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(XMLoadFloat4(&m_fRespawnArea)));	

	}


#pragma region ĳ���� ȭ��� uv ��ǥ ���		
	_vector vWorldPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	_vector vViewPos = XMVector4Transform(vWorldPos, m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE::D3DTS_VIEW));
	_vector vClipPos = XMVector4Transform(vViewPos, m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE::D3DTS_PROJ));


	// Ŭ�� ���� �� NDC (����ȭ ����̽� ��ǥ)
	vClipPos /= vClipPos.m128_f32[3];



	//NDC �� ȭ�� ��ǥ(0~1 UV)
	m_fObject_UV_Pos.x = vClipPos.m128_f32[0] * 0.5f + 0.5f;
	m_fObject_UV_Pos.y = -vClipPos.m128_f32[1] * 0.5f + 0.5f - 0.2f;

	// �̰� �Ѱ������ 

#pragma endregion 

}

void CPlayer::Mouse_section(_float fTimeDelta)
{

#ifdef _DEBUG


	if (m_pGameInstance->isKeyEnter(DIK_V))
	{
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_KEY1, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_KEY2, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);

		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_BADZOKER, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_BEOMSEUNG, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_FAKER, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_UIN, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_PLAYERMASTER, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_YUBHIN, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);

		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_1, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_2, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_3, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_4, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_5, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_6, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);
		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_7, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this);

		m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, m_pTransformCom->Get_State(CTransform::STATE_POSITION), this, 100);
	}

#endif // _DEBUG
	if (m_pGameInstance->isMouseEnter(DIM_MB) && m_bLockOn)
	{
		m_iPhaseState ^= PHASE_LOCKON;
	}

	if (!m_bLockOn)
	{
		m_iPhaseState &= ~PHASE_LOCKON;
		m_bNextStateCanPlay = true;
	}

	if (m_pGameInstance->isMouseEnter(DIM_LB)
		&& m_iState != STATE_PARRY_L
		&& m_iState != STATE_PARRY_R
		&& m_iState != STATE_ATTACK_LONG_CLAW_01
		&& m_iState != STATE_ATTACK_LONG_CLAW_02
		&& m_iState != STATE_CLAW_CHARGE_START
		&& m_iState != STATE_CLAW_CHARGE_LOOP
		&& m_iState != STATE_CLAW_CHARGE_FULL_ATTACK
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER)
		)
	{
		/* ó�� ���� �۾� */
		if (m_iMonster_Execution_Category != MONSTER_EXECUTION_CATEGORY::MONSTER_START)
		{
			switch (m_iMonster_Execution_Category)
			{
			case MONSTER_EXECUTION_CATEGORY::MONSTER_VARG:
				m_iState = STATE_STUN_EXECUTE_START_VARG;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_NORMAL:
				//m_iState = STATE_LIGHT_EXECUTION_R;
				m_iState = STATE_LIGHT_EXECUTION_L;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_MAGICIAN:
				m_iState = STATE_STUN_EXECUTE_START_MAGICIAN;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_MUTATION_MAGICIAN:
				m_iState = STATE_MAGICIAN_MUTATION_Execution;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_GRACE:
				m_iState = STATE_STUN_EXECUTE_START_GRACE;
				//m_iState = STATE_GRACE_Execution;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_PUNCH_MAN:
				m_iState = STATE_STUN_EXECUTE_START_PUNCHMAN;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_URD:
				m_iState = STATE_STUN_EXECUTE_START_URD;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_BAT:
				m_iState = STATE_STUN_EXECUTE_START_BAT;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_RESEARCHER:
				m_iState = STATE_STUN_EXECUTE_START_RESEARCHER;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_HARMOR:
				m_iState = STATE_STUN_EXECUTE_START_HARMOR;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_JOKER:
				m_iState = STATE_STUN_EXECUTE_START_JOKER;
				break;
			case MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1:
				m_iState = STATE_STUN_EXECUTE_START_VILLAGEM1;
				break;
			default:
				m_iState = STATE_STUN_EXECUTE;
				break;
			}

			m_iPhaseState |= PHASE_EXECUTION;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	

		}

		else if (m_iPhaseState & PHASE_SPRINT)
		{
			m_iState = STATE_SPRINT_ATTACK_L1;
			m_iPhaseState |= PHASE_FIGHT;

			/* ������ ���� ���� */
			m_iPhaseState &= ~PHASE_DASH;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.		
		}


		else
		{
			if ((m_iState == STATE_ATTACK_L1 || m_iState == STATE_SPRINT_ATTACK_L1)
				&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
					&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
			{
				m_pStateMgr->Get_VecState().at(3)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_L2;
			}

			else if (m_iState == STATE_ATTACK_L2
				&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
					&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
			{
				m_pStateMgr->Get_VecState().at(4)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_L3;
			}

			else if (m_iState == STATE_ATTACK_L3
				&& (m_pModel->Get_CurrentAnmationTrackPosition() > 17.f
					&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f)
				&& m_iTalentState & TALENT_HIT_COMBO)
			{
				m_pStateMgr->Get_VecState().at(43)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_L4;
			}

			else if (m_iState == STATE_ATTACK_L4
				&& (m_pModel->Get_CurrentAnmationTrackPosition() > 22.f
					&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
			{
				m_pStateMgr->Get_VecState().at(44)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_L5;
			}

			else
			{
				if (m_iState != STATE_ATTACK_L1 && m_iState != STATE_ATTACK_L2 && m_iState != STATE_ATTACK_L3 && m_iState != STATE_ATTACK_L4 && m_iState != STATE_ATTACK_L5)
				{
					m_pStateMgr->Get_VecState().at(2)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
					m_iState = STATE_ATTACK_L1;
				}

			}

			m_iPhaseState |= PHASE_FIGHT;

			/* ������ ���� ���� */
			m_iPhaseState &= ~PHASE_DASH;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	

		}
		m_iPhaseState &= ~PHASE_PARRY;
	}


	// ��Ŭ���� 0.3���̻� ������ 
	else if (m_pGameInstance->isMousePressed(DIM_RB)
		&& m_iState != STATE_CLAW_CHARGE_START
		&& m_iState != STATE_CLAW_CHARGE_LOOP
		&& m_iState != STATE_ATTACK_LONG_CLAW_01
		&& m_iState != STATE_ATTACK_LONG_CLAW_02
		&& !(m_iPhaseState & PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
	{
		m_fChrageTime += fTimeDelta;
		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	

		if (m_fChrageTime > 0.3f) // �̰� ���ݸ� �� ª�� �غ���	
		{
			if (m_iState != STATE_CLAW_CHARGE_START)
			{
				m_iState = STATE_CLAW_CHARGE_START;
				m_pStateMgr->Get_VecState().at(52)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iPhaseState |= PHASE_FIGHT;
				m_iPhaseState &= ~PHASE_PARRY;

			}

		}
	}




	else if (m_pGameInstance->isMouseRelease(DIM_RB)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
	{

		if (m_iState != STATE_CLAW_CHARGE_START
			&& m_iState != STATE_CLAW_CHARGE_LOOP
			&& m_iState != STATE_CLAW_CHARGE_FULL_ATTACK)
		{
			if (m_iState == STATE_ATTACK_LONG_CLAW_01
				&& (m_pModel->Get_CurrentAnmationTrackPosition() > 45.f)
				&& (m_pModel->Get_CurrentAnmationTrackPosition() < 90.f)) // ���� �߰�
			{
				m_pStateMgr->Get_VecState().at(6)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_LONG_CLAW_02;
			}

			else if (m_iState != STATE_ATTACK_LONG_CLAW_01
				&& m_iState != STATE_ATTACK_LONG_CLAW_02)
			{
				m_pStateMgr->Get_VecState().at(5)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_LONG_CLAW_01;
			}

			m_iPhaseState |= PHASE_FIGHT;


			/* ������ ���� ���� */
			m_iPhaseState &= ~PHASE_DASH;
			m_iPhaseState &= ~PHASE_PARRY;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
		}

		else if (m_iState == STATE_CLAW_CHARGE_LOOP)
		{
			m_iState = STATE_CLAW_CHARGE_FULL_ATTACK;
		}

		m_fChrageTime = 0.f;
	}


	else if (m_iState == STATE_CLAW_CHARGE_LOOP)
	{
		m_pStateMgr->Get_VecState().at(53)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
	}



	/* ���� ���� �� �� �����Ǿ��� �� �ٽ� m_fChrageTime = 0.f; ���� ����� */
	if (!m_pGameInstance->isMousePressed(DIM_RB))
	{
		m_fChrageTime = 0.f;
	}
}

void CPlayer::Keyboard_section(_float fTimeDelta)
{
//#pragma region ��ų���� �ں���	
//	if ((m_pGameInstance->isKeyEnter(DIK_6))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_JAVELIN_SWORD;
//	}
//#pragma endregion 
//#pragma region ��ų���� ���	
//	if ((m_pGameInstance->isKeyEnter(DIK_5))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_GREATSWORD;
//	}
//#pragma endregion 
//#pragma region ��ų���� ������	
//	if ((m_pGameInstance->isKeyEnter(DIK_4))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_CANE_SWORD_SP02;
//	}
//#pragma endregion 
//#pragma region ��ų���� ����		
//	if ((m_pGameInstance->isKeyEnter(DIK_3))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_AXE;
//	}
//#pragma endregion 
//#pragma region ��ų���� ��
//	if ((m_pGameInstance->isKeyEnter(DIK_2))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_SCYTHE_B;
//	}
//#pragma endregion 
//#pragma region ��ų���� �ҹ��� 
//	if ((m_pGameInstance->isKeyEnter(DIK_1))
//		&& m_iState != STATE_DEAD
//		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
//		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
//		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
//		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
//		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
//		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
//	{
//		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	
//		m_iPhaseState |= CPlayer::PHASE_FIGHT;
//		m_iState = STATE_HALBERDS_B;
//	}
#pragma endregion 
	if ((m_pGameInstance->isKeyEnter(DIK_1))
		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER)
		&& m_iState != STATE_DEAD)
	{


		switch (m_iSkill_Eqip_1st)
		{
		case PLAYER_SKILL::PLAYER_SKILL_AXE:
			if (m_pPlayerSkillMgr->Get_VecState().at(0)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_AXE;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1; 

			}
			break;
		case PLAYER_SKILL::PLAYER_SKILL_CANESWORD:
			if (m_pPlayerSkillMgr->Get_VecState().at(1)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_CANE_SWORD_SP02;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1;

			}
			break;
		case PLAYER_SKILL::PLAYER_SKILL_GREADSWORD:
			if (m_pPlayerSkillMgr->Get_VecState().at(2)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_GREATSWORD;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.				
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1;

			}
			break;
		case PLAYER_SKILL::PLAYER_SKILL_HALBERD:
			if (m_pPlayerSkillMgr->Get_VecState().at(3)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_HALBERDS_B;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1;

			}
			break;
		case PLAYER_SKILL::PLAYER_SKILL_JAVELINSWORD:
			if (m_pPlayerSkillMgr->Get_VecState().at(4)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_JAVELIN_SWORD;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1;

			}
			break;
		case PLAYER_SKILL::PLAYER_SKILL_SCYTHE:
			if (m_pPlayerSkillMgr->Get_VecState().at(5)->Check_UnLocked() && m_bPlayerSkill_CoolTime && m_iCurrentMp > 20)
			{
				m_iState = STATE_SCYTHE_B;
				m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
				m_iPhaseState |= CPlayer::PHASE_FIGHT;
				m_iCurrentMp -= 20;
				m_iSkill_input_Key = 1;

			}
			break;
		}

	}


	if ((m_pGameInstance->isKeyEnter(DIK_2) && m_bPlayerSkill_CoolTime)
		&& !(m_iPhaseState & CPlayer::PHASE_FIGHT)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
		&& !(m_iPhaseState & CPlayer::PHASE_PARRY)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER)
		&& m_iState != STATE_DEAD)
	{


		switch (m_iTake_Away_Skill)
		{
		case PLAYER_SKILL::PLAYER_SKILL_AXE:
			m_iState = STATE_AXE;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iSkill_input_Key = 2;
			break;
		case PLAYER_SKILL::PLAYER_SKILL_CANESWORD:
			m_iState = STATE_CANE_SWORD_SP02;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iCurrentMp -= 20;
			m_iSkill_input_Key = 2;
			break;
		case PLAYER_SKILL::PLAYER_SKILL_GREADSWORD:
			m_iState = STATE_GREATSWORD;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.				
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iCurrentMp -= 20;
			m_iSkill_input_Key = 2;
			break;
		case PLAYER_SKILL::PLAYER_SKILL_HALBERD:
			m_iState = STATE_HALBERDS_B;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iCurrentMp -= 20;
			m_iSkill_input_Key = 2;
			break;
		case PLAYER_SKILL::PLAYER_SKILL_JAVELINSWORD:
			m_iState = STATE_JAVELIN_SWORD;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iCurrentMp -= 20;
			m_iSkill_input_Key = 2;

			break;
		case PLAYER_SKILL::PLAYER_SKILL_SCYTHE:
			m_iState = STATE_SCYTHE_B;
			m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.			
			m_iPhaseState |= CPlayer::PHASE_FIGHT;
			m_iCurrentMp -= 20;
			m_iSkill_input_Key = 2;

			break;
		}
	}



#pragma region ���� 
#ifdef _DEBUG
#endif // _DEBUG


	if (/*(m_pGameInstance->isKeyEnter(DIK_T) ||*/ m_iCurrentHp <= 0
		&& m_iState != STATE_DEAD)
	{
		m_iPhaseState = 0;
		m_iPhaseState |= CPlayer::PHASE_DEAD;
		m_iState = STATE_DEAD;
	}
#pragma endregion 

#pragma region ���� �� 
	if (m_pGameInstance->isKeyEnter(DIK_Q)
		&& m_iState != STATE_HEAL
		&& !(m_iPhaseState & PHASE_FIGHT)
		&& !(m_iPhaseState & PHASE_HITTED)
		&& !(m_iPhaseState & PHASE_PARRY)
		&& !(m_iPhaseState & PHASE_DASH)
		&& !(m_iPhaseState & PHASE_EXECUTION)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER)
		&& m_iPotionCount > 0)
	{
		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.		
		m_iPhaseState |= CPlayer::PHASE_HEAL;
		m_iPotionCount--;	 // ���� �� ���� 
		m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, m_iPotion_Heal_Amount);
		m_iCurrentHp += m_iPotion_Heal_Amount;
		if (m_iCurrentHp > m_iFullHp)
			m_iCurrentHp = m_iFullHp;
		m_iState = STATE_HEAL;
	}
#pragma endregion 

#pragma region �и�	
	if (m_pGameInstance->isKeyEnter(DIK_F)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
		&& m_iState != STATE_ATTACK_LONG_CLAW_01
		&& m_iState != STATE_ATTACK_LONG_CLAW_02
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_DEAD)
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
	{
		m_iPhaseState &= ~PHASE_SPRINT;	 //������Ʈ ���� ��Ŵ.	

		if ((m_iState == STATE_PARRY_L ||
			((m_iState == STATE_PARRY_DEFLECT_L || (m_iState == STATE_PARRY_DEFLECT_L_UP))))
			&& m_pModel->Get_CurrentAnmationTrackPosition() > 20.f) // �и� 2��° ���				
		{


			m_pStateMgr->Get_VecState().at(20)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_PARRY_R;


		}
		else if (m_iState != STATE_PARRY_L)  // �и� ��� 
		{
			if (m_iState == STATE_PARRY_R && m_pModel->Get_VecAnimation().at(16)->Get_Current_TrackPoisition() > 20.f)
			{
				m_pStateMgr->Get_VecState().at(19)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_pModel->Get_VecAnimation().at(15)->Set_StartOffSetTrackPosition(10.f);
				m_pGameInstance->Play_Sound(L"Player_Parry_L_Real.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
				m_iState = STATE_PARRY_L;
			}
			else if (m_iState != STATE_PARRY_R && m_iState != STATE_PARRY_DEFLECT_L && m_iState != STATE_PARRY_DEFLECT_L_UP)
			{
				m_pModel->Get_VecAnimation().at(15)->Set_StartOffSetTrackPosition(0.f);
				m_pStateMgr->Get_VecState().at(19)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_PARRY_L;
			}
		}


		m_iPhaseState &= ~CPlayer::PHASE_PARRY; // 3�� 19��	

	}
#pragma endregion	

#pragma region 8���� Run 
	if ((!(m_iPhaseState & PHASE_FIGHT)
		&& !(m_iPhaseState & PHASE_LOCKON)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION))
		&& !(m_iPhaseState & PHASE_PARRY)
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_DEAD)
		&& m_iState != STATE_PARRY_L
		&& m_iState != STATE_PARRY_R
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
	{
#pragma region �뽬 
		if (m_pGameInstance->isKeyEnter(DIK_SPACE))
		{

			if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(31)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_FL;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(30)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_FR;
			}
			else if (m_pGameInstance->isKeyPressed(DIK_B) && m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(34)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_BL;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_B) && m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(33)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_BR;
			}
			else if (m_pGameInstance->isKeyPressed(DIK_S))
			{
				m_pStateMgr->Get_VecState().at(35)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_B;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(29)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_L;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(28)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_R;

			}
			else
			{
				m_pStateMgr->Get_VecState().at(32)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_F;
			}

			m_iPhaseState |= PHASE_DASH;

		}
#pragma endregion 

		else if (
			((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
				|| (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
				|| (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))
				|| (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D)))
			&& !(m_iPhaseState & PHASE_DASH))
		{
			// Sprint �߰� 
			if (m_pGameInstance->isKeyEnter(DIK_LSHIFT))
			{
				m_iPhaseState ^= PHASE_SPRINT;
			}

			if (m_iPhaseState & PHASE_SPRINT)
			{
				m_pStateMgr->Get_VecState().at(64)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_SPRINT;
			}

			else
			{
				m_pStateMgr->Get_VecState().at(1)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_RUN;
			}
		}

		else
		{
			if (!(m_iPhaseState & PHASE_DASH)
				&& m_iState != STATE_PARRY_L
				&& m_iState != STATE_PARRY_R
				&& !(m_iPhaseState & PHASE_PARRY))
			{
				m_iPhaseState &= ~PHASE_SPRINT;
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;

			}
		}

		m_iPhaseState |= PHASE_IDLE;

	}
#pragma endregion 
#pragma region ���¸�� 8���� �̵� 
	if (!(m_iPhaseState & PHASE_FIGHT)
		&& (m_iPhaseState & PHASE_LOCKON)
		&& !(m_iPhaseState & CPlayer::PHASE_HITTED)
		&& !(m_iPhaseState & CPlayer::PHASE_EXECUTION)
		&& !(m_iPhaseState & PHASE_PARRY)
		&& !(m_iPhaseState & CPlayer::PHASE_HEAL)
		&& !(m_iPhaseState & CPlayer::PHASE_DEAD)
		&& m_iState != STATE_PARRY_L
		&& m_iState != STATE_PARRY_R
		&& !(m_iPhaseState & CPlayer::PHASE_LADDER))
	{
		//m_iPhaseState &= ~PHASE_SPRINT;	 // ���� ������ ���� ������Ʈ ���� ��Ŵ.	

		/* �� Ű�Է��� ���ÿ� ������ �� */
		if (((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A)))   // WA	
			|| ((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D)))  // WD
			|| ((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)) && (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A)))  // SA
			|| ((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D)))  // SD 
			|| ((m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D)))  // AD 
			|| ((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)))
			)// WS
		{
			if (((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))) //WA
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(10)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_FL;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����
			}
			else if (((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))) //WD
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(9)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_FR;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}

			else if (((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)) && (m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))) //SA
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(12)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_BL;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}


			else if (((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))) //SD
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(11)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_BR;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����		
			}


			else if (((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W)) && (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))) //WS
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����				
			}

			else if (((m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A)) && (m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))) //AD
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}

		}

		else
		{
			if ((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
				&& m_bNextStateCanPlay)
			{
				if (XMVectorGetX(XMVector3Length(m_pTargetMonsterPtr->Get_Transfrom()->Get_State(CTransform::STATE_POSITION) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
					< 1.f)
				{
					/* ���⼭ �Ÿ��� �ʹ� ����� ���� �ȵǵ��� �����ϱ� */
					/* �ϴ� Ÿ�� ������ �����Ͱ� �ʿ���. */

					m_pStateMgr->Get_VecState().at(10)->Priority_Update(this, m_pNavigationCom, fTimeDelta);

					m_iState = STATE_LOCK_ON_RUN_FL;
					m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����				
				}

				else
				{
					if (m_iPhaseState & PHASE_SPRINT)
					{
						m_pStateMgr->Get_VecState().at(64)->Priority_Update(this, m_pNavigationCom, fTimeDelta);

						m_iState = STATE_SPRINT;
					}
					else
					{
						m_pStateMgr->Get_VecState().at(14)->Priority_Update(this, m_pNavigationCom, fTimeDelta);

						m_iState = STATE_RUN;
					}
				}
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(8)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_L;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(7)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_R;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(13)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_B;
				m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
			}
		}

		if (m_pGameInstance->isKeyPressed(DIK_W)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(18)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_F;
			m_bNextStateCanPlay = false;
			m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	

		}

		else if (m_pGameInstance->isKeyPressed(DIK_A)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(16)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_L;
			m_bNextStateCanPlay = false;
			m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	

		}

		else if (m_pGameInstance->isKeyPressed(DIK_D)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(17)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_R;
			m_bNextStateCanPlay = false;
			m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	

		}

		else if (m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(15)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_B;
			m_bNextStateCanPlay = false;
			m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	

		}

		/* �ƹ� Ű�� �ȴ����ִٸ� IDLE ���·� */
		if (!(m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))  // WŰ	
			&& !(m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S)) // SŰ	
			&& !(m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A)) // AŰ	
			&& !(m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D)) // DŰ	
			&& m_bNextStateCanPlay
			&& (m_iState != STATE_PARRY_L)
			&& (m_iState != STATE_PARRY_R)
			&& (m_iState != STATE_PARRY_DEFLECT_L)
			&& (m_iState != STATE_PARRY_DEFLECT_L_UP)
			&& (m_iState != STATE_PARRY_DEFLECT_R)
			&& (m_iState != STATE_PARRY_DEFLECT_R_UP)
			)
		{
			m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_IDLE;
			m_iPhaseState &= ~PHASE_SPRINT; // ������Ʈ ����	
		}

	}
#pragma endregion 


}

void CPlayer::Can_Move()
{
	if (m_iState != m_iPreState)
		m_bMove = true;

	// �������� �����صα� ȸ�ǹ� �ٸ� ���ֱ� �� 
	if (m_iState == STATE_LOCK_ON_EVADE_B
		|| m_iState == STATE_LOCK_ON_EVADE_L
		|| m_iState == STATE_LOCK_ON_EVADE_R
		|| m_iState == STATE_NORMAL_EVADE_B
		|| m_iState == STATE_NORMAL_EVADE_BL
		|| m_iState == STATE_NORMAL_EVADE_BR
		|| m_iState == STATE_NORMAL_EVADE_FL
		|| m_iState == STATE_NORMAL_EVADE_FR
		|| m_iState == STATE_NORMAL_EVADE_L
		|| m_iState == STATE_NORMAL_EVADE_R
		|| m_iState == STATE_HurtMFR_L
		|| m_iState == STATE_HurtMFR_R
		|| m_iState == STATE_HURT_LF
		|| m_iState == STATE_HURT_SF
		|| m_iState == STATE_HURT_SL
		|| m_iState == STATE_HURT_HURXXLF
		|| m_iState == STATE_HURT_KNOCKBACK
		|| m_iState == STATE_HURT_KNOCKDOWN
		|| m_iState == STATE_HURT_FALLDOWN
		|| m_iState == STATE_WEAK_GETUP_F
		|| m_iState == STATE_STUN_EXECUTE
		|| m_iState == STATE_HARMOR_EXECUTION
		|| m_iState == STATE_LV1Villager_M_Execution
		|| m_iState == STATE_Joker_Execution
		|| m_iState == STATE_Varg_Execution
		|| m_iState == STATE_REBOUND_R
		|| m_iState == STATE_CLAW_LONG_PLUNDER_ATTACK2
		|| m_iState == STATE_CATCHED
		|| m_iState == STATE_VARG_RUN_EXECUTION
		|| m_iPhaseState & PHASE_LADDER
		|| m_iPhaseState & PHASE_EXECUTION)
	{
		m_bMove = true;
	}

	if (m_iState == STATE_STUN_EXECUTE_START_URD)
	{
		m_bMove = false;
	}
}




void CPlayer::Update(_float fTimeDelta)
{

	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);


	//if (m_pGameInstance->isKeyEnter(DIK_P))	
	//{
	//	m_pGameInstance->Add_RippleInfo(_float2(XMVectorGetX(vCurPosition), XMVectorGetZ(vCurPosition)), 1.f);	
	//} // �ĵ� �߰��ڵ�


	// �� state ����Ǹ� �ѹ� bool������ �����ϰ� �ش� ������Ʈ���� ���� �߻��ߴ� �ϸ� �������� �ʵ��� ���̻� 


	Can_Move(); // �̰� ���Ϳ� ��ġ�� �ʰ� �ϴ� ��Ʈ �ִϸ��̼� ���� 


	_vector CheckRootMatrix = { 0.f,0.f,0.f,1.f };
	/* ��Ʈ ��� �ִϸ޼� �ڵ� */
	m_pRootMatrix = m_pModel->Get_RootMotionMatrix("root");

	if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], CheckRootMatrix) && m_pModel->Get_LerpFinished() && m_bMove)
	{
		if (m_pNavigationCom->isMove(vCurPosition))
			m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix); // �׷� ��ȯ�ɶ��� ���ϱ������.

		/* 2�� 19�� �߰� �ڵ� */
		if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
		{
			_float4x4 PreMatirx = {};
			_vector PrePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			XMStoreFloat4x4(&PreMatirx, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
			const _float4x4* Const_PreMatirx = const_cast<_float4x4*>(&PreMatirx);
			m_pTransformCom->Set_MulWorldMatrix(Const_PreMatirx);

			// Pretest �� test2 �� ������ ���ؼ� �־����
			_vector Curtest = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			_float MoveSpeed = XMVectorGetX(XMVector3Length(PrePos - Curtest));

			/* ��Ʈ �ִϸ��̼��� �̵��� ��ŭ�� �����̵� �� �� �ְ� �����ϱ�*/
			m_pTransformCom->Sliding_Root_Ani(fTimeDelta * 0.01f, m_pNavigationCom, MoveSpeed);
		}
	}

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	if (!(m_iPhaseState & PHASE_LADDER))
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

	__super::Update(fTimeDelta);

	//m_pPlayerSkillMgr->Update_Skill_CoolTime(this, fTimeDelta);  Ui�ʿ��� �ٷ��� �� ( 4/15 )  

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	/* ��� �׺���̼� ��Ȯ�� Ÿ���� Ȯ���ҷ��� �ִ� �ڵ� */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;


	/* ���� ���� �����ϱ� */
	m_iPreState = m_iState;

	m_iPrePhaseState = m_iPhaseState;


	/* ����� ���� ���� */
	if (m_iPhaseState & PHASE_EXECUTION
		|| m_iPhaseState & PHASE_BOSS_INTRO
		|| m_iState == STATE_CATCHED
		|| m_iState == STATE_MAGICIAN_CATCH
		|| m_iState == STATE_LOBBY_IDLE_01
		|| m_iState == STATE_LOBBY_IDLE_01_END
		|| m_iState == STATE_RESEARCHER_EXECUTION	
		|| m_iState == STATE_HURT_RESEARCHER_CATCHED	
		|| m_iState == STATE_HURT_MUTATION_MAGICIAN_CATCH)	
	{
		m_pGameInstance->Set_Dithering(false);
	}

	else
		m_pGameInstance->Set_Dithering(true);

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
#endif 

	return S_OK;
}

HRESULT CPlayer::Ready_Components(void* _pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(_pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),		 // �̰� ���߿� Current Scene���� �ٲ���ҵ� 
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects(void* _pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(_pArg);

	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentActor = m_pActor;
	BodyDesc.pParent = this;
	BodyDesc.pParentNavigationCom = m_pNavigationCom;
	BodyDesc.pParentStateMgr = m_pStateMgr;
	BodyDesc.pParentState = &m_iState;
	BodyDesc.pPreParentState = &m_iPreState;
	BodyDesc.pParentPhaseState = &m_iPhaseState;
	BodyDesc.pParentNextStateCan = &m_bNextStateCanPlay;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.pParentExectueMonsterState = &m_iMonster_Execution_Category;

	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	BodyDesc.pParentHp = &m_iCurrentHp;
	BodyDesc.pParentMp = &m_iCurrentMp;

	BodyDesc.pParentTalent = &m_iTalentState;

	BodyDesc.iCurLevel = pDesc->iCurLevel;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_STATIC, TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	m_pModel = dynamic_cast<CModel*>(Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));


	/* ������ ���⸦ �����. */
	CRightWeapon::WEAPON_DESC		RightWeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;

	RightWeaponDesc.pParent = this;
	RightWeaponDesc.pParentModel = m_pModel;
	RightWeaponDesc.pParentState = &m_iState;
	RightWeaponDesc.pParentPhaseState = &m_iPhaseState;
	RightWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	RightWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightWeaponDesc.fSpeedPerSec = 0.f;
	RightWeaponDesc.fRotationPerSec = 10.f;
	RightWeaponDesc.pParentHp = &m_iCurrentHp;
	RightWeaponDesc.pParentMp = &m_iCurrentMp;


	RightWeaponDesc.iCurLevel = pDesc->iCurLevel;


	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Weapon"), LEVEL_STATIC, TEXT("Prototype_GameObject_Right_Weapon"), &RightWeaponDesc)))
		return E_FAIL;

	/* ���� ���⸦ �����. */
	CPlayer_Weapon_Axe::WEAPON_DESC		Weapon_Axe_Desc{};


	Weapon_Axe_Desc.pParent = this;
	Weapon_Axe_Desc.pParentModel = m_pModel;
	Weapon_Axe_Desc.pParentState = &m_iState;
	Weapon_Axe_Desc.pPreParentState = &m_iPreState;
	Weapon_Axe_Desc.pParentPhaseState = &m_iPhaseState;
	Weapon_Axe_Desc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_Axe_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Axe_Desc.fSpeedPerSec = 0.f;
	Weapon_Axe_Desc.fRotationPerSec = 10.f;

	Weapon_Axe_Desc.iCurLevel = pDesc->iCurLevel;
	Weapon_Axe_Desc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Axe"), LEVEL_STATIC, TEXT("Prototype_GameObject_Axe"), &Weapon_Axe_Desc)))
		return E_FAIL;


	/* �ҹ��� ���⸦ �����. */
	CWeapon_Halberd::WEAPON_DESC		Weapon_HalberdDesc{};


	Weapon_HalberdDesc.pParent = this;
	Weapon_HalberdDesc.pParentModel = m_pModel;
	Weapon_HalberdDesc.pParentState = &m_iState;
	Weapon_HalberdDesc.pPreParentState = &m_iPreState;
	Weapon_HalberdDesc.pParentPhaseState = &m_iPhaseState;
	Weapon_HalberdDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_HalberdDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_HalberdDesc.fSpeedPerSec = 0.f;
	Weapon_HalberdDesc.fRotationPerSec = 10.f;

	Weapon_HalberdDesc.iCurLevel = pDesc->iCurLevel;
	Weapon_HalberdDesc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Halberd"), LEVEL_STATIC, TEXT("Prototype_GameObject_Halberd"), &Weapon_HalberdDesc)))
		return E_FAIL;


	/*�ں��� ���⸦ �����. */
	CPlayer_Weapon_JavelinSword::WEAPON_DESC		Weapon_JavelinSwordDesc{};

	Weapon_JavelinSwordDesc.pParent = this;
	Weapon_JavelinSwordDesc.pParentModel = m_pModel;
	Weapon_JavelinSwordDesc.pParentState = &m_iState;
	Weapon_JavelinSwordDesc.pPreParentState = &m_iPreState;
	Weapon_JavelinSwordDesc.pParentPhaseState = &m_iPhaseState;
	Weapon_JavelinSwordDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_JavelinSwordDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_JavelinSwordDesc.fSpeedPerSec = 0.f;
	Weapon_JavelinSwordDesc.fRotationPerSec = 10.f;

	Weapon_JavelinSwordDesc.iCurLevel = pDesc->iCurLevel;
	Weapon_JavelinSwordDesc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Javelin_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Javelin_Sword"), &Weapon_JavelinSwordDesc)))
		return E_FAIL;


	/*��� ���⸦ �����. */
	CPlayer_Weapon_GreadSword::WEAPON_DESC		Weapon_GreadSwordDesc{};

	Weapon_GreadSwordDesc.pParent = this;
	Weapon_GreadSwordDesc.pParentModel = m_pModel;
	Weapon_GreadSwordDesc.pParentState = &m_iState;
	Weapon_GreadSwordDesc.pPreParentState = &m_iPreState;
	Weapon_GreadSwordDesc.pParentPhaseState = &m_iPhaseState;
	Weapon_GreadSwordDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_GreadSwordDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_GreadSwordDesc.fSpeedPerSec = 0.f;
	Weapon_GreadSwordDesc.fRotationPerSec = 10.f;

	Weapon_GreadSwordDesc.iCurLevel = pDesc->iCurLevel;
	Weapon_GreadSwordDesc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_GreadSword"), LEVEL_STATIC, TEXT("Prototype_GameObject_GreadSword"), &Weapon_GreadSwordDesc)))
		return E_FAIL;

	/*������ ���⸦ �����. */
	CPlayer_Weapon_Cane::WEAPON_DESC		Weapon_Cane_Desc{};

	Weapon_Cane_Desc.pParent = this;
	Weapon_Cane_Desc.pParentModel = m_pModel;
	Weapon_Cane_Desc.pParentState = &m_iState;
	Weapon_Cane_Desc.pPreParentState = &m_iPreState;
	Weapon_Cane_Desc.pParentPhaseState = &m_iPhaseState;
	Weapon_Cane_Desc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_Cane_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Cane_Desc.fSpeedPerSec = 0.f;
	Weapon_Cane_Desc.fRotationPerSec = 10.f;

	Weapon_Cane_Desc.iCurLevel = pDesc->iCurLevel;


	if (FAILED(__super::Add_PartObject(TEXT("Part_Player_Cane"), LEVEL_STATIC, TEXT("Prototype_GameObject_Cane"), &Weapon_Cane_Desc)))
		return E_FAIL;



	/*������ �� ���⸦ �����. */
	CPlayer_Weapon_Cane_Sword::WEAPON_DESC		Weapon_Cane_Sword_Desc{};

	Weapon_Cane_Sword_Desc.pParent = this;
	Weapon_Cane_Sword_Desc.pParentModel = m_pModel;
	Weapon_Cane_Sword_Desc.pParentState = &m_iState;
	Weapon_Cane_Sword_Desc.pPreParentState = &m_iPreState;
	Weapon_Cane_Sword_Desc.pParentPhaseState = &m_iPhaseState;
	Weapon_Cane_Sword_Desc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_Cane_Sword_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Cane_Sword_Desc.fSpeedPerSec = 0.f;
	Weapon_Cane_Sword_Desc.fRotationPerSec = 10.f;

	Weapon_Cane_Sword_Desc.iCurLevel = pDesc->iCurLevel;
	Weapon_Cane_Sword_Desc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Player_Cane_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Cane_Sword"), &Weapon_Cane_Sword_Desc)))
		return E_FAIL;


	/*�� ���⸦ �����. */
	CWeapon_Scythe::WEAPON_DESC		Weapon_ScytheDesc{};

	Weapon_ScytheDesc.pParent = this;
	Weapon_ScytheDesc.pParentModel = m_pModel;
	Weapon_ScytheDesc.pParentState = &m_iState;
	Weapon_ScytheDesc.pPreParentState = &m_iPreState;
	Weapon_ScytheDesc.pParentPhaseState = &m_iPhaseState;
	Weapon_ScytheDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	Weapon_ScytheDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_ScytheDesc.fSpeedPerSec = 0.f;
	Weapon_ScytheDesc.fRotationPerSec = 10.f;

	Weapon_ScytheDesc.iCurLevel = pDesc->iCurLevel;
	Weapon_ScytheDesc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Scythe"), LEVEL_STATIC, TEXT("Prototype_GameObject_Scythe"), &Weapon_ScytheDesc)))
		return E_FAIL;


	/* �޼� ���⸦ �����. */
	CLeftWeapon::WEAPON_DESC		LeftWeaponDesc{};

	LeftWeaponDesc.pParent = this;
	LeftWeaponDesc.pParentState = &m_iState;
	LeftWeaponDesc.pParentPhaseState = &m_iPhaseState;
	LeftWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* ĳ���� �𵨸��� �ٸ� */
	LeftWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	LeftWeaponDesc.fSpeedPerSec = 0.f;
	LeftWeaponDesc.fRotationPerSec = 10.f;

	LeftWeaponDesc.iCurLevel = pDesc->iCurLevel;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Left_Weapon"), LEVEL_STATIC, TEXT("Prototype_GameObject_Left_Weapon"), &LeftWeaponDesc)))
		return E_FAIL;



	/* ������ ���� ���⸦ �����. */
	CClawWeapon::WEAPON_DESC		RightClawWeaponDesc{};

	RightClawWeaponDesc.pParent = this;
	RightClawWeaponDesc.pParentModel = m_pModel;
	RightClawWeaponDesc.pParentState = &m_iState;
	RightClawWeaponDesc.pParentPhaseState = &m_iPhaseState;
	RightClawWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	RightClawWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightClawWeaponDesc.fSpeedPerSec = 0.f;
	RightClawWeaponDesc.fRotationPerSec = 10.f;

	RightClawWeaponDesc.iCurLevel = pDesc->iCurLevel;

	RightClawWeaponDesc.pParentHp = &m_iCurrentHp;
	RightClawWeaponDesc.pParentMp = &m_iCurrentMp;
	RightClawWeaponDesc.pParentSkillBonusDamage = &m_fBonusSkillDamage;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Claw"), LEVEL_STATIC, TEXT("Prototype_GameObject_Right_Claw"), &RightClawWeaponDesc)))
		return E_FAIL;


	/* �÷��̾� ī�޶� */
	CPlayerCamera::WEAPON_DESC    PlayerCameraDesd{};

	PlayerCameraDesd.pParentActor = m_pActor;
	PlayerCameraDesd.pParent = this;
	PlayerCameraDesd.pParentModel = m_pModel;
	PlayerCameraDesd.pParentState = &m_iState;
	PlayerCameraDesd.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("camera"); /* ĳ���� �𵨸��� �ٸ� */
	PlayerCameraDesd.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	PlayerCameraDesd.fSpeedPerSec = 0.f;
	PlayerCameraDesd.fRotationPerSec = 10.f;

	PlayerCameraDesd.iCurLevel = pDesc->iCurLevel;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Player_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerCamera"), &PlayerCameraDesd)))
		return E_FAIL;


	return S_OK;
}


void CPlayer::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	/* ���� ������� �浹 */
	if (!strcmp("MONSTER_WEAPON", _pOther->Get_Name()))
	{
		/* �ӽ� ���� ���� ���� ���� �� �� �ȱ�  */
		m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE);


		/* �浹 ���� �� ������ ���� ���� �Ǻ� �ؾ��� */
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����		
		_information.extractContacts(contactPoints, 1);

		PxVec3 position = contactPoints[0].position;
		PxVec3 dir = contactPoints[0].normal;
		_vector PlayerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		int a = 4;

#pragma region EFFECT
		m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP);
#pragma endregion

		/* �и� ���� �Ͻ� */
		if (m_iPhaseState & CPlayer::PHASE_PARRY)
		{
			//int Parry = rand() % 4;
			_vector vHitPosition = { position.x, position.y, position.z, 1.f };
			/* ���⼭ �÷��̾��� Ư�� ������ */
			switch (m_iState)
			{
			case STATE::STATE_PARRY_L:
			{
				switch (m_iParryMotion)
				{
				case 0:
					m_iState = STATE_PARRY_DEFLECT_L;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 1:
					m_iState = STATE_PARRY_DEFLECT_R_UP;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 2:
					m_iState = STATE_PARRY_DEFLECT_L_UP;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 3:
					m_iState = STATE_PARRY_DEFLECT_R;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion = 0;
					break;
				}


				if ((m_iCurrentMp + m_iBonus_Parry_Mp) > m_iFullMp) // Mp�� �̹� �� Ŭ ��					
				{
					if (m_iFullMp > m_iCurrentMp)
						m_iCurrentMp += m_iFullMp - m_iCurrentMp;
				}
				else
				{
					m_iCurrentMp += m_iBonus_Parry_Mp;
				}

				m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT, vHitPosition, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			}
			break;
			case STATE::STATE_PARRY_R:
				switch (m_iParryMotion)
				{
				case 0:
					m_iState = STATE_PARRY_DEFLECT_L;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 1:
					m_iState = STATE_PARRY_DEFLECT_R_UP;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 2:
					m_iState = STATE_PARRY_DEFLECT_L_UP;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion++;
					break;
				case 3:
					m_iState = STATE_PARRY_DEFLECT_R;
					m_iPhaseState &= ~CPlayer::PHASE_FIGHT;
					m_iParryMotion = 0;
					break;
				}


				if ((m_iCurrentMp + m_iBonus_Parry_Mp) > m_iFullMp) // Mp�� �̹� �� Ŭ ��				      
				{
					if (m_iFullMp > m_iCurrentMp)
						m_iCurrentMp += m_iFullMp - m_iCurrentMp;
				}
				else
				{
					m_iCurrentMp += m_iBonus_Parry_Mp;
				}

				m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_RIGHT, vHitPosition, m_pTransformCom->Get_State(CTransform::STATE_LOOK));
				break;
			}
		}


		else  // ����ٰ� �и� ������ ��Ȳ�� �־�� �ڳ�. 
		{

			if (m_set_Player_Skill_State.count((CPlayer::STATE)m_iState))
			{
				//Hit Effect
				_vector vHitPosition = { position.x, position.y, position.z, 1.f };
				_vector vHitDir = { dir.x, dir.y, dir.z, 1.f };
				m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_PLAYER_HIT_HOLDING, vHitPosition, vHitDir);

				/* ������ �ȹޱ� ���� */
				m_iCurrentHp -= *dynamic_cast<CPartObject*>(_pOther)->Get_Monster_Attack_Ptr();
				if (m_iCurrentHp <= 0)
					m_iCurrentHp = 0;
			}

			else
			{
				/* ���� ���°� ���� �� �ϱ� */
				m_pGameInstance->StopSound(SOUND_PLAYER_ATTACK_1);
				m_pGameInstance->StopSound(SOUND_PLAYER_ATTACK_2);
				m_pGameInstance->StopSound(SOUND_PLAYER_ACTION_1);
				m_pGameInstance->StopSound(SOUND_PLAYER_ACTION_2);

				/* �и� ���� �� ( �� ���� �� ) */
				m_iPhaseState &= ~CPlayer::PHASE_PARRY;	   // 3�� 19��
				m_iPhaseState &= ~CPlayer::PHASE_DASH;	   // 3�� 19�� 
				m_iPhaseState &= ~CPlayer::PHASE_FIGHT;	   // 3�� 19��
				m_iPhaseState &= ~CPlayer::PHASE_IDLE;     // 3�� 19��
				m_iPhaseState &= ~CPlayer::PHASE_DASH;     // 3�� 19��
				m_iPhaseState &= ~CPlayer::PHASE_EXECUTION;	  // 3�� 19�� 
				m_iPhaseState &= ~CPlayer::PHASE_HEAL;
				m_iPhaseState &= ~CPlayer::PHASE_SPRINT;
				m_iPhaseState &= ~CPlayer::PHASE_LADDER;


				if (!(m_iPhaseState & PHASE_DEAD))
				{
					m_iPhaseState |= CPlayer::PHASE_HITTED;    // 3�� 19�� 

					_float4 fMonsterLookDir = {};
					const _float4x4* ParentMatrix = dynamic_cast<CPartObject*>(_pOther)->Get_ParentWorldMatrix();
					fMonsterLookDir = { ParentMatrix->_31,ParentMatrix->_32,ParentMatrix->_33,0.f };


					/* ������ �ȹޱ� ���� */
					m_iCurrentHp -= *dynamic_cast<CPartObject*>(_pOther)->Get_Monster_Attack_Ptr();
					if (m_iCurrentHp <= 0)
						m_iCurrentHp = 0;


					switch (dynamic_cast<CPartObject*>(_pOther)->Get_Parent_Ptr()->Get_Player_Hitted_State())
					{
					case Player_Hitted_State::PLAYER_HURT_FallDown:
						m_iState = CPlayer::STATE_HURT_FALLDOWN;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(42)->Set_MonsterLookDir(fMonsterLookDir);
						m_pStateMgr->Get_VecState().at(42)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_HURTLF:
						m_iState = CPlayer::STATE_HURT_LF;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(38)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(38)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_HURTMFL: // 31�� �ִϸ��̼� �ε���
						m_iState = CPlayer::STATE_HurtMFR_L;  // 22		
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(21)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(21)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_HURTSF:
						m_iState = CPlayer::STATE_HURT_SF;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(39)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(39)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_HURTSL:
						m_iState = CPlayer::STATE_HURT_SL;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(41)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(41)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit04.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_HURXXLF:
						m_iState = CPlayer::STATE_HURT_HURXXLF;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(40)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(40)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_KnockBackF:
						m_iState = CPlayer::STATE_HURT_KNOCKBACK;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(36)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(36)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_KNOCKDOWN:
						m_iState = CPlayer::STATE_HURT_KNOCKDOWN;
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(37)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(37)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						//m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;

					case Player_Hitted_State::PLAYER_HURT_REBOUND:
						m_iState = CPlayer::STATE_REBOUND_R;  // 46					
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(46)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(46)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						//m_pGameInstance->Play_Sound(L"Player_AttackHit06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.3f);
						break;
					case Player_Hitted_State::PLAYER_HURT_STUN:
						m_iState = CPlayer::STATE_STUNNED_START;  // 47						
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(47)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(47)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						break;
					case Player_Hitted_State::PLAYER_HURT_CATCH:
					{
						m_iState = CPlayer::STATE_CATCHED;  // 55									
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(55)->Set_MonsterLookDir(fMonsterLookDir);

						/* �ٱ� �� ��ġ �������� */
						_float4x4 VargHandWorldMatrix = dynamic_cast<CVargKnife*>(_pOther)->Get_HandWorldMatrix();
						_vector VargHandRightDir = { VargHandWorldMatrix._11, VargHandWorldMatrix._12, VargHandWorldMatrix._13, 0.f };
						_vector VargHandUpDir = { VargHandWorldMatrix._21, VargHandWorldMatrix._22, VargHandWorldMatrix._23, 0.f };
						VargHandRightDir = XMVector3Normalize(VargHandRightDir);
						VargHandUpDir = XMVector3Normalize(VargHandUpDir);

						_vector VargHandPos = { VargHandWorldMatrix._41 , VargHandWorldMatrix._42,VargHandWorldMatrix._43,1.f };

						VargHandPos = VargHandPos - VargHandRightDir * -1.f * 0.2f;

						_float4 FinalPos = {};
						XMStoreFloat4(&FinalPos, VargHandPos);

						m_pStateMgr->Get_VecState().at(55)->Set_GetMonsterPos(FinalPos);

						m_pStateMgr->Get_VecState().at(55)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						break;
					}
					case Player_Hitted_State::PLAYER_HURT_MAGICIAN_CATCH:
					{
						m_iState = CPlayer::STATE_MAGICIAN_CATCH;  // 63												
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(63)->Set_MonsterLookDir(fMonsterLookDir);
						//m_pStateMgr->Get_VecState().at(63)->Set_GetMonsterPos(fMonsterPos);		

						m_pStateMgr->Get_VecState().at(63)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pGameInstance->Sub_Actor_Scene(m_pActor);
						break;
					}
					case Player_Hitted_State::PLAYER_HURT_MUTATION_MAGICIAN_CATCH:
					{
						m_iState = CPlayer::STATE_HURT_MUTATION_MAGICIAN_CATCH;  // 65												
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(65)->Set_MonsterLookDir(fMonsterLookDir);

						_float4 FinalPos = { -42.f,m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],-100.46f,1.f };
						m_pStateMgr->Get_VecState().at(65)->Set_GetMonsterPos(FinalPos);

						m_pStateMgr->Get_VecState().at(65)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pGameInstance->Sub_Actor_Scene(m_pActor);
						break;
					}
					case Player_Hitted_State::PLAYER_HURT_RESEARCH_CATCH:
						m_iState = CPlayer::STATE_HURT_RESEARCHER_CATCHED;  // 66													
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(66)->Set_MonsterLookDir(fMonsterLookDir);
						m_pStateMgr->Get_VecState().at(66)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pGameInstance->Sub_Actor_Scene(m_pActor);
						//m_pGameInstance->Play_Sound(L"Researcher_GrabYou_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.7f);
						break;
					default:
						_uint test = dynamic_cast<CPartObject*>(_pOther)->Get_Parent_Ptr()->Get_Player_Hitted_State();
						m_iState = CPlayer::STATE_HurtMFR_R;  // 22			
						/* ���� ���� ���� */
						m_pStateMgr->Get_VecState().at(22)->Set_MonsterLookDir(fMonsterLookDir);

						m_pStateMgr->Get_VecState().at(22)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
						m_pModel->Set_Continuous_Ani(true);
						break;
					}


#pragma region Effect 0321����
					//Hit Effect
					_vector vHitPosition = { position.x, position.y, position.z, 1.f };
					_vector vHitDir = { dir.x, dir.y, dir.z, 1.f };
					m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_PLAYER_HIT_HOLDING, vHitPosition, vHitDir);
#pragma endregion
				}
			}
		}
	}


	if (!strcmp("MONSTER", _pOther->Get_Name()))
	{
		m_bMove = false;
	}

	if (!strncmp("SM_Door_Left", _pOther->Get_Name(), 12))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����				
		_information.extractContacts(contactPoints, 1);

		PxVec3  dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });

		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);

		m_bMove = false;
	}

	if (!strncmp("SM_Door_Right", _pOther->Get_Name(), 13))
	{

		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����					
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });

		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);

		m_bMove = false;
	}


	if (!strcmp("DESTRUCT", _pOther->Get_Name()))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����							
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });

		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);

	}
}

// �Ѱ� �� �з� 

void CPlayer::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("MONSTER", _pOther->Get_Name()))
	{
		if ((m_pGameInstance->isKeyPressed(DIK_W) || m_pGameInstance->isKeyPressed(DIK_S)
			|| m_pGameInstance->isKeyPressed(DIK_A) || m_pGameInstance->isKeyPressed(DIK_D))
			&& !(m_iPhaseState & PHASE_LOCKON)
			&& (m_iPhaseState & PHASE_IDLE)
			&& m_iState != STATE_NORMAL_EVADE_F)
		{
			Slide_Move(_pOther);
		}

		m_bMove = false;

		/* ���� ���� �޾ƿ��� */
		//m_iMonster_State  = _pOther->Get_Monster_State();			
		m_iMonster_Execution_Category = _pOther->Get_Monster_Execution_Category();
		/* =========================== */
	}



	Player_Interaction(_pOther);



	if (!strncmp("SM_Door_Left", _pOther->Get_Name(), 12))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����			
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });


		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);

		m_bMove = false;
	}

	if (!strncmp("SM_Door_Right", _pOther->Get_Name(), 13))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����				
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });


		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);

		m_bMove = false;
	}

	if (!strcmp("DESTRUCT", _pOther->Get_Name()))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����								
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });

		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);
	}

	if (!strcmp("BARRIERSCREEN", _pOther->Get_Name()))
	{
		PxContactPairPoint contactPoints[1]; // �ִ� 10������ ����									
		_information.extractContacts(contactPoints, 1);

		PxVec3 dir = contactPoints[0].normal;
		_vector Dir = XMVector3Normalize({ dir.x, dir.y, dir.z });

		m_pTransformCom->Go_Dir(Dir, m_pNavigationCom, m_fTimeDelta * 0.075f);
	}
}

void CPlayer::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
	m_bMove = true;

	//m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;	 // �� �κ� ��� �� �غ��� �� �� ����.	
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CPlayer::Slide_Move(CGameObject* pGameObject)
{
	//_vector vSlider = vPlayerLook - vMonsterLook;	
	// �÷��̾� �̵� ���� (�Ի� ����)
	_vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// ���Ͱ� �÷��̾�� �ε����� �븻 ����  
	_vector vNoramlVec = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - (pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION)));

	if (XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec)) < 0.f)
	{
		// ���� ����: �÷��̾� �̵� ���͸� ���� ǥ�� ����(Normal)�� ����
		_vector vProj = vNoramlVec * XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec));

		// �����̵� ���� = ���� �̵� ���� - ������ ����
		_vector vSlider = vPlayerLook - vProj;

		// ���� �̵� ����
		m_pTransformCom->Go_Backward_With_Navi(m_fTimeDelta * 0.075f, m_pNavigationCom);

		m_pTransformCom->Go_Dir(vSlider, m_pNavigationCom, m_fTimeDelta * 0.075f);
	}
}

void CPlayer::Player_Interaction(CGameObject* _pOther)
{
	if (!strncmp("NPCLamp", _pOther->Get_Name(), 7))
	{
		if (m_pGameInstance->isKeyEnter(DIK_E))
		{
			if (m_iState != STATE_ARCHIVE_SIT_LIGHT_UP)
			{
				_float4 fChairLookDir = {};
				_float4 fChairRightDir = {};
				_float4 fChairPos = {};
				const _float4x4* ChiarWolrdMatrix = dynamic_cast<CChairLamp*>(_pOther)->Get_Chair_GameObj()->Get_Transfrom()->Get_WorldMatrix_Ptr();
				fChairLookDir = { ChiarWolrdMatrix->_31,ChiarWolrdMatrix->_32,ChiarWolrdMatrix->_33,0.f };

				XMStoreFloat4(&fChairRightDir, XMVector3Normalize(XMLoadFloat4(&fChairRightDir)));


				fChairPos = { ChiarWolrdMatrix->_41,ChiarWolrdMatrix->_42,ChiarWolrdMatrix->_43,1.f };

				m_pStateMgr->Get_VecState().at(51)->Set_MonsterLookDir(fChairLookDir);
				m_pStateMgr->Get_VecState().at(51)->Set_GetMonsterPos(fChairPos);
				m_pStateMgr->Get_VecState().at(51)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				m_iState = STATE_ARCHIVE_SIT_LIGHT_UP;
				m_iPhaseState |= PHASE_INTERACTION;



				XMStoreFloat4(&fChairLookDir, XMVector3Normalize(XMLoadFloat4(&fChairLookDir)));

				_vector vRespawn = XMLoadFloat4(&fChairPos) + XMLoadFloat4(&fChairLookDir) * 1.f;

				XMStoreFloat4(&m_fRespawnArea, vRespawn);	// �ٽ� ��Ƴ��� �� �����ϱ�.		

			}
		}
	}


	/* ���ڿ� �浹 ���� ��. */
	else if (!strncmp("P_Archive_Chair", _pOther->Get_Name(), 15))  // ��ȣ�ۿ��� 1�����ӿ� 1���� �ҰŴϱ� else if�� ���� 
	{
		if (m_pGameInstance->isKeyEnter(DIK_E))
		{
			if (m_iState != STATE_ARCHIVE_SIT_LOOP)
			{
				_float4 fChairLookDir = {};
				_float4 fChairPos = {};
				const _float4x4* ChiarWolrdMatrix = _pOther->Get_Transfrom()->Get_WorldMatrix_Ptr();
				fChairLookDir = { ChiarWolrdMatrix->_31,ChiarWolrdMatrix->_32,ChiarWolrdMatrix->_33,0.f };
				fChairPos = { ChiarWolrdMatrix->_41,ChiarWolrdMatrix->_42,ChiarWolrdMatrix->_43,1.f };

				m_pStateMgr->Get_VecState().at(48)->Set_MonsterLookDir(fChairLookDir);
				m_pStateMgr->Get_VecState().at(48)->Set_GetMonsterPos(fChairPos);
				m_pStateMgr->Get_VecState().at(48)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				m_iState = STATE_ARCHIVE_SIT_START;
				m_iPhaseState |= PHASE_INTERACTION;
			}


		}

		if (m_iPhaseState & PHASE_INTERACTION)
		{
			if (m_pGameInstance->isKeyEnter(DIK_ESCAPE) && !m_bUI_End)		 // !m_bUI_End  �����̰� false�϶��� ui �� ������ �Ŷ�� ��..
			{
				m_iState = STATE_ARCHIVE_SIT_GETUP;

				m_iAttackPower += m_iBonus_Sword_Attack_Power;  // ���ڿ��� �Ͼ ���� Ư�� ���� Ÿ�̹�	

				m_iPotionCount = 3;
				m_iCurrentHp = m_iFullHp;
				m_iCurrentMp = m_iFullMp;
			}
		}
	}


	/* ��ٸ��� �浹 ���� ��. */
	else if (!strncmp("P_Ladder02_Up", _pOther->Get_Name(), 13))
	{
		if (m_pGameInstance->isKeyEnter(DIK_E))
		{
			/*if (m_pModel->Get_VecAnimation().at(46)->isAniMationFinish())*/
			//{
			_float4 fLadderLookDir = {};
			_float4 fLadderUpDir = {};
			_float4 fLadderPos = {};
			const _float4x4* LadderWolrdMatrix = _pOther->Get_Transfrom()->Get_WorldMatrix_Ptr();
			fLadderLookDir = { LadderWolrdMatrix->_31,LadderWolrdMatrix->_32,LadderWolrdMatrix->_33,0.f };
			fLadderUpDir = { LadderWolrdMatrix->_21,LadderWolrdMatrix->_22,LadderWolrdMatrix->_23,0.f };
			fLadderPos = { LadderWolrdMatrix->_41,LadderWolrdMatrix->_42,LadderWolrdMatrix->_43,1.f };

			m_pStateMgr->Get_VecState().at(59)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(59)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(59)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(59)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

			m_iState = STATE_LADDER_CLIMB_R_UP_REVERSE_END;
			m_iPhaseState |= PHASE_LADDER;
			//}
		}
	}

	if (!strncmp("P_Ladder02_Up", _pOther->Get_Name(), 13))
	{
		_float4 fLadderLookDir = {};
		_float4 fLadderUpDir = {};
		_float4 fLadderPos = {};
		const _float4x4* LadderWolrdMatrix = _pOther->Get_Transfrom()->Get_WorldMatrix_Ptr();
		fLadderLookDir = { LadderWolrdMatrix->_31,LadderWolrdMatrix->_32,LadderWolrdMatrix->_33,0.f };
		fLadderUpDir = { LadderWolrdMatrix->_21,LadderWolrdMatrix->_22,LadderWolrdMatrix->_23,0.f };
		fLadderPos = { LadderWolrdMatrix->_41,LadderWolrdMatrix->_42,LadderWolrdMatrix->_43,1.f };

		if (m_iState == STATE_LADDER_CLIMB_L_UP)
		{
			m_pStateMgr->Get_VecState().at(60)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(60)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(60)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(60)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
		}

		else if (m_iState == STATE_LADDER_CLIMB_R_UP)
		{
			m_pStateMgr->Get_VecState().at(61)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(61)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(61)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(61)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
		}
	}



	if (!strncmp("P_Ladder02_Down", _pOther->Get_Name(), 15))
	{
		_float4 fLadderLookDir = {};
		_float4 fLadderUpDir = {};
		_float4 fLadderPos = {};

		const _float4x4* LadderWolrdMatrix = _pOther->Get_Transfrom()->Get_WorldMatrix_Ptr();
		fLadderLookDir = { LadderWolrdMatrix->_31,LadderWolrdMatrix->_32,LadderWolrdMatrix->_33,0.f };
		fLadderUpDir = { LadderWolrdMatrix->_21,LadderWolrdMatrix->_22,LadderWolrdMatrix->_23,0.f };
		fLadderPos = { LadderWolrdMatrix->_41,LadderWolrdMatrix->_42,LadderWolrdMatrix->_43,1.f };


		if (m_pGameInstance->isKeyEnter(DIK_E))
		{
			m_pStateMgr->Get_VecState().at(56)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(56)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(56)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(56)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

			m_iState = STATE_LADDER_CLIMB_START;
			m_iPhaseState |= PHASE_LADDER;
		}


		else if (m_iState == STATE_LADDER_CLIMB_L_DOWN)
		{
			m_pStateMgr->Get_VecState().at(57)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(57)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(57)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(57)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
		}

		else if (m_iState == STATE_LADDER_CLIMB_R_DOWN)
		{
			m_pStateMgr->Get_VecState().at(62)->Set_MonsterLookDir(fLadderLookDir);
			m_pStateMgr->Get_VecState().at(62)->Set_MonsterUpDir(fLadderUpDir);
			m_pStateMgr->Get_VecState().at(62)->Set_GetMonsterPos(fLadderPos);
			m_pStateMgr->Get_VecState().at(62)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
		}
	}

}

void CPlayer::Player_Setting_PartAni()
{
#pragma region body State 
	m_set_Body_States =
	{
		STATE_PARRY_DEFLECT_L,
		//STATE_PARRY_L,
		//STATE_PARRY_R,
		STATE_PARRY_DEFLECT_L_UP,
		STATE_PARRY_DEFLECT_R,
		STATE_PARRY_DEFLECT_R_UP,
		STATE_HurtMFR_L,
		STATE_HurtMFR_R,
		STATE_HURT_FALLDOWN,
		STATE_HURT_HURXXLF,
		STATE_HURT_KNOCKBACK,
		STATE_HURT_KNOCKDOWN,
		STATE_HURT_LF,
		STATE_HURT_SF,
		STATE_HURT_SL,
		STATE_NORMAL_EVADE_B,
		STATE_NORMAL_EVADE_BL,
		STATE_NORMAL_EVADE_BR,
		STATE_NORMAL_EVADE_F,
		STATE_NORMAL_EVADE_FL,
		STATE_NORMAL_EVADE_FR,
		STATE_NORMAL_EVADE_L,
		STATE_NORMAL_EVADE_R,
		STATE_LOCK_ON_EVADE_B,
		STATE_LOCK_ON_EVADE_F,
		STATE_LOCK_ON_EVADE_R,
		STATE_LOCK_ON_EVADE_L,
	};
#pragma endregion 
#pragma region Claw State
	m_set_Claw_Weapon_States =
	{
		STATE_ATTACK_LONG_CLAW_01,
		STATE_ATTACK_LONG_CLAW_02,

		/* �÷��̾� ��Ŭ�� ���� ���� */
		STATE_CLAW_CHARGE_START,
		STATE_CLAW_CHARGE_LOOP,
		STATE_CLAW_CHARGE_FULL_ATTACK,
		STATE_CLAW_LONG_PLUNDER_ATTACK2,
	};
#pragma endregion
#pragma region Right Weapon State
	m_set_Right_Weapon_States =
	{
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
		STATE_ATTACK_L4,
		STATE_ATTACK_L5,
		STATE_LIGHT_EXECUTION_L,
		STATE_LIGHT_EXECUTION_R,
		STATE_SPRINT_ATTACK_L1,
		STATE_STUN_EXECUTE_START_VARG,
		STATE_STUN_EXECUTE_START_PUNCHMAN,
		STATE_PUNCH_MAN_Execution,
		STATE_STUN_EXECUTE_START_MAGICIAN,
		STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN,
		STATE_STUN_EXECUTE_START_RESEARCHER,
		STATE_STUN_EXECUTE_START_HARMOR,
		STATE_STUN_EXECUTE_START_JOKER,
		STATE_STUN_EXECUTE_START_GRACE,
	};
#pragma endregion 
#pragma region Player Camera State
	m_set_Player_Camera_States =
	{

		STATE_HARMOR_EXECUTION,
		STATE_LV1Villager_M_Execution,
		STATE_Joker_Execution,
		STATE_Varg_Execution,
		STATE_STUN_EXECUTE ,   // �̰� ó�� ���� ���	
		STATE_CATCHED,
		STATE_VARG_RUN_EXECUTION,
		STATE_GRACE_Execution,
		STATE_MAGICIAN_Execution,
		STATE_MAGICIAN_MUTATION_Execution,
		STATE_MAGICIAN_CATCH,
		STATE_URD_EXECUTION,
		STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START,
		STATE_HURT_MUTATION_MAGICIAN_CATCH,
		STATE_RESEARCHER_EXECUTION,

	};
#pragma endregion 
#pragma region Halberd Weapon State
	m_set_Halberd_Weapon_States =
	{
		STATE_HALBERDS_B,
	};
#pragma endregion 
#pragma region Scythe Weapon State
	m_set_Scythe_Weapon_States =
	{
		STATE_SCYTHE_B,
	};
#pragma endregion 
#pragma region Axe Weapon State
	m_set_Axe_Weapon_States =
	{
		STATE_AXE,
	};
#pragma endregion
#pragma region Cane Weapon State
	m_set_Cane_Weapon_States =
	{
		STATE_CANE_SWORD_SP02,
	};
#pragma endregion 

#pragma region GreadSword Weapon States
	m_set_GreadSword_Weapon_States =
	{
		STATE_GREATSWORD,
	};
#pragma endregion

#pragma region JavelinSword Weapon States
	m_set_JavelinSword_Weapon_States =
	{
		STATE_JAVELIN_SWORD,
	};
#pragma endregion

#pragma region Player Skill State 
	m_set_Player_Skill_State =
	{
		STATE_HALBERDS_B,
		STATE_SCYTHE_B,
		STATE_AXE,
		STATE_CANE_SWORD_SP02,
		STATE_GREATSWORD,
		STATE_JAVELIN_SWORD,
		STATE_CLAW_LONG_PLUNDER_ATTACK2,
	};
#pragma endregion 
}

void CPlayer::Free()
{

	m_pGameInstance->Delete_Dynamic_Light(m_pTransformCom);

	__super::Free();

	m_pGameInstance->Sub_Actor_Scene(m_pActor);

	Safe_Release(m_pStateMgr);
	Safe_Release(m_pPlayerSkillMgr);
	Safe_Release(m_pNavigationCom);
}
