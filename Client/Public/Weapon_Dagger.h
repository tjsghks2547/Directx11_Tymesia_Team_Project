#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;

END


BEGIN(Client)

class CWeapon_Dagger final : public CPartObject
{
public:
	struct WEAPON_DAGGER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CWeapon_Dagger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Dagger(const CWeapon_Dagger& Prototype);
	virtual ~CWeapon_Dagger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	PxRigidDynamic* m_pParryActor = { nullptr };
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
	static CWeapon_Dagger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END