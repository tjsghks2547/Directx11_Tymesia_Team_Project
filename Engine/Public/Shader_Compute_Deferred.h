#pragma once

#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Compute_Deferred final : public CShader
{
public:
	typedef struct LightShaftParams
	{
		_float2 g_ScreenLightPos;
		float Padding[2];
		_float4 g_LightShaftValue;
	}LIGHTSHAFTPARAMS;

	typedef struct FogParams
	{
		_float4x4 g_ProjMatrixInv; // 64
		_float4x4 g_ViewMatrixInv;// 64

		_float2 fHeightNoiseFactor;	//8
		_float2 fFogStartDistance; // x 거리 안개 거리 , y 높이 안개 거리, byte = 8

		_float4 g_vCamPosition; // 16
		_float4 g_FogColor;		// 16
		_float4 fFogFactor;		// 12

		float g_fTime; //4
		float Padding[3];
	}FOGPARAMS;

	enum Compute_Deferred_Type
	{
		DEFERRED_TYPE_LIGHTSHAFT,
		DEFERRED_TYPE_FOG,
	};
private:
	CShader_Compute_Deferred(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader_Compute_Deferred(const CShader_Compute_Deferred& Prototype);
	virtual ~CShader_Compute_Deferred() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements, void* pArg, Compute_Deferred_Type eType);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Create_ComputeShader(const _tchar* _pShaderFilePath, const _char* _pFunctionName, ID3D11Device* _pDevice);
	HRESULT Compute_Shader_LightShaft(_uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, ID3D11ShaderResourceView* pSRV, ID3D11UnorderedAccessView* pUAV, void* pArg);

	HRESULT Compute_Shader_Fog(_uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, ID3D11ShaderResourceView* pSRVDepth, ID3D11ShaderResourceView* pNoiseSRV, ID3D11ShaderResourceView* pSRVGodRay, ID3D11ShaderResourceView* pSRVFinal, ID3D11ShaderResourceView* pSRVRangeFogFinal, ID3D11UnorderedAccessView* pUAV, void* pArg);


private:
	HRESULT Create_ConstantBuffer_LightShaft(LightShaftParams* ParamDesc);
	HRESULT Create_Buffer_Fog(FogParams* ParamDesc);

private:
	ID3D11ComputeShader* m_pComputeShader = { nullptr };

	ID3D11Buffer* m_pSampleParmsBuffer = { nullptr };

	ID3D11SamplerState* m_pComputeShaderSampler = { nullptr }; // 샘플러 바인딩하지 않고 사용해서 생기는 경고 메시지 없애기 용도

public:
	static CShader_Compute_Deferred* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements, void* pArg, Compute_Deferred_Type eType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END