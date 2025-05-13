#pragma once

#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CShader_Compute final : public CShader
{
private:
	CShader_Compute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader_Compute(const CShader_Compute& Prototype);
	virtual ~CShader_Compute() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg);
public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iSize);
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices);
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);
	//ID3DX11Effect* Get_Effect() { return m_pEffect; }
	ID3D11ComputeShader* Get_ComputeShader() { return m_pComputeShader; }
	ID3D11ComputeShader* Get_ComputeShader_Reset() { return m_pComputeShader_Reset; }

	HRESULT Bind_SRV_Compute(ID3D11ShaderResourceView* _pSRV, ID3D11UnorderedAccessView* _pUAV);

	HRESULT Create_ComputeShader(const _tchar* _pShaderFilePath, const _char* _pFunctionName, ID3D11Device* _pDevice);

private:
	ID3D11ComputeShader*			m_pComputeShader = { nullptr };
	ID3D11ComputeShader*			m_pComputeShader_Reset = { nullptr };

public:
	static CShader_Compute* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END