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
		STATE_NORMAL_RENDER,  /*   평상시 공격 상태       */
		STATE_CLAW_RENDER,    /*   발톱 공격시의 상태     */
		STATE_DEAD_RENDER,    /*   죽음 상태 일시         */

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
	/* 각 모션에 따른 렌더링 관련 */
	HRESULT    STATE_NORMAL_Render();
	HRESULT    STATE_ATTACK_LONG_CLAW_Render();
	HRESULT    STATE_DEAD_Render();
	/* ============================ */

public:
	/* 각 모션에 따른 애니메이션 관련 */
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
	/*락온 관련 기능 */
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

	/* 패링 관련 기능 */
	void    STATE_PARRY_L_Method();
	void    STATE_PARRY_R_Method();


	void    STATE_PARRY_DEFLECT_LARGE_Method();
	void	STATE_PARRY_DEFLECT_L_UP_Method();
	void	STATE_PARRY_DEFLECT_L_Method();

	void 	STATE_PARRY_DEFLECT_R_UP_Method();
	void 	STATE_PARRY_DEFLECT_R_Method();
	/* ============================================ */



	/* 피격 관련 */
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


	/* 회피 관련 */
	void STATE_NORMAL_EVADE_R_Method();
	void STATE_NORMAL_EVADE_L_Method();
	void STATE_NORMAL_EVADE_FR_Method();
	void STATE_NORMAL_EVADE_FL_Method();
	void STATE_NORMAL_EVADE_F_Method();
	void STATE_NORMAL_EVADE_BR_Method();
	void STATE_NORMAL_EVADE_BL_Method();
	void STATE_NORMAL_EVADE_B_Method();
	/* ------------------------- */

	/* 처형 모션 */
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

	/* 의자 관련 모션 */
	void STATE_ARCHIVE_SIT_START_Method();
	void STATE_ARCHIVE_SIT_LOOP_Method();
	void STATE_ARCHIVE_SIT_GETUP_Method();
	void STATE_ARCHIVE_SIT_LIGHT_UP_Method();

	/* 힐 관련 모션 */
	void STATE_HEAL_Method();

	/* 죽음 관련 모션 */
	void STATE_DEAD_Method();

	/* 게임 시작 및 리스폰 시작 모션*/
	void STATE_START_WALK_Method();

	/* 플레이어 우클릭 차지 공격 */
	void STATE_CLAW_CHARGE_START_Method();
	void STATE_CLAW_CHARGE_LOOP_Method();
	void STATE_CLAW_CHARGE_FULL_ATTACK_Method();
	void STATE_CLAW_LONG_PLUNDER_ATTACK2_Method();


	/* 스킬 관련 */
	void STATE_HALBERDS_B_Method();
	void STATE_SCYTHE_B_Method();
	void STATE_AXE_Method();
	void STATE_CANE_SWORD_SP02_Method();	/* 플레이어 지팡이 스킬 */
	void STATE_GREATSWORD_Method();			/* 플레이어 대검 스킬 */
	void STATE_JAVELIN_SWORD_Method();		/* 플레이어 자벨린 스킬 */

	/* 플레이어 잡히는 모션 */
	void STATE_CATCHED_Method();

	/* 플레이어 잡히고 일어서는 모션*/
	void STATE_GET_UP_Method();

	/* 바그에게 뛰어가서 처형하는 모션 */
	void STATE_VARG_RUN_EXECUTION_Method();

	/* 노말 몬스터 처형*/
	void STATE_LIGHT_EXECUTION_R_Method();
	void STATE_LIGHT_EXECUTION_L_Method();


	/* 사다리 이동 관련 */
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

	/* 매지션 보스 첫번째 컷신 */
	void STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START_Method();

	/* (오두르) 매지션 LV_1 처형 컷신 */
	void STATE_MAGICIAN_Execution_Method();
	void STATE_STUN_EXECUTE_START_MAGICIAN_Method();

	/* (변이된 오두르) 처형 컷신 */
	void STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN_Method();
	void STATE_MAGICIAN_MUTATION_Execution_Method();

	/* 그레이스 처형 컷신 */
	void STATE_STUN_EXECUTE_START_GRACE_Method();
	void STATE_GRACE_Execution_Method();

	/* 펀치맨 처형 컷신 */
	void STATE_STUN_EXECUTE_START_PUNCHMAN_Method();
	void STATE_PUNCH_MAN_Execution_Method();

	/* 오두르 스킬에 맞아서 잡히는 모션 */
	void STATE_MAGICIAN_CATCH_Method();

	/* 플레이어 스프린트 */
	void STATE_SPRINT_Method();

	/* 플레이어 스프린트 어택 */
	void STATE_SPRINT_ATTACK_L1_Method();

	/* 플레이어 철학자의 언덕 로비 IDLE 상태 */
	void STATE_LOBBY_IDLE_01_Method();
	void STATE_LOBBY_IDLE_01_END_Method();

	/* 바그 처음 처형 모션 시작 */
	void STATE_STUN_EXECUTE_START_VARG_Method();

	/* 변이 오두르에게 당할 때 */
	void STATE_HURT_MUTATION_MAGICIAN_CATCH_Method();

	/* 리서처에게 잡혔을 때 */
	void STATE_HURT_RESEARCHER_CATCHED_Method();




private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pDissolveNoiseTextureCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	CGameObject* m_pParent = { nullptr };

	/* ORM 텍스처 관련 */
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

	/* 손톱 디졸브 관련 */
	_float* m_pfClawParentTime = { };
	_float* m_pfClawFinishTime = {};
	_float* m_pfClawAppearTimer = {};
	_bool* m_pbClawDeadOn = {};
	_bool* m_pbClawAppear = {};


	_uint m_iCurrentLevel = {}; //종한 추가 Level전환때문에

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

	/* 발소리 관련 */
	//_bool  m_RightFootStepSound = { true };
	//_bool  m_LeftFootStepSound = { true };

	_uint  m_iRightFootStepSound = { 0 };
	_uint  m_iLeftFootStepSound = { 0 };

	/* 사다리 소리 관련 */
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