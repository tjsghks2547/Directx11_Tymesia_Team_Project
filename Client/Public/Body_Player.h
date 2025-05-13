#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Player.h"	

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
class CTexture;
END

class CStateMgr;

BEGIN(Client)


class CCamera_Free;

class CBody_Player final : public CPartObject
{

	enum RENDER_STATE
	{
		STATE_NORMAL_RENDER,  /*   ���� ���� ����       */
		STATE_CLAW_RENDER,    /*   ���� ���ݽ��� ����     */
		STATE_DEAD_RENDER,    /*   ���� ���� �Ͻ�         */

	};

public:
	struct BODY_PLAYER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* pParentState = { nullptr };
		_uint* pPreParentState = { nullptr };
		_uint* pParentPhaseState = { nullptr };
		_uint* pParentExectueMonsterState = { nullptr };

		_bool* pParentNextStateCan = { nullptr };
		CStateMgr* pParentStateMgr = { nullptr };
		CNavigation* pParentNavigationCom = { nullptr };
		CGameObject* pParent = { nullptr };
		PxRigidDynamic* pParentActor = { nullptr };

		_int* pParentHp = { nullptr };
		_int* pParentMp = { nullptr };

		_uint* pParentTalent = { nullptr };
	};

private:
	CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Player(const CBody_Player& Prototype);
	virtual ~CBody_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;


public:
	/* �� ��ǿ� ���� ������ ���� */
	HRESULT    STATE_NORMAL_Render();
	HRESULT    STATE_ATTACK_LONG_CLAW_Render();
	HRESULT    STATE_DEAD_Render();
	/* ============================ */

