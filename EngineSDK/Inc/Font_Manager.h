#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
private:
	CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFont_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);

	HRESULT Render(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_World(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Shadow(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Outline(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Color(const wstring& strFontTag, const _tchar* pText1, const _tchar* pText2, const _float2& vPosition,
		_float4 vColor1 = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float4 vColor2 = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	
	HRESULT Render_Alpha(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	_float2 Get_TextSize(const _wstring& strFontTag, const _tchar* pText);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	map<const _wstring, class CCustomFont*>     m_Fonts;


private:
	class CCustomFont* Find_Font(const _wstring& strFontTag);


public:
	static CFont_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;


};
END