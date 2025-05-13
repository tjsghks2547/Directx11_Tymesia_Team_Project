#pragma once
#include "UIObject.h"
BEGIN(Engine)
class ENGINE_DLL CUI_Image abstract : public CUIObject
{
protected:
	CUI_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Image(const CUI_Image& Prototype);
	virtual ~CUI_Image() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

	_bool On_Mouse_UI(HWND hWnd, _int iSize);
	_bool On_Mouse_Side_Select(HWND hWnd, _int iSize, MOUSEKEYSTATE eMouseKey);

	void Set_Open_Image(_bool bOpen) { m_bImageOn = bOpen; }
	_bool Get_Open_Image() { return m_bImageOn; }
protected:
	_bool m_bImageOn = { false };
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END