public:
	/* �� ��ǿ� ���� �ִϸ��̼� ���� */
	void	STATE_IDLE_Method();
	void	STATE_RUN_Method();
	void	STATE_WALK_Method();
	void	STATE_ATTACK_Method();
	void	STATE_ATTACK_L1_Method();
	void	STATE_ATTACK_L2_Method();
	void	STATE_ATTACK_L3_Method();
	void	STATE_ATTACK_L4_Method();
	void	STATE_ATTACK_L5_Method();
	void	STATE_ATTACK_LONG_CLAW_01_Method();
	void	STATE_ATTACK_LONG_CLAW_02_Method();
	/*���� ���� ��� */
	void    STATE_LOCK_ON_RUN_B_Method();
	void    STATE_LOCK_ON_RUN_BL_Method();
	void    STATE_LOCK_ON_RUN_BR_Method();
	void    STATE_LOCK_ON_RUN_FL_Method();
	void    STATE_LOCK_ON_RUN_FR_Method();
	void    STATE_LOCK_ON_RUN_L_Method();
	void    STATE_LOCK_ON_RUN_R_Method();

	void    STATE_LOCK_ON_EVADE_F_Method();
	void    STATE_LOCK_ON_EVADE_B_Method();
	void    STATE_LOCK_ON_EVADE_L_Method();
	void    STATE_LOCK_ON_EVADE_R_Method();
	/* =========================================== */

	/* �и� ���� ��� */
	void    STATE_PARRY_L_Method();
	void    STATE_PARRY_R_Method();


	void    STATE_PARRY_DEFLECT_LARGE_Method();
	void	STATE_PARRY_DEFLECT_L_UP_Method();
	void	STATE_PARRY_DEFLECT_L_Method();

	void 	STATE_PARRY_DEFLECT_R_UP_Method();
	void 	STATE_PARRY_DEFLECT_R_Method();
	/* ============================================ */



	/* �ǰ� ���� */
	void   STATE_HurtMFR_L_Method();
	void   STATE_HurtMFR_R_Method();
	void   STATE_HURT_LF_Method();
	void   STATE_HURT_SF_Method();
	void   STATE_HURT_SL_Method();
	void   STATE_HURT_HURXXLF_Method();
	void   STATE_HURT_KNOCKBACK_Method();
	void   STATE_HURT_KNOCKDOWN_Method();
	void   STATE_HURT_FALLDOWN_Method();
	void   STATE_HURT_FALLDOWN_END_Method();
	void   STATE_WEAK_GETUP_F_Method();
	void   STATE_REBOUND_R_Method();
	void   STATE_STUNNED_START_Method();
	void   STATE_STUNNED_LOOP_Method();
	/*-----------------*/


	/* ȸ�� ���� */
	void STATE_NORMAL_EVADE_R_Method();
	void STATE_NORMAL_EVADE_L_Method();
	void STATE_NORMAL_EVADE_FR_Method();
	void STATE_NORMAL_EVADE_FL_Method();
	void STATE_NORMAL_EVADE_F_Method();
	void STATE_NORMAL_EVADE_BR_Method();
	void STATE_NORMAL_EVADE_BL_Method();
	void STATE_NORMAL_EVADE_B_Method();
	/* ------------------------- */

	/* ó�� ��� */
	void STATE_STUN_EXECUTE_START_HARMOR_Method();
	void STATE_HARMOR_EXECUTION_Method();
	void STATE_STUN_EXECUTE_Method();
	void STATE_STUN_EXECUTE_START_VILLAGEM1_Method();
	void STATE_LV1Villager_M_Execution_Method();
	void STATE_STUN_EXECUTE_START_JOKER_Method();
	void STATE_Joker_Execution_Method();
	void STATE_Varg_Execution_Method();
	void STATE_STUN_EXECUTE_START_URD_Method();
	void STATE_URD_EXECUTION_Method();
	void STATE_STUN_EXECUTE_START_BAT_Method();
	void STATE_BAT_EXECUTION_Method();
	void STATE_STUN_EXECUTE_START_RESEARCHER_Method();
	void STATE_RESEARCHER_EXECUTION_Method();
	/* ------------------------------- */

	/* ���� ���� ��� */
	void STATE_ARCHIVE_SIT_START_Method();
	void STATE_ARCHIVE_SIT_LOOP_Method();
	void STATE_ARCHIVE_SIT_GETUP_Method();
	void STATE_ARCHIVE_SIT_LIGHT_UP_Method();

	/* �� ���� ��� */
	void STATE_HEAL_Method();

	/* ���� ���� ��� */
	void STATE_DEAD_Method();

	/* ���� ���� �� ������ ���� ���*/
	void STATE_START_WALK_Method();

	/* �÷��̾� ��Ŭ�� ���� ���� */
	void STATE_CLAW_CHARGE_START_Method();
	void STATE_CLAW_CHARGE_LOOP_Method();
	void STATE_CLAW_CHARGE_FULL_ATTACK_Method();
	void STATE_CLAW_LONG_PLUNDER_ATTACK2_Method();


	/* ��ų ���� */
	void STATE_HALBERDS_B_Method();
	void STATE_SCYTHE_B_Method();
	void STATE_AXE_Method();
	void STATE_CANE_SWORD_SP02_Method();	/* �÷��̾� ������ ��ų */
	void STATE_GREATSWORD_Method();			/* �÷��̾� ��� ��ų */
	void STATE_JAVELIN_SWORD_Method();		/* �÷��̾� �ں��� ��ų */

	/* �÷��̾� ������ ��� */
	void STATE_CATCHED_Method();

	/* �÷��̾� ������ �Ͼ�� ���*/
	void STATE_GET_UP_Method();

	/* �ٱ׿��� �پ�� ó���ϴ� ��� */
	void STATE_VARG_RUN_EXECUTION_Method();

	/* �븻 ���� ó��*/
	void STATE_LIGHT_EXECUTION_R_Method();
	void STATE_LIGHT_EXECUTION_L_Method();


	/* ��ٸ� �̵� ���� */
	void STATE_LADDER_CLIMB_START_Method();

	void STATE_LADDER_CLIMB_L_DOWN_Method();
	void STATE_LADDER_CLIMB_L_DOWN_END_Method();
	void STATE_LADDER_CLIMB_L_UP_Method();
	void STATE_LADDER_CLIMB_L_UP_END_Method();

	void STATE_LADDER_CLIMB_L_IDEL_Method();

	void STATE_LADDER_CLIMB_R_DOWN_Method();
	void STATE_LADDER_CLIMB_R_DOWN_END_Method();
	void STATE_LADDER_CLIMB_R_UP_Method();
	void STATE_LADDER_CLIMB_R_UP_END_Method();
	void STATE_LADDER_CLIMB_R_UP_REVERSE_END_Method();

	void STATE_LADDER_CLIMB_R_IDEL_Method();

	/* ������ ���� ù��° �ƽ� */
	void STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START_Method();

	/* (���θ�) ������ LV_1 ó�� �ƽ� */
	void STATE_MAGICIAN_Execution_Method();
	void STATE_STUN_EXECUTE_START_MAGICIAN_Method();

	/* (���̵� ���θ�) ó�� �ƽ� */
	void STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN_Method();
	void STATE_MAGICIAN_MUTATION_Execution_Method();

	/* �׷��̽� ó�� �ƽ� */
	void STATE_STUN_EXECUTE_START_GRACE_Method();
	void STATE_GRACE_Execution_Method();

	/* ��ġ�� ó�� �ƽ� */
	void STATE_STUN_EXECUTE_START_PUNCHMAN_Method();
	void STATE_PUNCH_MAN_Execution_Method();

	/* ���θ� ��ų�� �¾Ƽ� ������ ��� */
	void STATE_MAGICIAN_CATCH_Method();

	/* �÷��̾� ������Ʈ */
	void STATE_SPRINT_Method();

	/* �÷��̾� ������Ʈ ���� */
	void STATE_SPRINT_ATTACK_L1_Method();

	/* �÷��̾� ö������ ��� �κ� IDLE ���� */
	void STATE_LOBBY_IDLE_01_Method();
	void STATE_LOBBY_IDLE_01_END_Method();

	/* �ٱ� ó�� ó�� ��� ���� */
	void STATE_STUN_EXECUTE_START_VARG_Method();

	/* ���� ���θ����� ���� �� */
	void STATE_HURT_MUTATION_MAGICIAN_CATCH_Method();

	/* ����ó���� ������ �� */
	void STATE_HURT_RESEARCHER_CATCHED_Method();




