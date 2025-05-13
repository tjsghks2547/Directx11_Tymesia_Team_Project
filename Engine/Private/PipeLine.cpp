#include "..\Public\PipeLine.h"



CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformationMatrices[i], XMMatrixIdentity());
		XMStoreFloat4x4(&m_TransformationInverseMatrices[i], XMMatrixIdentity());

		XMStoreFloat4x4(&m_PreTransformationMatrices[i], XMMatrixIdentity());
		XMStoreFloat4x4(&m_PreTransformationInverseMatrices[i], XMMatrixIdentity());
	}

	m_vCamPosition = _float4(0.f, 0.f, 0.f, 1.f);

	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_ReflectionTransformationMatrices[i], XMMatrixIdentity());
		XMStoreFloat4x4(&m_ReflectionTransformationInverseMatrices[i], XMMatrixIdentity());
	}

	m_vReflectionCamPosition = _float4(0.f, 0.f, 0.f, 1.f);

	return S_OK;
}

void CPipeLine::Priority_Update()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_PreTransformationMatrices[i], XMLoadFloat4x4(&m_TransformationMatrices[i]));
	}

	XMStoreFloat4(&m_vCamPosition, XMLoadFloat4x4(&m_PreTransformationInverseMatrices[D3DTS_VIEW]).r[3]);

}

void CPipeLine::Update()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_TransformationInverseMatrices[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformationMatrices[i])));
	}

	XMStoreFloat4(&m_vCamPosition, XMLoadFloat4x4(&m_TransformationInverseMatrices[D3DTS_VIEW]).r[3]);

	for (size_t i = 0; i < D3DTS_END; i++)
	{
		XMStoreFloat4x4(&m_ReflectionTransformationInverseMatrices[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ReflectionTransformationMatrices[i])));
	}

	XMStoreFloat4(&m_vReflectionCamPosition, XMLoadFloat4x4(&m_ReflectionTransformationInverseMatrices[D3DTS_VIEW]).r[3]);
}

CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CPipeLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeLine::Free()
{
	__super::Free();

}


