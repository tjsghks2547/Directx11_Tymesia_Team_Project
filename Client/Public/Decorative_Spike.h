#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CDecorative_Spike final : public CPartObject
{
public:
	struct DECORATIVE_SPIKE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		_float3* pRotate_Degree = { nullptr };
		_float4* pPosition = { nullptr };
		_bool* pRender = { nullptr };

	};

private:
	CDecorative_Spike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDecorative_Spike(const CDecorative_Spike& Prototype);
	virtual ~CDecorative_Spike() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	const _uint* m_pParentState = { nullptr };
	const _bool* m_pRender = { nullptr };
	_float m_fLinear = {};

	_float3 m_fMyScale = {};
	_float3 m_fMaxScale = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDecorative_Spike* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END