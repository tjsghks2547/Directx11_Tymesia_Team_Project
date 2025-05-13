#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)

class CElite_Grace final : public CMonster
{
private:
	CElite_Grace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Grace(const CElite_Grace& Prototype);
	virtual ~CElite_Grace() = default;
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
	void Shoot_Calculate_Distance(_vector& vMyPos, _vector& vPlayerPos);

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
	_bool						     m_bCanHit = true;

	_float                           m_fDagger_Delete_Time = {};

	_uint                            m_iNearPatternIndex = {};
	_uint                            m_iFarPatternIndex = {};
	_uint							 m_iSpawn_Cell_Index = {};
	_uint					         m_iHit_Motion_Index = {};
	_uint						     m_iParryReadyHits = {};
private:
	CState_Machine<CElite_Grace>* m_pState_Manager = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CElite_Grace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//»óÅÂµé
	class Idle_State : public CStates<CElite_Grace>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Move_State : public CStates<CElite_Grace>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Run_State : public CStates<CElite_Grace>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool bCheck = {};
		CNavigation* m_pPlayerNavi = { nullptr };
	};

	class Hit_State : public CStates<CElite_Grace>
	{
	public:
		Hit_State(_uint pHitNum);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_uint m_iHitNum = -1;
	};

	class Stun_State : public CStates<CElite_Grace>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Execution_State : public CStates<CElite_Grace>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Return_To_SpawnPoint_State : public CStates<CElite_Grace>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Attack_ComboA : public CStates<CElite_Grace>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Attack_ComboB : public CStates<CElite_Grace>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Attack_ComboC : public CStates<CElite_Grace>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool m_bIs_Fired = {};
		_float4 m_vStartPos = {};
		_float4 m_vEndPos = {};
	};

	class Parry_State : public CStates<CElite_Grace>
	{
	public:
		Parry_State() = default;
		virtual ~Parry_State() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Parry_Attack_A : public CStates<CElite_Grace>
	{
	public:
		Parry_Attack_A() = default;
		virtual ~Parry_Attack_A() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	};

	class Parry_Attack_B : public CStates<CElite_Grace>
	{
	public:
		Parry_Attack_B() = default;
		virtual ~Parry_Attack_B() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool m_bIs_Fired = {};
	};

	class Shoot_Attack_A : public CStates<CElite_Grace>
	{
	public:
		Shoot_Attack_A() = default;
		virtual ~Shoot_Attack_A() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool m_bIs_Fired = {};
		_float4 m_vStartPos = {};
		_float4 m_vEndPos = {};
	};

	class Shoot_Attack_B : public CStates<CElite_Grace>
	{
	public:
		Shoot_Attack_B() = default;
		virtual ~Shoot_Attack_B() = default;
	public:
		void State_Enter(CElite_Grace* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Grace* pObject) override;
		void State_Exit(CElite_Grace* pObject) override;
	private:
		_bool m_bIs_Fired = {};
		_float4 m_vStartPos = {};
		_float4 m_vEndPos = {};
	};
};

END