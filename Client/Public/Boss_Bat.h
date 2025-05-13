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

class CBoss_Bat final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
private:
	CBoss_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Bat(const CBoss_Bat& Prototype);
	virtual ~CBoss_Bat() = default;
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
	void Load_Spike_SpawnPoints();

private:
	vector<_float4>                  m_vSpike_Spawn_Positions;
	vector<_float4>                  m_vSpike_Spawn_Positions_To_Three[3];
	vector<_float4>					 m_vSpike_Positions;

private:
	_bool						     m_bCan_Hit_Motion = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bSpecial_Skill_Progress = {};
	_bool                            m_bCatch_Special_Attack = {};
	_bool                            m_bCristal_Create = {};
	_bool							 m_bSummon_Spike = {};

	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fRecovery_Skill_CoolTime = {};
	_float                           m_fAir_Delete_Time = 3.f;

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;

	_uint                            m_iPhase = { PHASE_END };

private:
	CState_Machine<CBoss_Bat>* m_pState_Manager = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Bat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	class Intro_State : public CStates<CBoss_Bat>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Bat>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	class Move_State : public CStates<CBoss_Bat>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	private:
		_float m_fTimer = {};
	};

	class Recovery_State : public CStates<CBoss_Bat>
	{
	public:
		Recovery_State() = default;
		virtual ~Recovery_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	private:
		_float m_fRecovery_Amount = 20.f;
		_float m_fCurrent_Recovery_Speed = 8.f;
		_bool m_bFinish_Recovery = {};
		_float m_fRecoveryMax = {};
	};

	class Stun_State : public CStates<CBoss_Bat>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	class Execution_State : public CStates<CBoss_Bat>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	class Dead_State : public CStates<CBoss_Bat>
	{
	public:
		Dead_State() = default;
		virtual ~Dead_State() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//오른손 긁기 패턴(Index 4)
	class Attack_Combo_A : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//왼손 긁기 패턴 (Index 0)
	class Attack_Combo_B : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//두번 물기 패턴 (Index 7)
	class Attack_Combo_C : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//한번 물기 패턴 (Index 8)
	class Attack_Combo_D : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//뒤로 뺐다 돌진 패턴(Index 9)
	class Attack_Combo_E : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_E() = default;
		virtual ~Attack_Combo_E() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//기둥 소환 패턴(HellScream (Index 13))
	class Attack_Combo_F : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_F() = default;
		virtual ~Attack_Combo_F() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//점프뛰어서 짓누르는 패턴?(Index 17, 18(앞으로) 19(제자리))
	class Attack_Combo_G : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_G() = default;
		virtual ~Attack_Combo_G() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};

	//SonicBoom?아마 공기팡?(Index 22)
	class Attack_Combo_H : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_H() = default;
		virtual ~Attack_Combo_H() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	private:
		_bool m_IsFired = {};
	};

	//Storm이라 되어있음 이름이 아마 폭풍..?(Index = 24)
	class Attack_Combo_I : public CStates<CBoss_Bat>
	{
	public:
		Attack_Combo_I() = default;
		virtual ~Attack_Combo_I() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	};


	//파티클 지리는 스페셜 스킬 패턴
	class Attack_Special : public CStates<CBoss_Bat>
	{
	public:
		Attack_Special() = default;
		virtual ~Attack_Special() = default;
	public:
		void State_Enter(CBoss_Bat* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Bat* pObject) override;
		void State_Exit(CBoss_Bat* pObject) override;
	private:
		_bool m_bChange_Attack_Power = {};
	};

};

END