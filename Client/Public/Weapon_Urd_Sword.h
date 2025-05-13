#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


BEGIN(Client)

class CCamera_Free;

class CWeapon_Urd_Sword final : public CPartObject
{
public:
	struct URD_SWORD_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pIntroSocketMatrix = { nullptr };
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* bChange_Socket = { nullptr };
		_bool* bCollider_Change = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CWeapon_Urd_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Urd_Sword(const CWeapon_Urd_Sword& Prototype);
	virtual ~CWeapon_Urd_Sword() = default;

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
	PxRigidDynamic* m_pActor = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _float4x4* m_pIntroSocketMatrix = { nullptr };

private:
	_bool  m_bColliderOff = {};
	_bool* m_bChange_Socket = { nullptr };
	_bool* m_bCollider_Change = { nullptr };

	_uint m_iPassNum = {};

	_float			   m_fTimeDelta = { 0.f };

	_float			   m_fReverseRadius = { 0.f };
	_float			   m_fZoomBlurStrength = { 0.f };
	_float			   m_fReverseEndTime = { 1.f };

	CCamera_Free* m_pCamera = { nullptr };

private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CWeapon_Urd_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END