#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


BEGIN(Client)

class CWeapon_Magician2_Sword final : public CPartObject
{
	enum COLLIDER_CATEGORY
	{
		COLLIDER_SWORD,
		COLLIDER_HAND,
		COLLIDER_MUTATION,
		COLLIDER_SPECIAL,
		COLLIDER_BURST,
		COLLIDER_SLASH,
		COLLIDER_END
	};


public:
	struct MAGICIAN2_SWORD_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* bSpecial_Skill_Progress = { nullptr };
		_bool* bCatch_Special_Attack = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CWeapon_Magician2_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Magician2_Sword(const CWeapon_Magician2_Sword& Prototype);
	virtual ~CWeapon_Magician2_Sword() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	PxRigidDynamic* m_pActor[COLLIDER_END];

	const _float4x4* m_pSocket_Matrix = { nullptr };
	const _float4x4* m_pSocket_Hand_Matrix = { nullptr };
	const _float4x4* m_pSocket_Mutation_Matrix = { nullptr };
private:
	_bool* m_bSpecial_Skill_Progress = { nullptr };
	_bool* m_bCatch_Special_Attack = { nullptr };

	_bool  m_bColliderOff = {};

	_float m_fTimeDelta = { 0.f };
	_float             m_fDeadTimer = {};
	_float             m_fFinishTime = {};
	_uint              m_iPassNum = {};
private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CWeapon_Magician2_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END