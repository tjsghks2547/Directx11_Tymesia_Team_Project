#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CDecorative_Tonic final : public CPartObject
{
public:
	struct DECORATIVE_TONIC_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _float4x4* pHandSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		_bool* bChange = { nullptr };
	};

private:
	CDecorative_Tonic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDecorative_Tonic(const CDecorative_Tonic& Prototype);
	virtual ~CDecorative_Tonic() = default;

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
	const _float4x4* m_pHandSocketMatrix = { nullptr };

private:
	const _uint* m_pParentState = { nullptr };
	_uint m_iPassNum = {};
	_bool* m_bChange = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDecorative_Tonic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END