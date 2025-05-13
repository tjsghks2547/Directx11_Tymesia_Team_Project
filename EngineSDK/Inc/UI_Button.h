#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI_Button abstract : public CUIObject
{
protected:
	CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Button(const CUI_Button& Prototype);
	virtual ~CUI_Button() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	virtual _bool Mouse_Select_Talent(HWND hWnd, MOUSEKEYSTATE eMouseKey);
	virtual _bool Mouse_Select(HWND hWnd, MOUSEKEYSTATE eMouseKey, _float fSize);
	//HRESULT On_Mouse_UI(HWND hWnd, _int iBeforeTexNumber, _uint iAfterTexNumber);
	_bool On_Mouse_UI(HWND hWnd, _uint iRange);

	void Set_Mouse_Select_OnOff(_bool bOn) { m_bMouseSelectOn = bOn; }
	_bool Get_Mouse_Select_OnOff() { return m_bMouseSelectOn; }

	void Set_Open_Image(_bool bOpen) { m_bImageOn = bOpen; }
	_bool Get_Mouse_OnOff() { return m_bImageOn; }

public:
	void Set_Change_TextColor(_float4 fColor) { m_fTextColor = { fColor.x / 255.f, fColor.y / 255.f,fColor.z / 255.f,fColor.w / 255.f }; }
	void Set_TextDrawType(TextDrawEffect eType) { m_eRenderType = eType; }

protected:
	_float4		m_fTextColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f };
	TextDrawEffect	m_eRenderType = { TEXT_DEFALUT };
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	//_bool m_bMouseClick = { false };
	_int iTestCount = { 0 };
	_bool m_bMouseSelectOn = { false };
	_bool m_bImageOn = { false };
};

END
