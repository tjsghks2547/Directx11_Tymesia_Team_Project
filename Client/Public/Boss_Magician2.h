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

class CBoss_Magician2 final : public CMonster
{
private:
	CBoss_Magician2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Magician2(const CBoss_Magician2& Prototype);
	virtual ~CBoss_Magician2() = default;
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
	_bool                            m_bSpecial_Skill_Progress = {};
	_bool                            m_bCatch_Special_Attack = {};
	_bool							 m_bIntro_Change_Socket = { true };
	_bool							 m_bIntro_Mutation_Active = {};

	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fSlash_Skill_CoolTime = {};


	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = {};
private:
	CState_Machine<CBoss_Magician2>* m_pState_Manager = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Magician2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


	class Intro_State : public CStates<CBoss_Magician2>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Magician2>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Run_State : public CStates<CBoss_Magician2>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Stun_State : public CStates<CBoss_Magician2>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Magician2>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboA : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboB : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	private:
		_bool m_bAttack_V2 = {};
	};

	class Attack_ComboC : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboD : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboD() = default;
		virtual ~Attack_ComboD() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboE : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboE() = default;
		virtual ~Attack_ComboE() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboF : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboF() = default;
		virtual ~Attack_ComboF() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboG : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboG() = default;
		virtual ~Attack_ComboG() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboH : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboH() = default;
		virtual ~Attack_ComboH() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboI : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboI() = default;
		virtual ~Attack_ComboI() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_ComboJ : public CStates<CBoss_Magician2>
	{
	public:
		Attack_ComboJ() = default;
		virtual ~Attack_ComboJ() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Attack_Special : public CStates<CBoss_Magician2>
	{
	public:
		Attack_Special() = default;
		virtual ~Attack_Special() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	};

	class Catch_State : public CStates<CBoss_Magician2>
	{
	public:
		Catch_State() = default;
		virtual ~Catch_State() = default;
	public:
		void State_Enter(CBoss_Magician2* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician2* pObject) override;
		void State_Exit(CBoss_Magician2* pObject) override;
	private:
		_bool m_bNeed_Move = {};
		_bool m_bAttack = {};
	};

};

END