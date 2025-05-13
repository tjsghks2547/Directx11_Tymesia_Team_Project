#pragma once

#include "Base.h"
BEGIN(Engine)

class CRenderTarget;

class CTarget_Manager final : public CBase
{
private:
	CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTarget_Manager() = default; 

public:
	HRESULT Initialize();
	HRESULT Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, class CShader* pShader, const _char* pConstantName);
	HRESULT Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize);
	HRESULT Add_UAV_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag);	
	HRESULT Begin_MRT(const _wstring& strMRTTag, _bool isClear = true, ID3D11DepthStencilView* pDSV = nullptr);	
	HRESULT End_MRT(ID3D11DepthStencilView* _pDSV = nullptr);		

	HRESULT RTV_Compute_LightShaft(const _wstring& strRenderTargetTagToRead, const _wstring& strRenderTargetTagToWrite, class CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg);

	HRESULT RTV_Compute_Fog(const _wstring& strRenderTargetTagDepth, ID3D11ShaderResourceView* pNoiseSRV, const _wstring& strRenderTargetTagGodRay, const _wstring& strRenderTargetTagFinal, const _wstring& strRenderTargetTagRangeFogFinal, const _wstring& strRenderTargetTagFog, CShader_Compute_Deferred* pCompute_Shader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, void* pArg);

	HRESULT Clear_RTV(const _wstring& strRenderTargetTag);

	HRESULT Copy_RTV(const _wstring& strRenderTargetTag, const _wstring& strRenderTargetTagToCopy);
#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif 
		
private:
	ID3D11Device*									m_pDevice	     = { nullptr };
	ID3D11DeviceContext*							m_pContext	     = { nullptr };
																     
	ID3D11RenderTargetView*							m_pBackRTV       = { nullptr };
	ID3D11DepthStencilView*							m_pOriginalDSV   = { nullptr };

	
	map<const _wstring, CRenderTarget*>				m_RenderTargets;
	map<const _wstring, vector<CRenderTarget*>>		m_MRTs;

private:
	CRenderTarget* Find_RenderTarget(const _wstring& strRenderTargetTag);
	vector<CRenderTarget*>* Find_MRT(const _wstring& strMRTTag);	


public:
	static CTarget_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); 
	virtual void Free() override;

};

END