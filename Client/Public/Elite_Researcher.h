#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)

class CElite_Researcher final : public CMonster
{
private:
	CElite_Researcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElite_Researcher(const CElite_Researcher& Prototype);
	virtual ~CElite_Researcher() = default;
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
	_bool                            m_bBook_Anim_Start = {};
	_bool						     m_bCanHit = true;
	_bool							 m_bSpecial_Skill_Progress = {};
	_bool							 m_bCatch_Special_Attack = {};
	_bool							 m_bDecorative_Render = {};

	_float                           m_fFireBall_Delete_Time = {};
	_float							 m_fSpecial_Skill_CoolTime = {};

	_uint                            m_iNearPatternIndex = {};
	_uint                            m_iFarPatternIndex = {};
	_uint							 m_iSpawn_Cell_Index = {};
	_uint					         m_iHit_Motion_Index = {};
	_uint						     m_iParryReadyHits = {};
private:
	CState_Machine<CElite_Researcher>* m_pState_Manager = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CElite_Researcher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//»óÅÂµé
	class Idle_State : public CStates<CElite_Researcher>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	};

	class Move_State : public CStates<CElite_Researcher>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	};

	class Hit_State : public CStates<CElite_Researcher>
	{
	public:
		Hit_State(_uint pHitNum);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_uint m_iHitNum = -1;
	};

	class Stun_State : public CStates<CElite_Researcher>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Execution_State : public CStates<CElite_Researcher>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	};

	class Return_To_SpawnPoint_State : public CStates<CElite_Researcher>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Attack_ComboA : public CStates<CElite_Researcher>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	};

	class Attack_ComboB : public CStates<CElite_Researcher>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bBonus_Attack_Check = {};
	};

	class Attack_ComboC : public CStates<CElite_Researcher>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bSpawn_Deco = {};
	};

	class Parry_Attack_A : public CStates<CElite_Researcher>
	{
	public:
		Parry_Attack_A() = default;
		virtual ~Parry_Attack_A() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bIs_Fired = {};
	};

	class Parry_Attack_B : public CStates<CElite_Researcher>
	{
	public:
		Parry_Attack_B() = default;
		virtual ~Parry_Attack_B() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bIs_Fired = {};
	};

	class Shoot_Attack_A : public CStates<CElite_Researcher>
	{
	public:
		Shoot_Attack_A() = default;
		virtual ~Shoot_Attack_A() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bIs_Fired = {};
	};

	class Attack_Special : public CStates<CElite_Researcher>
	{
	public:
		Attack_Special() = default;
		virtual ~Attack_Special() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	private:
		_bool m_bCatch_Success = {};
	};
	class Attack_Special_Catch : public CStates<CElite_Researcher>
	{
	public:
		Attack_Special_Catch() = default;
		virtual ~Attack_Special_Catch() = default;
	public:
		void State_Enter(CElite_Researcher* pObject) override;
		void State_Update(_float fTimeDelta, CElite_Researcher* pObject) override;
		void State_Exit(CElite_Researcher* pObject) override;
	};

};

END