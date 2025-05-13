#pragma once

#include "Base.h"

/* 1.화면에ㅐ 그려져야할 객체들을 모아둔다.  */
/* 1_1. 내가 그리고자하는 순서대로 객체들을 분류하여 모아둔다. */

/* 2.순서대로 모아놓은 객체들을 저장해놓은 순서대로 돌면서 렌더함수를 호출(렌더콜, 드로우콜)해준다. */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP {
		RG_PRIORITY, RG_SHADOW, RG_NONBLEND, RG_OCCULUSION, RG_NONLIGHT, RG_BLEND, RG_UI, RG_FONT,
		RG_BLUR, RG_GLOW, RG_DISTORTION, RG_MOTION_BLUR, RG_HIGHLIGHT, RG_GAUSSIAN_BLUR, RG_WEIGHTBLEND, RG_BLOOM,
		RG_FOG, RG_WATER,
		RG_END
	};

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroupID, class CGameObject* pGameObject);
	HRESULT Render();
	void Set_MotionBlur(_bool _bOnOff) { m_bMotionBlurOnOff = _bOnOff; }
	void Set_ZoomBlur_Option(_bool _bOnOff, _float _fStrength);
	void Set_Dithering(_bool _OnOff) { m_bDitheringOnOff = _OnOff; }	
	void Set_ReverseScreenEffect(_bool _OnOff) { m_bScreenReverseEffect_OnOff = _OnOff; }
	void Set_ReverseScreenRadius(_float _Radius) { m_fScreenReverseRadius = _Radius; }
	void Set_ReverseEnd(_bool _OnOff, _float _ReverseEndStrength)
	{
		m_bReverseEnd = _OnOff;
		m_fReverseEndStrength = _ReverseEndStrength;
	};
	_bool* Get_Dithering_Ptr() { return  &m_bDitheringOnOff; }		
	void Set_FogColor(_float4 vFogColor);
	void Set_LightShaftValue(_float4 _vLightShatValue);
	void Set_FogFactors(FOGPARAMS _ParamDesc) {
		m_ParamDesc = _ParamDesc;
	};


#ifdef _DEBUG
#endif // _DEBUG
	void Set_FogOnOff() {
		m_bFogStop = !m_bFogStop;
	};

	void Set_GodRayOnOff() {
		m_bGodRayStop = !m_bGodRayStop;
	};

	void Set_ShadowOnOff() {
		m_bShadowStop = !m_bShadowStop;
	};

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	list<class CGameObject*>		m_RenderObjects[RG_END];

	class CShader* m_pShader = { nullptr };
	class CShader* m_pShadowShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };

	_float4x4		      m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};
	_float4x4		      m_BlurWorldMatrix{}, m_BlurViewMatrix{}, m_BlurProjMatrix{};

	ID3D11DepthStencilView* m_pShadowDSV = { nullptr };
	ID3D11ShaderResourceView* m_pShadowSRV = { nullptr };

	_uint					m_iOriginalViewportWidth{}, m_iOriginalViewportHeight{};

	_bool					m_bMotionBlurOnOff = { false };	
	_bool					m_bZoomBlurOnOff = { false };	
	_bool					m_bDitheringOnOff = { true };	
	_bool					m_bScreenReverseEffect_OnOff = { false };	
	_bool					m_bReverseEnd = { false };	

	_float					m_fScreenReverseRadius = { 0.f };	
	_float					m_fZoomBlurStrength    = { 0.f };	
	_float					m_fReverseEndStrength  = { 0.f };	

	vector<unsigned char>							noiseData;

	_int											m_perm[512] = {};
	ID3D11Texture3D* m_pNoiseTexture3D = { nullptr };
	ID3D11ShaderResourceView* m_pNoiseSRV = { nullptr };

	_float											m_fTime = { 0.f };
	_float4											m_vFogColor = { 0.f, 0.f, 0.f, 1.f };
	_float4											m_vLightShaftValue = { 0.f, 0.f, 0.f, 0.f };
	FOGPARAMS										m_ParamDesc = {};



#ifdef _DEBUG
#endif // _DEBUG
	_bool											m_bFogStop = { false };
	_bool											m_bGodRayStop = { false };
	_bool											m_bShadowStop = { false };


	class CShader_Compute_Deferred* m_pLightShaftComputeShader = { nullptr };
	class CShader_Compute_Deferred* m_pFogComputeShader = { nullptr };

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonBlend();
	HRESULT Render_Occulsion();
	HRESULT Render_Distortion();
	HRESULT Render_GlowBegin();
	HRESULT Render_GlowX();
	HRESULT Render_GlowY();
	HRESULT Render_LightShaftX();
	HRESULT Render_LightShaftY();
	HRESULT Render_Fog();
	HRESULT Render_MotionBlur_By_Velocity();
	HRESULT Render_Water();

	HRESULT Render_NonLight();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_Shadow_Final();
	HRESULT Render_HighLightBegin();
	HRESULT Render_HighLightX();
	HRESULT Render_HighLightY();
	HRESULT Render_Final();
	HRESULT Render_Blend();
	HRESULT Render_WeightBlend();
	HRESULT Render_UI();
	HRESULT Render_Font();

	HRESULT	Render_BloomBegin();
	HRESULT	Render_BloomX();
	HRESULT	Render_BloomY();

	HRESULT Render_Zoom_Blur();


private:
	HRESULT Ready_Depth_Stencil_Buffer(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** ppOut);
	HRESULT SetUp_ViewportDesc(_uint iWidth, _uint iHeight);

	HRESULT Add_NoiseTexture();
	HRESULT Bind_NoiseTexture(class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
	_bool m_bDebugRender = { false };
#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END