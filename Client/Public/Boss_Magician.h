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

class CBoss_Magician final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
private:
	CBoss_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Magician(const CBoss_Magician& Prototype);
	virtual ~CBoss_Magician() = default;
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
	_vector Bezier_Move(_vector _vStartPos, _vector vCurvePos, _vector vEndPos, _float fTime);

public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);
private:
	HRESULT Load_SporeObject();
public:
	void Near_Pattern_Create();
	void Far_Pattern_Create();

private:
	_bool                            m_bCardActive = {};
	_bool							 m_bIntro_Body_Render = {};
	_bool                            m_bIntro_Card_Render = {};
	_bool                            m_bIntro_Card_Change = {};
	_bool						     m_bCan_Hit_Motion = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bSpecial_Skill_Progress = {};
	_bool                            m_IsDissolveOn = {};
	_bool                            m_IsDissolveOff = {};
	_bool                            Is_Change_Sword_Bone = {};
	_bool                            m_bCanDissapear = {};
	_bool                            m_bCatch_Special_Attack = {};
	_bool                            m_bCane_Collider_On = {};

	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fSlash_Skill_CoolTime = {};
	_float                           m_fCard_Delete_Time = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint							 m_iHit_Motion_Index = -1;
	_uint							 m_iParryReadyHits = {};
	_uint                            m_iPhase = { PHASE_END };


private:
	CState_Machine<CBoss_Magician>* m_pState_Manager = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Magician* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	class Intro_State : public CStates<CBoss_Magician>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Magician>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Dissappear_Idle_State : public CStates<CBoss_Magician>
	{
	public:
		Dissappear_Idle_State() = default;
		virtual ~Dissappear_Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Move_State : public CStates<CBoss_Magician>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Dissappear_Move_State : public CStates<CBoss_Magician>
	{
	public:
		Dissappear_Move_State(_uint iDissappear_Index, _bool bShootCard = false);
		virtual ~Dissappear_Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bShootCard = {};
		_uint m_iDissappear_Index = {};
		_uint iRandomMove = {};
	};

	class Stun_State : public CStates<CBoss_Magician>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};


	class Shoot_ComboA : public CStates<CBoss_Magician>
	{
	public:
		Shoot_ComboA() = default;
		virtual ~Shoot_ComboA() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bIs_Fired = {};
	};

	class Shoot_ComboB : public CStates<CBoss_Magician>
	{
	public:
		Shoot_ComboB() = default;
		virtual ~Shoot_ComboB() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bIs_Fired = {};
		_float4 m_vPos = {};
		_float4 m_vPlayerPos = {};
	};

	class Attack_ComboA : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_ComboB : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_ComboC : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_ComboD : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboD() = default;
		virtual ~Attack_ComboD() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bFirst = {};
		_float4 m_vStartPos = {};
		_float4 m_vCurvePos = {};
		_float4 m_vEndPos = {};
		_float m_fLinearTime = {};
	};

	class Attack_Slash : public CStates<CBoss_Magician>
	{
	public:
		Attack_Slash() = default;
		virtual ~Attack_Slash() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_Special : public CStates<CBoss_Magician>
	{
	public:
		Attack_Special() = default;
		virtual ~Attack_Special() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bCatch_Success = {};
	};

	class Attack_Special_Catch : public CStates<CBoss_Magician>
	{
	public:
		Attack_Special_Catch() = default;
		virtual ~Attack_Special_Catch() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bAttack = {};
	};

	class Hit_State : public CStates<CBoss_Magician>
	{
	public:
		Hit_State(_uint iHit_Index);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_uint m_iHit_Index = {};
	};

	class ExeCution_State : public CStates<CBoss_Magician>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Parry_Attack_A : public CStates<CBoss_Magician>
	{
	public:
		Parry_Attack_A() = default;
		virtual ~Parry_Attack_A() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Parry_Attack_B : public CStates<CBoss_Magician>
	{
	public:
		Parry_Attack_B() = default;
		virtual ~Parry_Attack_B() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Parry_Attack_C : public CStates<CBoss_Magician>
	{
	public:
		Parry_Attack_C() = default;
		virtual ~Parry_Attack_C() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Dissappear_Jump_State : public CStates<CBoss_Magician>
	{
	public:
		Dissappear_Jump_State() = default;
		virtual ~Dissappear_Jump_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_Is_Spawn = {};
	};

	class Phase_Change_State : public CStates<CBoss_Magician>
	{
	public:
		Phase_Change_State() = default;
		virtual ~Phase_Change_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

};

END