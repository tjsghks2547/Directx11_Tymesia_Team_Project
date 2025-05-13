#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;

END

BEGIN(Client)

class CElite_Punch_Man final : public CMonster
{
private:
	CElite_Punch_Man(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Punch_Man(const CElite_Punch_Man& Prototype);
	virtual ~CElite_Punch_Man() = default;
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
	virtual void Return_To_Spawn() override;
	virtual void Stun() override;

public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
public:
	void Near_Pattern_Create();
	void Far_Pattern_Create();
private:
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bExecution_Start = {};
	_bool                            m_bFirstActive = {};

	_uint                            m_iNearPatternIndex = {};
	_uint							 m_iSpawn_Cell_Index = {};

private:
	CState_Machine<CElite_Punch_Man>* m_pState_Manager = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CElite_Punch_Man* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//»óÅÂµé
	class Intro_State : public CStates<CElite_Punch_Man>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	};

	class Idle_State : public CStates<CElite_Punch_Man>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	};

	class Move_State : public CStates<CElite_Punch_Man>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	};

	class Stun_State : public CStates<CElite_Punch_Man>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Execution_State : public CStates<CElite_Punch_Man>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	};

	class Return_To_SpawnPoint_State : public CStates<CElite_Punch_Man>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Attack_ComboA : public CStates<CElite_Punch_Man>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	private:
		_bool m_bBonus_Attack = {};
	};

	class Attack_ComboB : public CStates<CElite_Punch_Man>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	};

	class Attack_ComboC : public CStates<CElite_Punch_Man>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		void State_Enter(CElite_Punch_Man* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Punch_Man* pObject) override;
		void State_Exit(CElite_Punch_Man* pObject) override;
	private:
		_bool m_bLoop = true;
		_uint m_iLoopCount = {};
	};

};

END