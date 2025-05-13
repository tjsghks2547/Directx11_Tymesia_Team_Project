#include "..\Public\Shader_Compute.h"

CShader_Compute::CShader_Compute(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CShader { pDevice, pContext }
{

}

CShader_Compute::CShader_Compute(const CShader_Compute & Prototype)
	: CShader( Prototype )
	, m_pComputeShader(Prototype.m_pComputeShader)
	, m_pComputeShader_Reset(Prototype.m_pComputeShader_Reset)
{
	Safe_AddRef(m_pComputeShader);
	Safe_AddRef(m_pComputeShader_Reset);
}

HRESULT CShader_Compute::Initialize_Prototype(const _tchar * pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements)
{
	_uint			iHlslFlag = {};

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob*		pErrMsg = { nullptr };	

	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, &pErrMsg)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc{};

	pTechnique->GetDesc(&TechniqueDesc);
	
	m_iNumPasses = TechniqueDesc.Passes;

	for (_uint i = 0; i < m_iNumPasses; i++)
	{
		ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(i);
		ID3D11InputLayout*		pInputLayout = { nullptr };

		D3DX11_PASS_DESC		PassDesc{};

		pPass->GetDesc(&PassDesc);



		if (FAILED(m_pDevice->CreateInputLayout(pVertexElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayout.push_back(pInputLayout);

	}

	Create_ComputeShader(pShaderFilePath, _pFunctionName, m_pDevice);

	return S_OK;
}

HRESULT CShader_Compute::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CShader_Compute::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayout[iPassIndex]);

	m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader_Compute::Bind_RawValue(const _char * pConstantName, const void * pData, _uint iSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 내가 지정한 셰이더 내에 선언된 전역변수의 핸들을 얻어온다. */
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iSize);
	
}



HRESULT CShader_Compute::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	/* 내가 지정한 셰이더 내에 선언된 전역변수의 핸들을 얻어온다. */
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable*		pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;	

	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatrix));	
}

HRESULT CShader_Compute::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL; 

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL; 

	return pMatrixVariable->SetMatrixArray(reinterpret_cast<const _float*>(pMatrix), 0 , iNumMatrices);
}

HRESULT CShader_Compute::Bind_SRV(const _char * pConstantName, ID3D11ShaderResourceView * pSRV)
{
	/* 내가 지정한 셰이더 내에 선언된 전역변수의 핸들을 얻어온다. */
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader_Compute::Bind_SRV_Compute(ID3D11ShaderResourceView* _pSRV, ID3D11UnorderedAccessView* _pUAV)
{
	ID3DX11EffectShaderResourceVariable* Input = m_pEffect->GetVariableByName("gInput_Point")->AsShaderResource();
	ID3DX11EffectUnorderedAccessViewVariable* Output = m_pEffect->GetVariableByName("gOutput_Point")->AsUnorderedAccessView();
	
	Input->SetResource(_pSRV);
	Output->SetUnorderedAccessView(_pUAV);

	

	m_pContext->IASetInputLayout(m_InputLayout[0]);

	m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader_Compute::Create_ComputeShader(const _tchar* _pShaderFilePath, const _char* _pFunctionName, ID3D11Device* _pDevice)
{
	if (_pDevice == nullptr)
		return E_FAIL;
	
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	//dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

	const D3D_SHADER_MACRO defines[] =
	{
		"USE_STRUCTURED_BUFFERS", "1",
//#ifdef USE_STRUCTURED_BUFFERS
//		"USE_STRUCTURED_BUFFERS", "1",
//#endif
		nullptr, nullptr
	};

	_char* pProfile = (m_pDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

	ID3DBlob* pError = { nullptr };
	ID3DBlob* pBlob = { nullptr };
	if (FAILED(D3DCompileFromFile(TEXT("../../EngineSDK/Hlsl/Shader_Compute.hlsl"), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, _pFunctionName, pProfile, dwShaderFlags, 0, &pBlob, &pError)))
	{
		if (pError != nullptr)
		{
			OutputDebugStringA((_char*)pError->GetBufferPointer());
			_char* asd = (_char*)pError->GetBufferPointer();
			int a = 10;

		}


		Safe_Release(pError);
		Safe_Release(pBlob);

		return E_FAIL;
	}

	if(FAILED(m_pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pComputeShader)))
		return E_FAIL;

	Safe_Release(pError);
	Safe_Release(pBlob);


	//Reset용도 모든 Shader는 이놈을 들고있을것임
	if (FAILED(D3DCompileFromFile(TEXT("../../EngineSDK/Hlsl/Shader_Compute.hlsl"), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CSMain_Particle_Reset", pProfile, dwShaderFlags, 0, &pBlob, &pError)))
	{
		if (pError != nullptr)
		{
			OutputDebugStringA((_char*)pError->GetBufferPointer());
			_char* asd = (_char*)pError->GetBufferPointer();
			int a = 10;

		}


		Safe_Release(pError);
		Safe_Release(pBlob);

		return E_FAIL;
	}

	if (FAILED(m_pDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pComputeShader_Reset)))
		return E_FAIL;

	Safe_Release(pError);
	Safe_Release(pBlob);

#ifdef _DEBUG
	(m_pComputeShader)->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(_pFunctionName), _pFunctionName);
	(m_pComputeShader_Reset)->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA("CSMain_Particle_Reset"), "CSMain_Particle_Reset");
#endif // _DEBUG


	return S_OK;
}

CShader_Compute * CShader_Compute::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pShaderFilePath, const _char* _pFunctionName, const D3D11_INPUT_ELEMENT_DESC* pVertexElements, _uint iNumElements)
{
	CShader_Compute*	pInstance = new CShader_Compute(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, _pFunctionName, pVertexElements, iNumElements)))
	{
		MSG_BOX("Failed To Created : CShader_Compute");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CShader_Compute::Clone(void * pArg)
{
	CShader_Compute*	pInstance = new CShader_Compute(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CShader_Compute");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CShader_Compute::Free()
{
	__super::Free();

	Safe_Release(m_pComputeShader);
	Safe_Release(m_pComputeShader_Reset);
}
