#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)

class CBoss_Varg final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
private:
	CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Varg(const CBoss_Varg& Prototype);
	virtual ~CBoss_Varg() = default;
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
	void Far_Pattern_Create();
private:
	_bool                            m_bExecution_Progress = {};
	_bool                            m_Is_Catch = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = {};

	_float                           m_fSpecial_Skill_CoolTime = {};
	_float							 m_fBlurStrength = {};

private:
	CState_Machine<CBoss_Varg>* m_pState_Manager = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Varg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


	//상태 클래스 추가부분
	class Stun_State : public CStates<CBoss_Varg>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Intro_State : public CStates<CBoss_Varg>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Varg>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_A : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_B : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_C : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_D : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_E : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_E() = default;
		virtual ~Attack_Combo_E() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Avoid_State : public CStates<CBoss_Varg>
	{
	public:
		Avoid_State() = default;
		virtual ~Avoid_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_bool m_bBonusAttack = false;
	};

	class Hit_State : public CStates<CBoss_Varg>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Walk_State : public CStates<CBoss_Varg>
	{
	public:
		Walk_State() = default;
		virtual ~Walk_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Run_State : public CStates<CBoss_Varg>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_float m_fTimer = {};
	};

	class Raid_Attack_01 : public CStates<CBoss_Varg>
	{
	public:
		Raid_Attack_01() = default;
		virtual ~Raid_Attack_01() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Raid_Attack_02 : public CStates<CBoss_Varg>
	{
	public:
		Raid_Attack_02() = default;
		virtual ~Raid_Attack_02() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class ExeCution_Start_State : public CStates<CBoss_Varg>
	{
	public:
		ExeCution_Start_State() = default;
		virtual ~ExeCution_Start_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Varg>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_bool m_bOne_More_Look = {};
		_bool m_bNeed_Look_Player = {};
	};

	class Roar_State : public CStates<CBoss_Varg>
	{
	public:
		Roar_State(_bool pCheck);
		virtual ~Roar_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_bool  m_bFirst = {};
		_float m_fRoarBlurStrength = {};
	};

	class Catch_State : public CStates<CBoss_Varg>
	{
	public:
		Catch_State() = default;
		virtual ~Catch_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_float m_fTimer = {};
		_bool  m_bFirst = true;
		_bool  m_bCanCatch = {};
		_bool  m_bAttack = {};
	};

	class Dead_State : public CStates<CBoss_Varg>
	{
	public:
		Dead_State() = default;
		virtual ~Dead_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

};

END