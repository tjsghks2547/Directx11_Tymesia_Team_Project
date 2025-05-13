#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;

END

BEGIN(Client)

class CElite_Joker final : public CMonster
{
private:
	CElite_Joker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Joker(const CElite_Joker& Prototype);
	virtual ~CElite_Joker() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void PatternCreate() override;
	void Active() override;
	void Return_To_Spawn() override;
	void Stun() override;

public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);

	void State_Update(_float fTimeDelta);

public:
	void Near_Pattern_Create();
	void Far_Pattern_Create();
private:
	_bool                            m_bExecution_Progress = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iSpawn_Cell_Index = {};

private:
	CState_Machine<CElite_Joker>* m_pState_Manager = { nullptr };

private:
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CElite_Joker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


	class Idle_State : public CStates<CElite_Joker>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Intro_State : public CStates<CElite_Joker>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Walk_State : public CStates<CElite_Joker>
	{
	public:
		Walk_State() = default;
		virtual ~Walk_State() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Attack_Combo_A : public CStates<CElite_Joker>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_bool m_bBonusAttack = {};
	};

	class Attack_Combo_B : public CStates<CElite_Joker>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_bool m_bBonusAttack = {};
	};

	class Attack_Run : public CStates<CElite_Joker>
	{
	public:
		Attack_Run() = default;
		virtual ~Attack_Run() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_float   m_fTimer = {};
	};

	class Attack_Wheel : public CStates<CElite_Joker>
	{
	public:
		Attack_Wheel() = default;
		virtual ~Attack_Wheel() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_float   m_fTimer = {};
	};

	class Attack_Shock : public CStates<CElite_Joker>
	{
	public:
		Attack_Shock() = default;
		virtual ~Attack_Shock() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Attack_Strong : public CStates<CElite_Joker>
	{
	public:
		Attack_Strong() = default;
		virtual ~Attack_Strong() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Attack_Jump : public CStates<CElite_Joker>
	{
	public:
		Attack_Jump() = default;
		virtual ~Attack_Jump() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};

	class Stun_State : public CStates<CElite_Joker>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_float   m_fTimer = {};
	};

	class Execution_State : public CStates<CElite_Joker>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	};
	class Return_To_SpawnPoint_State : public CStates<CElite_Joker>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CElite_Joker* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Joker* pObject) override;
		void State_Exit(CElite_Joker* pObject) override;
	private:
		_bool m_bCheck = {};
	};
};

END