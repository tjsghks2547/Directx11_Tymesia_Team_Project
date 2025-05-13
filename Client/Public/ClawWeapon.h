#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Player.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CCamera_Free;


class CClawWeapon final : public CPartObject
{
public:
	struct WEAPON_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		_uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_uint* pParentPhaseState = { nullptr };
		_int* pParentHp = { nullptr };
		_int* pParentMp = { nullptr };
		_float* pParentSkillBonusDamage = { nullptr };
	};
private:
	CClawWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClawWeapon(const CClawWeapon& Prototype);
	virtual ~CClawWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	float			   m_AccColliderLifeAttack1 = { 0.f };
	float			   m_AccColliderLifeAttack2 = { 0.f };

	PxRigidDynamic* m_pActor = { nullptr };


	_int* m_pParentHp = { nullptr };
	_int* m_pParentMp = { nullptr };

private:
	_float			   m_fTimeDelta = { 0.f };
	_float			   m_fAccTimeDelta = { 0.f };
	_float			   m_fHitStopTime = { 0.f };

	_float			   m_fTimer_Effect1 = { 0.5f };
	_float			   m_fTimer_Effect2 = { 0.5f };

	_uint		       m_iPreParentState = {};
	_bool			   m_bHitStopOnOff = { false };
	_bool              m_bCollisionOn = { false };

	_uint m_iCurrentLevel = {}; //종한 추가 Level전환때문에


	/* 손톱 디졸브 관련 */
	_float* m_pfClawParentTime = { };
	_float* m_pfClawFinishTime = {};
	_float* m_pfClawAppearTimer = {};
	_bool* m_pbClawDeadOn = {};
	_bool* m_pbClawAppear = {};


private:
	_uint* m_pParentState = { nullptr };
	_uint* m_pParentPhsaeState = { nullptr };

	_float			   m_fSkillBaseDamage = {};
	_float* m_pParentSkillBonusDamage = { nullptr };

	unordered_set<CPlayer::STATE>* m_pSet_Body_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Claw_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Halberd_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Right_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Scythe_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Axe_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_GreadSword_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_JavelinSword_Weapon_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Player_Camera_States = { nullptr };
	unordered_set<CPlayer::STATE>* m_pSet_Cane_Weapon_States = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Hit_Slow();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static  CClawWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END