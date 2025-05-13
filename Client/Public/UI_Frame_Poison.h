#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Frame_Poison final : public CUI_Image
{
private:
	CUI_Frame_Poison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Frame_Poison(const CUI_Frame_Poison& Prototype);
	virtual ~CUI_Frame_Poison() = default;

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

	CTexture* m_pTexIconCom = { nullptr };
	CTexture* m_pTexNoiseCom = { nullptr };

	_bool m_bIconOn = { true };

public:
	HRESULT Ready_Components();

public:
	static CUI_Frame_Poison* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END