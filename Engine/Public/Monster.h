#pragma once

#include "ContainerObject.h"

BEGIN(Engine)

class ENGINE_DLL CMonster abstract : public CContainerObject
{
public:
	struct MONSTER_DESC : public CGameObject::GAMEOBJECT_DESC
	{

	};
protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	_bool Get_Active() const { return m_bActive; }
	_bool Get_Dead() const { return m_bDead; }
	_bool Get_IsRender() const { return m_bRender; }
	_bool Get_IsDelete() const { return m_IsDelete; }

	_float Get_Distance() const { return m_fDistance; }

	void Set_HP_Bar_ActiveOff() { m_bHP_Bar_Active = false; }
	void Set_IsRender(_bool bRender) { m_bRender = bRender; }
	void Set_IsDelete(_bool bDelete) { m_IsDelete = bDelete; }
	void Set_IsClosest(_bool bClosest) { m_bIsClosest = bClosest; }
public:
	void Reset_Info();
	void Cheat();


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);
public:
	void RootAnimation();
	void CalCulate_Distance();
public:
	virtual void PatternCreate();
	virtual void State_Update(_float fTimeDelta);
	virtual void Active();
	virtual void Return_To_Spawn();
	virtual void Stun();

	void RotateDegree_To_Player();
	void Rotation_To_Player();
	_bool Is_Player_Near();
	void Recovery_HP();

	_uint Get_MonsterSkill() { return m_iMonsterSkill; }	

protected:
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	PxRigidDynamic* m_pStunActor = { nullptr };
	const _float4x4* m_pRootMatrix = { nullptr };
protected:
	class CGameObject* m_pPlayer = { nullptr };
protected:
	_float4                          m_vPlayerPos = {};
	_float4                          m_vSpawnPoint = {};

	_bool							 m_Is_Boss = {};
	_bool                            m_bIsClosest = {};
	_bool							 m_bNot_Need_Root = {};
	_bool                            m_bActive = {};
	_bool                            m_bRender = {};
	_bool                            m_bCanRecovery = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_IsStun = {};
	_bool                            m_bHP_Bar_Active = {};
	_bool                            m_bDead = {};
	_bool                            m_IsDelete = {};
	_bool                            m_Is_Player_Dead = {};

	_uint							 m_iSpawn_Cell_Index = {};
	_uint                            m_iHitCount = {};
	_uint                            m_iMonster_Attack_Power = {};
	const _uint* m_Player_Attack = { nullptr };
	const _uint* m_Player_Phase = { nullptr };
	const _uint* m_Player_State = { nullptr };

	_float                           m_fRotateDegree = {};
	_float                           m_fRotateSpeed = {};
	_float                           m_fDelayTime = {};
	_float                           m_fDistance = {};
	_float                           m_fActive_Distance = {};
	//루트 모션을 진행할 플레이어와의 거리 제한? 
	_float                           m_fRootDistance = {};
	_float                           m_fSpawn_Distance_Max = {};

	_float							 m_fCoolTime = {};
	_float                           m_fSpawn_Distance = {};
	_float                           m_fTimeDelta = {};
	_float                           m_fHP_Bar_Height = {};
	_float                           m_fHP_Bar_Active_Timer = {};

	_float                           m_fMonsterMaxHP = {};
	_float                           m_fMonsterCurHP = {};
	_float                           m_fShieldHP = {};
	_float                           m_fRecoveryTime = {};

	/* 몬스터 약탈 스킬 */
	_uint							 m_iMonsterSkill = {};	


	/* 몬스터 네비 오프셋 */
	_float							 m_fNaviOffset = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END