private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDissolveNoiseTextureCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	CGameObject* m_pParent = { nullptr };

	/* ORM �ؽ�ó ���� */
	CTexture* m_pORM_HeadTexture = { nullptr };
	CTexture* m_pORM_BodyTexture = { nullptr };
	CTexture* m_pORM_LegTexture = { nullptr };
	CTexture* m_pORM_ClockTexture = { nullptr };

	_float							m_fAnimSpeed = 1.f;
	_float						    m_fTimeDelta = {};
	_float							m_fZoomBlurDeltaTime = {};

	_float							m_fHitStopTime = {};
	_float4* m_fRespawnPosPtr = {};
private:

	_uint* m_pParentMonsterExecute = { nullptr };
	_uint* m_pParentState = { nullptr };
	_uint* m_pPreParentState = { nullptr };
	_uint* m_pParentPhsaeState = { nullptr };
	_bool* m_pParentNextStateCan = { nullptr };
	_bool* m_pParentSkillCoolTime = { nullptr };
	_uint  m_iRenderState = {};
	_uint* m_pParentTalent = { nullptr };

	_int* m_pParentHp = { nullptr };
	_int* m_pParentMp = { nullptr };

	CStateMgr* m_pParentStateMgr = { nullptr };
	CNavigation* m_pParentNavigationCom = { nullptr };
	PxRigidDynamic* m_pParentActor = { nullptr };


	_bool  m_bParryStopOnOff = { true };

	_float m_fDissolveAmount = {};
	_float m_fDeadTimer = {};
	_float m_fFinishTime = {};
	_float m_fDeadStartTimer = {};

	/* ���� ������ ���� */
	_float* m_pfClawParentTime = { };
	_float* m_pfClawFinishTime = {};
	_float* m_pfClawAppearTimer = {};
	_bool* m_pbClawDeadOn = {};
	_bool* m_pbClawAppear = {};


	_uint m_iCurrentLevel = {}; //���� �߰� Level��ȯ������

	unordered_set<CPlayer::STATE>* m_pSet_Body_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Claw_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Halberd_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Right_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Scythe_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Axe_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_GreadSword_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_JavelinSword_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Player_Camera_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Cane_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Player_Skill_State = { nullptr };

	_bool  m_bFirstCameraPos = { true };

	/* �߼Ҹ� ���� */
	//_bool  m_RightFootStepSound = { true };
	//_bool  m_LeftFootStepSound = { true };

	_uint  m_iRightFootStepSound = { 0 };
	_uint  m_iLeftFootStepSound = { 0 };

	/* ��ٸ� �Ҹ� ���� */
	_uint  m_iLadderStepSound = {};


	const _float4x4* m_mRightFootBoneMartix = { nullptr };
	const _float4x4* m_mLeftFootBoneMartix = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END