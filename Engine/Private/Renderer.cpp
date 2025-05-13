#include "..\Public\Renderer.h"

#include "GameObject.h"
#include "GameInstance.h"

#include "Shader_Compute_Deferred.h"

#include "DirectXTex/DirectXTex.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	_uint   iNumViewports = { 1 };
	D3D11_VIEWPORT  ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_iOriginalViewportWidth = static_cast<_uint>(ViewportDesc.Width);
	m_iOriginalViewportHeight = static_cast<_uint>(ViewportDesc.Height);

	/* Target_Diffuse */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Depth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1000.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_MtrlSpecular"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Depth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Roughness"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Emissive"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target Occulusion*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Occulsion"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Reflection"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Water"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse_Copy"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	CShader_Compute_Deferred::LIGHTSHAFTPARAMS LightShaftDesc = {};

	LightShaftDesc.g_LightShaftValue = _float4(1.f, 0.97f, 1.f, 1.f);
	LightShaftDesc.g_ScreenLightPos = _float2(0.f, 0.f);

	m_pLightShaftComputeShader = CShader_Compute_Deferred::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Compute_Sample.hlsl"), "CSMain_Sample", nullptr, 0, &LightShaftDesc, CShader_Compute_Deferred::DEFERRED_TYPE_LIGHTSHAFT);

	m_ParamDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
	m_ParamDesc.fFogStartDistance = _float2(0.03f, 8.f);
	m_ParamDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
	m_ParamDesc.g_FogColor = m_vFogColor;

	m_pFogComputeShader = CShader_Compute_Deferred::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Compute_Fog.hlsl"), "CSMain_Fog", nullptr, 0, &m_ParamDesc, CShader_Compute_Deferred::DEFERRED_TYPE_FOG);

	/* Target_Fog */
	if (FAILED(m_pGameInstance->Add_UAV_RenderTarget(TEXT("Target_Fog"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target LightShaft*/
	if (FAILED(m_pGameInstance->Add_UAV_RenderTarget(TEXT("Target_Compute_LightShaft"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target LightShaft*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightShaftX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target LightShaft*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightShaftY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Shadow */
	if (FAILED(m_pGameInstance->Add_Shadow_RenderTarget(TEXT("Target_Shadow"), g_iMaxWidth, g_iMaxHeight, DXGI_FORMAT_R32_FLOAT, _float4(1.f, 1000.f, 1.f, 0.f), 3)))
		return E_FAIL;

	if (FAILED(Ready_Depth_Stencil_Buffer(g_iMaxWidth, g_iMaxHeight, &m_pShadowDSV)))
		return E_FAIL;

	/* Target_Final */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_GlowBegin */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowBegin"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_GlowX */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_GlowY */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Distortion */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Distortion"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_MotionBlur */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_MotionBlur"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_MotionBlur_By_Velocity */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_MotionBlur_By_Velocity"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_HighLight */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLight"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_HighLightX */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLightX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_HighLightY */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLightY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Shadow */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shadow_Final"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_WeightBlend */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_WeightBlend"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_BloomBegin */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BloomBegin"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_BloomX */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BloomX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_BloomY */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_BloomY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* Target_Final_Last */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final_Last"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_RangeFog*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_RangeFog_Front"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_RangeFog*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_RangeFog_Back"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_RangeFog_Final*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_RangeFog_Final"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* MRT_GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_MtrlSpecular"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Roughness"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Emissive"))))
		return E_FAIL;

	//MRT_Water
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Reflection"), TEXT("Target_Reflection"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Water"), TEXT("Target_Water"))))
		return E_FAIL;

	//Occulusion Texture
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Occulsion"), TEXT("Target_Occulsion"))))
		return E_FAIL;

	//LightShaft Blur Texture
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightShaftX"), TEXT("Target_LightShaftX"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightShaftY"), TEXT("Target_LightShaftY"))))
		return E_FAIL;

	/* MRT_LightAcc */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;
	/* MRT_Shadow */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))
		return E_FAIL;
	/* MRT_Shadow_FInal*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow_Final"), TEXT("Target_Shadow_Final"))))
		return E_FAIL;
	/* MRT_Final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final"), TEXT("Target_Final"))))
		return E_FAIL;
	/* MRT_GlowBegin */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowBegin"), TEXT("Target_GlowBegin"))))
		return E_FAIL;
	/* MRT_GlowX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowX"), TEXT("Target_GlowX"))))
		return E_FAIL;
	/* MRT_GlowY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowY"), TEXT("Target_GlowY"))))
		return E_FAIL;
	/* MRT_Distortion */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;
	/* MRT_MotionBlur_By_Velocity */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_MotionBlur_By_Velocity"), TEXT("Target_MotionBlur_By_Velocity"))))
		return E_FAIL;
	/* MRT_MotionBlur */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_MotionBlur"), TEXT("Target_MotionBlur"))))
		return E_FAIL;
	/* MRT_HighLight */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLight"), TEXT("Target_HighLight"))))
		return E_FAIL;
	/* MRT_HighLightX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLightX"), TEXT("Target_HighLightX"))))
		return E_FAIL;
	/* MRT_HighLightY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLightY"), TEXT("Target_HighLightY"))))
		return E_FAIL;
	/* MRT_WeightBlend*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_WeightBlend"), TEXT("Target_WeightBlend"))))
		return E_FAIL;
	/* MRT_BloomBegin */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BloomBegin"), TEXT("Target_BloomBegin"))))
		return E_FAIL;
	/* MRT_BloomX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BloomX"), TEXT("Target_BloomX"))))
		return E_FAIL;
	/* MRT_BloomY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_BloomY"), TEXT("Target_BloomY"))))
		return E_FAIL;

	/* 진짜 최종 final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final_Last"), TEXT("Target_Final_Last"))))
		return E_FAIL;

	/* MRT_Fog_Front */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Fog_Front"), TEXT("Target_RangeFog_Front"))))
		return E_FAIL;

	/* MRT_Fog_Back */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Fog_Back"), TEXT("Target_RangeFog_Back"))))
		return E_FAIL;

	/* MRT_Fog_Final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Fog_Final"), TEXT("Target_RangeFog_Final"))))
		return E_FAIL;


	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	m_pShadowShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Shadow.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;


	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	_float fWidth{}, fHeight{};

	fWidth = ViewportDesc.Width;
	fHeight = ViewportDesc.Height;

	if (FAILED(m_pShader->Bind_RawValue("g_fViewPortWidth", &fWidth, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_fViewPortHeight", &fHeight, sizeof(_float))))
		return E_FAIL;

#ifdef _DEBUG
	
	_float fSizeX{ 150.f }, fSizeY{ 150.f }, fStartPositionX{ 80.f }, fStartPositionY{ 80.f }, fIntervalX{ 155.f }, fIntervalY{ 155.f };
	_uint iCountX{}, iCountY{};

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Depth"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shadow"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * (iCountY++)), fSizeX, fSizeY)))
		return E_FAIL;

	iCountX = 0;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Final"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_HighLightX"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_HighLightY"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_MotionBlur_By_Velocity"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_MotionBlur"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY++), fSizeX, fSizeY)))
		return E_FAIL;

	iCountX = 0;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_GlowBegin"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_GlowX"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_GlowY"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightShaftX"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightShaftY"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * (iCountY++)), fSizeX, fSizeY)))
		return E_FAIL;

	iCountX = 0;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_BloomBegin"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_BloomX"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_BloomY"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * (iCountY++)), fSizeX, fSizeY)))
		return E_FAIL;

	iCountX = 0;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Final_Last"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_RangeFog_Front"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_RangeFog_Back"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_RangeFog_Final"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY), fSizeX, fSizeY)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Water"), fStartPositionX + (fIntervalX * (iCountX++)), fStartPositionY + (fIntervalY * iCountY++), fSizeX, fSizeY)))
		return E_FAIL;
#endif // _DEBUG

	Add_NoiseTexture();

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroupID, CGameObject* pGameObject)
{
	if (nullptr == pGameObject ||
		eRenderGroupID >= RG_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroupID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
	if (FAILED(Render_Priority()))
		return E_FAIL;
	
	if (FAILED(Render_Shadow()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_Occulsion()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	if (FAILED(Render_GlowBegin()))
		return E_FAIL;

	if (FAILED(Render_GlowX()))
		return E_FAIL;

	if (FAILED(Render_GlowY()))
		return E_FAIL;

	if (FAILED(Render_BloomBegin()))
		return E_FAIL;

	if (FAILED(Render_BloomX()))
		return E_FAIL;

	if (FAILED(Render_BloomY()))
		return E_FAIL;

	if (FAILED(Render_LightShaftX()))
		return E_FAIL;

	if (FAILED(Render_LightShaftY()))
		return E_FAIL;

	if (FAILED(Render_Distortion()))
		return E_FAIL;

	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Render_Shadow_Final()))
		return E_FAIL;

	if (FAILED(Render_Fog()))
		return E_FAIL;

	if (FAILED(Render_MotionBlur_By_Velocity()))
		return E_FAIL;

	if (FAILED(Render_HighLightBegin()))
		return E_FAIL;

	if (FAILED(Render_HighLightX()))
		return E_FAIL;

	if (FAILED(Render_HighLightY()))
		return E_FAIL;

	if (FAILED(Render_WeightBlend()))
		return E_FAIL;

	if (FAILED(Render_Final()))
		return E_FAIL;

	if (FAILED(Render_Zoom_Blur()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Font()))
		return E_FAIL;

#ifdef _DEBUG
	if (m_pGameInstance->isKeyEnter(DIK_F11))
		m_bDebugRender = !m_bDebugRender;

	if (m_bDebugRender)
	{
		if (FAILED(Render_Debug()))
			return E_FAIL;
	}
#endif
	

	return S_OK;
}

void CRenderer::Set_ZoomBlur_Option(_bool _bOnOff, _float _fStrength)
{
	m_bZoomBlurOnOff = _bOnOff;
	m_fZoomBlurStrength = _fStrength;

}

void CRenderer::Set_FogColor(_float4 vFogColor)
{
	m_vFogColor = vFogColor;
}

void CRenderer::Set_LightShaftValue(_float4 _vLightShatValue)
{
	m_vLightShaftValue = _vLightShatValue;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		//Safe_Release(pRenderObject);
	}

	//m_RenderObjects[RG_PRIORITY].clear();

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Reflection"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (FAILED(pRenderObject->Render_Reflection()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Shadow()
{
	/* Shadow */
	m_pContext->ClearDepthStencilView(m_pShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), false, m_pShadowDSV)))
		return E_FAIL;

	if (FAILED(SetUp_ViewportDesc(g_iMaxWidth, g_iMaxHeight)))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_SHADOW])
	{
		if (FAILED(pRenderObject->Render_Shadow()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_SHADOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(SetUp_ViewportDesc(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Copy_RTV(TEXT("Target_Diffuse"), TEXT("Target_Diffuse_Copy")))) // MRT를 사용중엔 MRT에 바인딩  되어있는 타겟들을 사용하지 못하므로 MRT가 끝난 이후 값을 그대로 복사해와 사용한다.
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"), false)))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_WATER])
	{
		if (FAILED(pRenderObject->Render_Reflection()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_WATER].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Render_Occulsion()
{
	if (!m_bGodRayStop)
	{
		if (!XMVector4Equal(XMLoadFloat4(&m_vLightShaftValue), XMVectorSet(0.f, 0.f, 0.f, 0.f)))
		{
			if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Occulsion"))))
				return E_FAIL;

			m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
			m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
			m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

			if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
				return E_FAIL;

			if (FAILED(m_pGameInstance->Bind_LightPos(m_pShader, "g_ScreenLightPos")))
				return E_FAIL;

			if (FAILED(m_pGameInstance->Bind_LightDir(m_pShader, "g_ScreenLightDir")))
				return E_FAIL;

			_matrix matWorld = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);

			_float4 CamDir;
			XMStoreFloat4(&CamDir, matWorld.r[1]);

			if (FAILED(m_pShader->Bind_RawValue("g_ScreenCameraDir", &CamDir, sizeof(_float4))))
				return E_FAIL;

			m_pShader->Begin(13);
			m_pVIBuffer->Bind_InputAssembler();
			m_pVIBuffer->Render();

			if (FAILED(m_pGameInstance->End_MRT()))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CRenderer::Render_Distortion()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_DISTORTION])
	{
		if (FAILED(pRenderObject->Render_Distortion()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_DISTORTION].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowBegin"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_GLOW])
	{
		if (FAILED(pRenderObject->Render_Glow()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_GLOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowBegin"), m_pShader, "g_GlowBeginTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(7);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowX"), m_pShader, "g_GlowXTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(8);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}



HRESULT CRenderer::Render_LightShaftX()
{
	CShader_Compute_Deferred::LIGHTSHAFTPARAMS ParamDesc = {  };

	ParamDesc.g_LightShaftValue = m_vLightShaftValue;
	ParamDesc.g_ScreenLightPos = m_pGameInstance->Get_LightPos();

	if (FAILED(m_pGameInstance->RTV_Compute_LightShaft(TEXT("Target_Occulsion"), TEXT("Target_Compute_LightShaft"), m_pLightShaftComputeShader, m_iOriginalViewportWidth, m_iOriginalViewportHeight, 1, &ParamDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightShaftX"))))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Compute_LightShaft"), m_pShader, "g_OccusionTexture")))
		return E_FAIL;

	m_pShader->Begin(11);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_LightShaftY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightShaftY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_LightShaftX"), m_pShader, "g_LightShaftXTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(12);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Fog()
{
	for (auto& pRenderObject : m_RenderObjects[RG_FOG])
	{
		if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Fog_Front"), true)))
			return E_FAIL;
		if (FAILED(pRenderObject->Render_Fog_Front()))
			return E_FAIL;

		if (FAILED(m_pGameInstance->End_MRT()))
			return E_FAIL;


		if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Fog_Back"), true)))
			return E_FAIL;

		if (FAILED(pRenderObject->Render_Fog_Back()))
			return E_FAIL;

		if (FAILED(m_pGameInstance->End_MRT()))
			return E_FAIL;

		//MRT_Fog_Final
		if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Fog_Final"), false)))
			return E_FAIL;

		if (FAILED(pRenderObject->Render_Fog_Final(m_pNoiseSRV)))
			return E_FAIL;

		if (FAILED(m_pGameInstance->End_MRT()))
			return E_FAIL;
		Safe_Release(pRenderObject);

	}

	m_RenderObjects[RG_FOG].clear();

	m_ParamDesc.g_vCamPosition = m_pGameInstance->Get_CamPosition();
	XMStoreFloat4x4(&m_ParamDesc.g_ProjMatrixInv, XMMatrixTranspose(XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ))));
	XMStoreFloat4x4(&m_ParamDesc.g_ViewMatrixInv, XMMatrixTranspose(XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW))));
	m_ParamDesc.g_fTime = m_fTime += 0.001f;

	if (m_bShadowStop)
	{
		if (FAILED(m_pGameInstance->RTV_Compute_Fog(TEXT("Target_Depth"), m_pNoiseSRV, TEXT("Target_LightShaftY"), TEXT("Target_Final"), TEXT("Target_RangeFog_Final"), TEXT("Target_Fog"), m_pFogComputeShader, m_iOriginalViewportWidth, m_iOriginalViewportHeight, 1, &m_ParamDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pGameInstance->RTV_Compute_Fog(TEXT("Target_Depth"), m_pNoiseSRV, TEXT("Target_LightShaftY"), TEXT("Target_Shadow_Final"), TEXT("Target_RangeFog_Final"), TEXT("Target_Fog"), m_pFogComputeShader, m_iOriginalViewportWidth, m_iOriginalViewportHeight, 1, &m_ParamDesc)))
			return E_FAIL;
	}

	m_pGameInstance->Clear_RTV(TEXT("Target_RangeFog_Final"));

	return S_OK;
}


HRESULT CRenderer::Render_MotionBlur_By_Velocity()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_MotionBlur_By_Velocity"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_MOTION_BLUR])
	{
		if (FAILED(pRenderObject->Render_Motion_Blur()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_MOTION_BLUR].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Water()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Water"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_WATER])
	{
		if (FAILED(pRenderObject->Render_Reflection()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_WATER].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Render_NonLight()
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	/* Shade */

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_MtrlSpecular"), m_pShader, "g_MtrlSpecular")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Roughness"), m_pShader, "g_RoughnessTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));

	m_pShader->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4));

	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred() //원래 Final에 있었음
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));

	m_pShader->Begin(6);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Shadow_Final()
{
	//Target_Shadow_Final
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow_Final"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), m_pShadowShader, "g_FinalTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShadowShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pShadowShader->Bind_SRV("g_CascadeShadowMapTexture", m_pShadowSRV)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShadowShader, "g_lightviewmatrix", "g_lightprojmatrix")))
		return E_FAIL;


	m_pShadowShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShadowShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));


	if (FAILED(m_pGameInstance->Bind_LightZ(m_pShadowShader)))
		return E_FAIL;

	if (FAILED(m_pShadowShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShadowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShadowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShadowShader->Begin(0);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLight"))))
		return E_FAIL;


	for (auto& pRenderObject : m_RenderObjects[RG_HIGHLIGHT])
	{
		if (FAILED(pRenderObject->Render_HighLight()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_HIGHLIGHT].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLightX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLight"), m_pShader, "g_HighLightTexture")))
		return E_FAIL;


	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(4);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLightY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLightX"), m_pShader, "g_HighLightXTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLight"), m_pShader, "g_HighLightTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	m_pShader->Begin(5);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Final() //원래 Deferred 에 있었음
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final_Last"))))
		return E_FAIL;
	if (!m_bFogStop)
	{
		if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Fog"), m_pShader, "g_FinalTexture")))
			return E_FAIL;

	}
	else if (m_bFogStop && m_bShadowStop)
	{
		if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), m_pShader, "g_FinalTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shadow_Final"), m_pShader, "g_FinalTexture")))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLightY"), m_pShader, "g_HighLightYTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowY"), m_pShader, "g_GlowYTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Distortion"), m_pShader, "g_DistortionTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_WeightBlend"), m_pShader, "g_WeightBlendTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_MotionBlur_By_Velocity"), m_pShader, "g_VelocityTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_bMotionBlurOnOff", &m_bMotionBlurOnOff, sizeof(bool))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_BloomY"), m_pShader, "g_BloomYTexture")))
		return E_FAIL;


	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(3);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();


	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Zoom_Blur()
{
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final_Last"), m_pShader, "g_Final_Last_Texture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_ZoomBlurCenter", &m_pGameInstance->Get_Zoom_Blur_Center(), sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_bZoomBlurOnOff", &m_bZoomBlurOnOff, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_bZoomBlurStrength", &m_fZoomBlurStrength, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShader->Bind_RawValue("g_bReverseScreenEffect", &m_bScreenReverseEffect_OnOff, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_ScreenReverseRadius", &m_fScreenReverseRadius, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_bReverseEnd", &m_bReverseEnd, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_fReverseEndStrength", &m_fReverseEndStrength, sizeof(_float))))
		return E_FAIL;




	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(16);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();


	return S_OK;
}


