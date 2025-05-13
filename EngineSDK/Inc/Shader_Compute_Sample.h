#pragma once

#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Compute_Sample final : public CShader
{
public:
	typedef struct LightShaftParams
	{
		_float2 g_ScreenLightPos;
		float Padding[2];
		_float4 g_LightShaftValue;
	}LIGHTSHAFTPARAMS;
private:
	CShader_Compute_Sample(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader_Compute_Sample(const CShader_Compute_Sample& Prototype);
	virtual ~CShader_Compute_Sample() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements, void* pArg);
	virtual HRESULT Initialize(void* pArg);

public:
	HRESULT Create_ComputeShader(const _tchar* _pShaderFilePath, const _char* _pFunctionName, ID3D11Device* _pDevice);
	HRESULT Compute_Shader(_uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ, ID3D11ShaderResourceView* pSRV, ID3D11UnorderedAccessView* pUAV, void* pArg);

private:
	HRESULT Create_ConstantBuffer(LightShaftParams* ParamDesc);

private:
	ID3D11ComputeShader* m_pComputeShader = { nullptr };

	ID3D11Buffer* m_pSampleParmsBuffer = { nullptr };

	D3D11_SUBRESOURCE_DATA			m_initData = {};

public:
	static CShader_Compute_Sample* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements, void* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END