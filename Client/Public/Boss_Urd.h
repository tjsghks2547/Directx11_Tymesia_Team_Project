#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CShader;
class CNavigation;
END

BEGIN(Client)

class CBoss_Urd final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
	enum SWORD_STACK { STACK_ONE, STACK_TWO, STACK_THREE, STACK_END };
private:
	CBoss_Urd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Urd(const CBoss_Urd& Prototype);
	virtual ~CBoss_Urd() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual void State_Update(_float fTimeDelta) override;
	virtual void PatternCreate() override;
	virtual void Active() override;
	virtual void Stun() override;
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);

public:
	void Near_Pattern_Create();
	void Stack_Skill_Create();
private:
	_bool                            m_bNeed_Memory_Position[STACK_END] = {};
	_bool							 m_bIs_Equipped_To_LeftHand[STACK_END] = {};
	_bool							 m_bIs_Stand_In_Ground[STACK_END] = {};
	_bool							 m_bStand_Stack_Sword_ColliderOn = {};


private:
	_bool							 m_bPattern70 = {};
	_bool							 m_bPattern50 = {};
	_bool							 m_bPattern30 = {};
private:
	_bool						     m_bCan_Hit_Motion = {};
	_bool							 m_bChange_Socket = {};
	_bool							 m_bRender_Left_Sword = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bSpecial_Skill_Progress = {};

	_bool                            m_bCatch_Special_Attack = {};
	_bool							 m_bNeed_Decide_Step_Num = { true };

	_float                           m_fSpecial_Skill_CoolTime = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint							 m_iHit_Motion_Index = -1;
	_uint							 m_iStep_Count = {};
	_uint							 m_iSword_Stack_Count = {};
	_uint                            m_iCheck_Step_Num = {};
	_uint                            m_iParryReadyHits = {};

	_uint                            m_iPhase = { PHASE_END };


private:
	CState_Machine<CBoss_Urd>* m_pState_Manager = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Urd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	class Intro_State : public CStates<CBoss_Urd>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Urd>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};


	class Hit_State : public CStates<CBoss_Urd>
	{
	public:
		Hit_State(_uint iHit_Index);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_uint m_iHit_Index = {};
	};

	class Dead_State : public CStates<CBoss_Urd>
	{
	public:
		Dead_State() = default;
		virtual ~Dead_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Parry_State : public CStates<CBoss_Urd>
	{
	public:
		Parry_State() = default;
		virtual ~Parry_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_uint m_iParry_Index = {};
	};

	class Step_Front_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Front_State() = default;
		virtual ~Step_Front_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Step_Back_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Back_State() = default;
		virtual ~Step_Back_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Step_Right_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Right_State() = default;
		virtual ~Step_Right_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Step_Left_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Left_State() = default;
		virtual ~Step_Left_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Stun_State : public CStates<CBoss_Urd>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Urd>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_A : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_B : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_C : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_D : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};
	class Attack_Combo_E : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_E() = default;
		virtual ~Attack_Combo_E() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};


	class Attack_Combo_F : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_F() = default;
		virtual ~Attack_Combo_F() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};


	class Attack_Stack_Skill_01 : public CStates<CBoss_Urd>
	{
	public:
		Attack_Stack_Skill_01() = default;
		virtual ~Attack_Stack_Skill_01() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_bool m_bIsSpawn = {};
	};

	class Attack_Stack_Skill_02 : public CStates<CBoss_Urd>
	{
	public:
		Attack_Stack_Skill_02() = default;
		virtual ~Attack_Stack_Skill_02() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_bool m_bIsSpawn = {};
	};

	class Attack_Special_Skill : public CStates<CBoss_Urd>
	{
	public:
		Attack_Special_Skill() = default;
		virtual ~Attack_Special_Skill() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};
};

END