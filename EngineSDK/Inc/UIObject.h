#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	enum TEXTSORT { TEXT_CENTER, TEXT_LEFT, TEXT_RIGHT, TEXT_END };
public:
	struct UIOBJECT_DESC : public CGameObject::GAMEOBJECT_DESC	
	{
		_float			fX, fY, fZ;
		_float			fSizeX, fSizeY;
		_float			fNear, fFar;
		_wstring		strProtoName;
		_uint			iShaderPassNum;
		_int			iTexNumber;
		_uint			iGroupID;
		_float3			fRotation;
		//////////////////////////////////
		_wstring		strFontName;
		_wstring		strContent;
		TEXTSORT        eTextSort;
	};

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;
	void		Set_Alpha(_float _fAlpha) { m_fAlpha = _fAlpha; }


public:
	virtual _bool Mouse_Select(HWND hWnd, _float fX, _float fY);

public:
	virtual void Set_Render_OnOff(_bool bOpen) { m_bRenderOpen = bOpen; } // UIObj ���� ���� ���� - ����
	virtual _bool Get_Render_State() { return  m_bRenderOpen; }
	void Set_OnOff(_bool bOpen) { m_bOpen = bOpen; }
	_bool Get_OnOff() { return m_bOpen; }

public:
	virtual _uint Get_UI_GroupID() { return m_iGroupID; }

	virtual void Set_FontName(const _tchar* pFont) { m_strFontName = pFont; }
	virtual void Set_Content(const _tchar* pText) { m_strContentText = pText; }
	virtual _wstring& Get_Font_Name() { return m_strFontName; }
	virtual _wstring& Get_Content() { return m_strContentText; }

	void Set_TexNumber(_int iNumber) { m_iTexNumber = iNumber; }
	_int Get_TexNumber() { return m_iTexNumber; }
	virtual _uint Set_UI_ShaderPassNum(_uint iShadernum) { return m_iShaderPassNum = iShadernum; } // ���̴� �н� �ѹ� �������� -����


public:
	void Set_TextSort(TEXTSORT eSort) { m_eTextSort = eSort; }
	TEXTSORT Get_TextSort() { return m_eTextSort; }

protected:
	TEXTSORT m_eTextSort = { TEXT_LEFT }; // �ؽ�Ʈ ���� ����

protected:
	HRESULT Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg);

protected:
	_bool m_bOpen = { true }; // ������� ȣ�� ������ �� ���δ�. // �⺻�� Ű�� �� => �ڵ带 ���� ����


protected:
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};

	_bool				m_bRenderOpen = { false };// ���� ���� ����

	_float3				m_fPos = {};   
	_float2				m_fSize = {};
	_uint				m_iUIType = { 0 };		// UI ���� Ÿ��

	_wstring			m_strFontName;		// TextBox�뵵
	_wstring			m_strContentText;	// TextBox�뵵

	_wstring			m_strProtoName = {};
	_uint				m_iShaderPassNum = { 0 };
	_uint				m_iGroupID = { 0 };		// �׷���̵�
	_uint				m_iTexNumber = { 0 }; // �ؽ�ó �ѹ�
	_float				m_fAlpha = { 0.0f };

	_vector             m_vTextClolor = { 1.0f,1.0f,1.0f,1.0f };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END