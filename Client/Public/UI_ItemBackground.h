#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_ItemBackground final : public CUI_Button
{
	//enum IconNum{}; 아이템 뭔지 이넘값으로 받아오면 좋겠는데 나중에 클래스 만들테니깐
private:
	CUI_ItemBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_ItemBackground(const CUI_ItemBackground& Prototype);
	virtual ~CUI_ItemBackground() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Item_Icon(_uint iIconNum) {m_iTexicon = iIconNum;}
	_uint Get_Item_Icon() { return m_iTexicon; }

	void Set_Item_Icon_OnOff(_bool bOpen) { m_bItemIconOn = bOpen;}
	_bool Get_Item_Icon_OnOff() { return m_bItemIconOn;}

	_bool Get_Slot_Mouse_OnOff() { return m_bMouseOn;}

	
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CTexture* m_pTexIconCom = { nullptr }; // 슬롯 위에 아이템이미지
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_uint m_iTexicon = {0};
	_bool m_bItemIconOn = { false };
	_bool m_bMouseOn = { false };
	_float3 m_fTextPosition = {};

	_bool m_bSoundOnOff = { true };


public:
	HRESULT Ready_Components();

public:
	static CUI_ItemBackground* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END