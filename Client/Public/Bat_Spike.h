#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


BEGIN(Client)

class CBat_Spike final : public CPartObject
{
public:
	struct BAT_SPIKE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* bRender = { nullptr };
		_float4* pPivot_Position = { nullptr };
	};
private:
	CBat_Spike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBat_Spike(const CBat_Spike& Prototype);
	virtual ~CBat_Spike() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Spawn(_float fTimeDelta);
	void Reset(_float fTimeDelta);

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };

private:
	_float4* m_pPivot_Position = { nullptr };
	_bool* m_pRender = { nullptr };
private:
	_bool  m_bColliderOff = {};
	_bool  m_bFirst = {};

	_float m_fLinear = {};
	_float3 m_fMyScale = {};
	_float3 m_fMaxScale = {};

	_uint m_iPassNum = {};
private:
	const _uint* m_pParentState = { nullptr };

private:
	_float4                         m_fModelPos = {};
	_float                          m_fModelHeightCenterY = {};

	_float                          m_fExplosionPower = {};
	_float                          m_fFallingTime = {};
	_float                          m_fLifeTime = {};

	_bool                           m_bHitted = { false };
	_bool                           m_bUpdating = { false };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBat_Spike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END