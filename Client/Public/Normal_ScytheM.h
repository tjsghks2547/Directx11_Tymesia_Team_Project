#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)

class CNormal_ScytheM final : public CMonster
{
private:
	CNormal_ScytheM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_ScytheM(const CNormal_ScytheM& Prototype);
	virtual ~CNormal_ScytheM() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
public:
	virtual void State_Update(_float fTimeDelta) override;
	virtual void PatternCreate() override;
	virtual void Active() override;
	virtual void Return_To_Spawn() override;
	virtual void Stun() override;
private:
	CState_Machine<CNormal_ScytheM>* m_pState_Manager = { nullptr };
private:
	_bool m_bCanHit = true;
	_bool m_bExecution_Start = {};
	_bool m_bFirstActive = {};
	_uint m_iHit_Motion_Index = -1;
	_uint m_iParryReadyHits = {};

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CNormal_ScytheM* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//ป๓ลย
	class Intro_State : public CStates<CNormal_ScytheM>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Idle_State : public CStates<CNormal_ScytheM>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Move_State : public CStates<CNormal_ScytheM>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Run_State : public CStates<CNormal_ScytheM>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	private:
		_bool bCheck = {};
		_bool bTest = {};
		CNavigation* m_pPlayerNavi = { nullptr };
	};

	class Stun_State : public CStates<CNormal_ScytheM>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Hit_State : public CStates<CNormal_ScytheM>
	{
	public:
		Hit_State(_uint pHitNum);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	private:
		_uint m_iHitNum = -1;
	};

	class Execution_State : public CStates<CNormal_ScytheM>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Return_To_SpawnPoint_State : public CStates<CNormal_ScytheM>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	private:
		_bool bCheck = {};
	};

	class NotActive_Idle : public CStates<CNormal_ScytheM>
	{
	public:
		NotActive_Idle() = default;
		virtual ~NotActive_Idle() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Parry_State : public CStates<CNormal_ScytheM>
	{
	public:
		Parry_State() = default;
		virtual ~Parry_State() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;

	};
	class Attack_ComboA : public CStates<CNormal_ScytheM>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Attack_ComboB : public CStates<CNormal_ScytheM>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

	class Attack_ComboC : public CStates<CNormal_ScytheM>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		void State_Enter(CNormal_ScytheM* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_ScytheM* pObject) override;
		void State_Exit(CNormal_ScytheM* pObject) override;
	};

};

END