HRESULT CRenderer::Render_Blend()
{
	if(FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"), false)))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_WeightBlend()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_WeightBlend"))))
		return E_FAIL;


	for (auto& pRenderObject : m_RenderObjects[RG_WEIGHTBLEND])
	{
		if (FAILED(pRenderObject->Render_WeightBlend()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_WEIGHTBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	if (!m_RenderObjects[RG_UI].empty())
	{
		m_RenderObjects[RG_UI].sort([](CGameObject* pSour, CGameObject* pDest)->_bool
			{
				return static_cast<CGameObject*>(pSour)->Get_Transfrom()->Get_State_UIObj(CTransform::STATE_POSITION).z > static_cast<CGameObject*>(pDest)->Get_Transfrom()->Get_State_UIObj(CTransform::STATE_POSITION).z;
			});

	}


	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Font()
{
	for (auto& pRenderObject : m_RenderObjects[RG_FONT])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_FONT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_BloomBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BloomBegin"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_BLOOM])
	{
		if (FAILED(pRenderObject->Render_Bloom()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLOOM].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BloomX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BloomX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_BloomBegin"), m_pShader, "g_BloomBeginTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(14);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BloomY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_BloomY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_BloomX"), m_pShader, "g_BloomXTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(15);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Ready_Depth_Stencil_Buffer(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** ppOut)
{
	ID3D11Texture2D* pDepthStencilTexture = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀으 ㄴ백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌덜잉을 못함. */
	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 3;
	TextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = { DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0 };
	DSVDesc.Texture2DArray.ArraySize = 3;
	DSVDesc.Texture2DArray.MipSlice = 0;
	DSVDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &DSVDesc, &m_pShadowDSV)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, 0, 0 };
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 3;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	//SRVDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(pDepthStencilTexture, &srvDesc, &m_pShadowSRV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}


//float Clamp(float x, float minVal, float maxVal) {
//	return (x < minVal) ? minVal : (x > maxVal) ? maxVal : x;
//}
//
//float SmoothStep(float edge0, float edge1, float x) {
//	float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
//	return t * t * (3.0f - 2.0f * t);
//}
//
//float fade(float t) {
//	return t * t * t * (t * (t * 6 - 15) + 10);
//}
//
//float lerp(float a, float b, float t) {
//	return a + t * (b - a);
//}
//
//float grad(int hash, float x, float y, float z) {
//	int h = hash & 15;
//	float u = h < 8 ? x : y;
//	float v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
//	return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
//}
//
//void GeneratePermutationTable(int perm[512]) {
//	std::vector<int> p(256);
//
//	for (int i = 0; i < 256; ++i) {
//		p[i] = i;
//	}
//
//	std::srand((unsigned int)std::time(nullptr));
//
//	for (int i = 255; i > 0; --i) {
//		int j = std::rand() % (i + 1);
//		std::swap(p[i], p[j]);
//	}
//
//	for (int i = 0; i < 256; ++i) {
//		perm[i] = perm[i + 256] = p[i];
//	}
//}


HRESULT CRenderer::SetUp_ViewportDesc(_uint iWidth, _uint iHeight)
{
	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)iWidth;
	ViewPortDesc.Height = (_float)iHeight;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CRenderer::Add_NoiseTexture()
{
	ScratchImage scratch;
	TexMetadata metadata;

	HRESULT hr = LoadFromDDSFile(
		L"../Bin/Resources/Textures/3DTexture/3DNoiseTexture.dds",
		DDS_FLAGS_NONE,
		&metadata,
		scratch
	);

	if (FAILED(CreateTexture(m_pDevice, scratch.GetImages(), scratch.GetImageCount(), scratch.GetMetadata(), (ID3D11Resource**)&m_pNoiseTexture3D)))
		return E_FAIL;

	if (FAILED(CreateShaderResourceView(m_pDevice, scratch.GetImages(), scratch.GetImageCount(), scratch.GetMetadata(), &m_pNoiseSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Bind_NoiseTexture(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRV(pConstantName, m_pNoiseSRV);
}
#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Shadow"), m_pShader, m_pVIBuffer)))	
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Final"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightX"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_MotionBlur_By_Velocity"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_MotionBlur"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightShaftX"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightShaftY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GlowBegin"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GlowX"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GlowY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_BloomBegin"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_BloomX"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_BloomY"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Final_Last"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Fog_Front"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Fog_Back"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Fog_Final"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Water"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	return S_OK;
}
#endif

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)
			Safe_Release(pRenderObject);

		RenderObjects.clear();
	}

	Safe_Release(m_pShadowDSV);
	Safe_Release(m_pShadowSRV);
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);


	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pShadowShader);

	Safe_Release(m_pNoiseTexture3D);
	Safe_Release(m_pNoiseSRV);

	Safe_Release(m_pLightShaftComputeShader);
	Safe_Release(m_pFogComputeShader);
}

