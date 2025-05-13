#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_LandingScreen final : public CUI_Image
{
private:
	CUI_LandingScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LandingScreen(const CUI_LandingScreen& Prototype);
	virtual ~CUI_LandingScreen() = default;

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

	_float m_fCurrentTime = { 0 };
	_float m_fSizeTime = { 0 };
	_float2 m_fMySize = {};
	const _float4x4* m_mtrSaveWorld = {};
public:
	HRESULT Ready_Components();

public:
	static CUI_LandingScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END