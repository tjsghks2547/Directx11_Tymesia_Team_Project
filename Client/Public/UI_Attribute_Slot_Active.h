#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Attribute_Slot_Active final : public CUI_Button
{
private:
	CUI_Attribute_Slot_Active(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Attribute_Slot_Active(const CUI_Attribute_Slot_Active& Prototype);
	virtual ~CUI_Attribute_Slot_Active() = default;

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
	_bool m_bImageLoopOn = { false };
	_float m_fCurrentTime = {};
public:
	HRESULT Ready_Components();

public:
	static CUI_Attribute_Slot_Active* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END