#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_MapEntryFrame final : public CUI_Button
{
private:
	CUI_MapEntryFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MapEntryFrame(const CUI_MapEntryFrame& Prototype);
	virtual ~CUI_MapEntryFrame() = default;

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
	_bool m_bSoundOnOff = { true };


public:
	HRESULT Ready_Components();

public:
	static CUI_MapEntryFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END