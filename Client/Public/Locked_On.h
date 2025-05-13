#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CLocked_On final : public CPartObject
{
public:
	struct LOCKED_ON_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		const _float4x4* pSocketMatrix = { nullptr };
		_bool* bLocked_On_Active = { nullptr };
	};

private:
	CLocked_On(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLocked_On(const CLocked_On& Prototype);
	virtual ~CLocked_On() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void Bill_Board();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	const _float4x4* m_pSocketMatrix = { nullptr };
private:
	const _uint* m_pParentState = { nullptr };
	_bool* m_bLocked_On = { nullptr };

	_uint m_iTextureNum = {};
	_uint m_iPassNum = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CLocked_On* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END