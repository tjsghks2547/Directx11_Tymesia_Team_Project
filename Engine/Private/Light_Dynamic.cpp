#include "..\Public\Light_Dynamic.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"	
#include "Engine_struct.h"
#include "Transform.h"


CLight_Dynamic::CLight_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CLight_Dynamic::Initialize(const LIGHT_DESC& LightDesc, CTransform* pTransform)
{
	m_LightDesc = LightDesc;

	if (pTransform == nullptr)
		return E_FAIL;

	m_pTransform = pTransform;
	Safe_AddRef(m_pTransform);

	return S_OK;
}

void CLight_Dynamic::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	_uint iPassIndex = { 2 };

	if (LIGHT_DESC::TYPE_POINT == m_LightDesc.eType)
	{
		iPassIndex = 2;
		if (nullptr != m_pTransform)
			XMStoreFloat4(&m_LightDesc.vPosition, m_pTransform->Get_State(CTransform::STATE_POSITION));

		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return;

		if (FAILED(pShader->Bind_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float))))
			return;
	}

	else if (LIGHT_DESC::TYPE_SPOT == m_LightDesc.eType)
	{
		iPassIndex = 10;

		if (FAILED(pShader->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4))))
			return;

		if (FAILED(pShader->Bind_RawValue("g_fRange", &m_LightDesc.fRange, sizeof(_float))))
			return;

		if (FAILED(pShader->Bind_RawValue("g_fSpot", &m_LightDesc.fSpot, sizeof(_float))))
			return;

	}

	if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return;

	if (FAILED(pShader->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return;

	if (FAILED(pShader->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return;


	pShader->Begin(iPassIndex);

	pVIBuffer->Bind_InputAssembler();
	pVIBuffer->Render();

}

_bool CLight_Dynamic::Delete_Light(CTransform* pTransform)
{
	if (pTransform == m_pTransform)
		return true;

	return false;
}

CLight_Dynamic* CLight_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& LightDesc, CTransform* pTransform)
{
	CLight_Dynamic* pInstance = new CLight_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize(LightDesc, pTransform)))
	{
		MSG_BOX("Failed to Created : CLight_Dynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLight_Dynamic::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pTransform);
}
