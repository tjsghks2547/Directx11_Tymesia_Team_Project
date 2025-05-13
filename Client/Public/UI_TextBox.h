#pragma once
#include "Client_Defines.h"
#include "UI_Text.h"

BEGIN(Client)

class CUI_TextBox final : public CUI_Text
{
private:
	CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_TextBox(const CUI_TextBox& Prototype);
	virtual ~CUI_TextBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:

	_uint				m_iTextID = {};

	_float2				m_fTextPosition = {};
	_float				m_fTextScale = {};
	_float4				m_fTextLayerDeth = {};
	_float4				m_fTextEffect = {};
	_float				m_fAnimAlpha = { 0.0f };

public:
	static CUI_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END