#pragma once

#include "Client_Defines.h"
#include "PartObject.h"
#include "Player.h"

BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CTexture;
END

BEGIN(Client)

class CCamera_Free;


class CPlayer_Weapon_JavelinSword final : public CPartObject
{
public:
	struct WEAPON_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		_uint* pParentState = { nullptr };
		const _uint* pParentPhaseState = { nullptr };
		CModel* pParentModel = { nullptr };
		_uint* pPreParentState = { nullptr };
		_float* pParentSkillBonusDamage = { nullptr };

	};
private:
	CPlayer_Weapon_JavelinSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Weapon_JavelinSword(const CPlayer_Weapon_JavelinSword& Prototype);
	virtual ~CPlayer_Weapon_JavelinSword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;


private:
	CTexture* m_pTextureCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	float			   m_AccColliderLifeAttack1 = { 0.f };
	float			   m_AccColliderLifeAttack2 = { 0.f };

	PxRigidDynamic* m_pActor = { nullptr };

private:
	_float			   m_fTime = { 0.f };
	_float			   m_fTimeDelta = { 0.f };
	_float			   m_fHitStopTime = { 0.f };
	_float			   m_fZoomBlurDeltaTime = { 0.f };

	_bool			   m_bHitStopOnOff = { false };
	_bool              m_bCollisionOn = { false };

	_uint m_iCurrentLevel = {}; //종한 추가 Level전환때문에


	/* 디졸브 관련 */
	_float m_fDissolveAmount = {};
	_float m_fAppearTimer = {};
	_float m_fDeadTimer = {};
	_float m_fFinishTime = {};
	_float m_fDeadStartTimer = {};
	_bool  m_bDeadOn = { false };
	_bool  m_bAppear = { false };



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


	/* 던지기 코드 관련 */
	_bool  m_bThrow = { false };
	_bool  m_bFirst = { false };
	_float4x4 m_PreParentMatrix = {  };
	_float4x4 m_PreSocketMatrix = {  };
	_float4x4 m_PreTransformMatrix = {  };

private:
	_uint* m_pParentState = { nullptr };
	_uint* m_pPreParentState = {};
	const _uint* m_pParentPhaseState = { nullptr };

	_float			   m_fSkillBaseDamage = {};
	_float* m_pParentSkillBonusDamage = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
	HRESULT Hit_Slow();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static  CPlayer_Weapon_JavelinSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END