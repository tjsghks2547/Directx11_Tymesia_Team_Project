#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;

END


BEGIN(Client)

class CWeapon_GreatSword final : public CPartObject
{
public:
	struct WEAPON_GreatSword_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CWeapon_GreatSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_GreatSword(const CWeapon_GreatSword& Prototype);
	virtual ~CWeapon_GreatSword() = default;

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
	const _float4x4* m_pSocketMatrix = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
private:
	_bool              m_bColliderOff = {};
	_uint              m_iPreAnimIndex = {};
	_uint              m_iPassNum = {};

	_float			   m_fTimeDelta = { 0.f };
	_float             m_fDeadTimer = {};
	_float             m_fFinishTime = {};

private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CWeapon_GreatSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END