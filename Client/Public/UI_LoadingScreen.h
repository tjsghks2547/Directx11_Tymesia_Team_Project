#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_LoadingScreen final : public CUI_Image
{
private:
	CUI_LoadingScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LoadingScreen(const CUI_LoadingScreen& Prototype);
	virtual ~CUI_LoadingScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	_uint Get_ImageTex() { return m_iTexNumber; }


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };



public:
	HRESULT Ready_Components();

public:
	static CUI_LoadingScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END