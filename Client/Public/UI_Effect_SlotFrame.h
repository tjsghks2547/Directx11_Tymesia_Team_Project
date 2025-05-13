#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_Effect_SlotFrame final : public CUI_Image
{
private:
	CUI_Effect_SlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Effect_SlotFrame(const CUI_Effect_SlotFrame& Prototype);
	virtual ~CUI_Effect_SlotFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_bool m_bCoolTime = { false };

	_float m_fCurrentTime = { 0 };
	_float m_fSizeTime = { 0 };
	_float2 m_fMySize = {};
	const _float4x4* m_mtrSaveWorld = {};
public:
	HRESULT Ready_Components();

public:
	static CUI_Effect_SlotFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END