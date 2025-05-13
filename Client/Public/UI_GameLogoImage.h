#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_GameLogoImage final : public CUI_Image
{
private:
	CUI_GameLogoImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_GameLogoImage(const CUI_GameLogoImage& Prototype);
	virtual ~CUI_GameLogoImage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool Get_LogoOpen() { return m_LogoOpen; }

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };

	_float m_fCurrentTime = { 0 };
	_bool  m_LogoOpen = { false };

public:
	HRESULT Ready_Components();

public:
	static CUI_GameLogoImage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END