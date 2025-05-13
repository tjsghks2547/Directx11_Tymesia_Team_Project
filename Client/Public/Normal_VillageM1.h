#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;

END

BEGIN(Client)

class CNormal_VillageM1 final : public CMonster
{
private:
	CNormal_VillageM1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_VillageM1(const CNormal_VillageM1& Prototype);
	virtual ~CNormal_VillageM1() = default;
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

private:
	CState_Machine<CNormal_VillageM1>* m_pState_Manager = { nullptr };
private:
	_bool m_bCanHit = {};
	_uint m_iParryReadyHits = {};

	_uint m_iHit_Motion_Index = {};
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CNormal_VillageM1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//상태
	class Idle_State : public CStates<CNormal_VillageM1>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Move_State : public CStates<CNormal_VillageM1>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Run_State : public CStates<CNormal_VillageM1>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_bool bCheck = {};
		CNavigation* m_pPlayerNavi = { nullptr };
	};

	class Stun_State : public CStates<CNormal_VillageM1>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Attack_01_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_01_State() = default;
		virtual ~Attack_01_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};
	class Attack_02_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_02_State() = default;
		virtual ~Attack_02_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Attack_03_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_03_State() = default;
		virtual ~Attack_03_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Parry_State : public CStates<CNormal_VillageM1>
	{
	public:
		Parry_State() = default;
		virtual ~Parry_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Parry_Attack_State : public CStates<CNormal_VillageM1>
	{
	public:
		Parry_Attack_State() = default;
		virtual ~Parry_Attack_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Hit_State : public CStates<CNormal_VillageM1>
	{
	public:
		Hit_State(_uint pHitNum);
		virtual ~Hit_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_uint m_iHitNum = -1;
	};

	class Execution_State : public CStates<CNormal_VillageM1>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Return_To_SpawnPoint_State : public CStates<CNormal_VillageM1>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_bool bCheck = {};
	};
};

END