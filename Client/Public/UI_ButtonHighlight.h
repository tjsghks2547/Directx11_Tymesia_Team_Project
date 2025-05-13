#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_ButtonHighlight final : public CUI_Button
{
private:
	CUI_ButtonHighlight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ButtonHighlight(const CUI_ButtonHighlight& Prototype);
	virtual ~CUI_ButtonHighlight() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	_bool Get_LogoOpen() { return m_ButtonShaderOpen; }

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_uint iMyShaderPass = {};
	_float3 m_fTextPosition = {};
	_bool  m_ButtonShaderOpen = { false };
	_float m_fCurrentTime = { 0 };
	
	_bool m_bSoundOnOff = { true };


	_float m_fShaderTime = {};

public:
	HRESULT Ready_Components();

public:
	static CUI_ButtonHighlight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END