#pragma once
#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_Bar_Poison final : public CUI_Image
{
private:
	CUI_Bar_Poison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Bar_Poison(const CUI_Bar_Poison& Prototype);
	virtual ~CUI_Bar_Poison() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexNoiseCom = { nullptr };
	CTexture* m_pTexGlowCom = { nullptr };
public:
	_bool Get_DeadSet() { return m_bDeadSet; }

private:
	_float m_fPoisonTime = {};
	_float m_fDeadTime = {60.f};
	_bool m_bDeadSet = { false };
	_float m_fTimeDelta = {};

public:
	HRESULT Ready_Components();

public:
	static CUI_Bar_Poison* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END