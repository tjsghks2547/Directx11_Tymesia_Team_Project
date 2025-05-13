#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CDecorative_Mutation2 final : public CPartObject
{
public:
	struct DECORATIVE_MUTATION2_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
	};

private:
	CDecorative_Mutation2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDecorative_Mutation2(const CDecorative_Mutation2& Prototype);
	virtual ~CDecorative_Mutation2() = default;

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
	_uint m_iPassNum = {};
	_float m_fDeadTimer = {};
	_float m_fFinishTime = {};
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDecorative_Mutation2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END