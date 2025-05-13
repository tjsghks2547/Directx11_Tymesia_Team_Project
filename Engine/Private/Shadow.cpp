#include "Shadow.h"
#include "Shader.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "Transform.h"

CShadow::CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CShadow::Bind_LightPos(CShader* pShader, const _char* pConstantName)
{
	_float4x4 vCamViewMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW);
	_float4x4 vCamProjMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ);

	_vector vLightPos = XMLoadFloat4(&m_LightPos);

	vLightPos = XMVector3TransformCoord(vLightPos, XMLoadFloat4x4(&vCamViewMatrix) * XMLoadFloat4x4(&vCamProjMatrix));

	vLightPos = XMVectorSet(vLightPos.m128_f32[0] * 0.5f + 0.5f, vLightPos.m128_f32[1] * -0.5f + 0.5f, vLightPos.m128_f32[2], 1.f);

	_float4 vLightViewProjPos;

	XMStoreFloat4(&vLightViewProjPos, vLightPos);

	return pShader->Bind_RawValue(pConstantName, &vLightViewProjPos, sizeof(_float4));
}


HRESULT CShadow::Bind_LightDir(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_RawValue(pConstantName, &XMLoadFloat4(&m_LightDir), sizeof(_float4));
}


HRESULT CShadow::Bind_LightProjDir(CShader* pShader, const _char* pConstantName)
{
	_float4x4 vLightViewMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW);
	_float4x4 vLightProjMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ);

	_matrix lightProjectionMatrix = XMLoadFloat4x4(&vLightViewMatrix) * XMLoadFloat4x4(&vLightProjMatrix);

	_vector vLightDir = XMLoadFloat4(&m_LightDir);

	vLightDir = XMVector3TransformNormal(XMVector3Normalize(vLightDir), lightProjectionMatrix);

	_float2 vLightViewProjPos = { XMVectorGetX(vLightDir), -XMVectorGetY(vLightDir) };

	return pShader->Bind_RawValue(pConstantName, &vLightViewProjPos, sizeof(_float2));
}

_float2 CShadow::Get_LightPos()
{
	_float4x4 vCamViewMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW);
	_float4x4 vCamProjMatrix = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ);

	_vector vLightPos = XMLoadFloat4(&m_LightPos);

	vLightPos = XMVector3TransformCoord(vLightPos, XMLoadFloat4x4(&vCamViewMatrix) * XMLoadFloat4x4(&vCamProjMatrix));

	vLightPos = XMVectorSet(vLightPos.m128_f32[0] * 0.5f + 0.5f, vLightPos.m128_f32[1] * -0.5f + 0.5f, vLightPos.m128_f32[2], 1.f);

	_float2 vLightViewProjPos = { XMVectorGetX(vLightPos), XMVectorGetY(vLightPos) };

	return vLightViewProjPos;
}




HRESULT CShadow::Initialize()
{
	return S_OK;
}

HRESULT CShadow::SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform)
{
	m_pPlayerTransform = pTransform;
	Safe_AddRef(m_pPlayerTransform);

	XMStoreFloat4(&m_LightPos, vEye);

	XMStoreFloat4(&m_LightDir, XMVector3Normalize(vAt - vEye));

	XMStoreFloat4x4(&m_LightViewMatrix, XMMatrixLookAtLH(vAt, vEye, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&m_LightProjMatrix, XMMatrixPerspectiveFovLH(fLightAngle, fAspect, fNear, fFar));

	_matrix camInv = matInvCam;

	float fov = XMVectorGetX(vCamInfo);
	float ar = XMVectorGetY(vCamInfo);
	float nearZ = XMVectorGetZ(vCamInfo);
	float farZ = XMVectorGetW(vCamInfo);

	// 카메라의 시야각을 이용한 수직 시야각
	float tanHalfVFov = tanf(XMConvertToRadians(fov / 2.f));

	float fHalfFov = XMConvertToDegrees(tanHalfVFov);

	// 수직 시야각을 이용한 수평 시야각
	float tanHalfHFov = tanHalfVFov * ar;

	// 카메라의 뷰 클립 공간
	/*m_cascadeEnd[0] = nearZ;
	m_cascadeEnd[1] = 30.f;
	m_cascadeEnd[2] = 90.f;
	m_cascadeEnd[3] = farZ;*/
	for (int i = 0; i < 3; i++)
	{
		float logDepth = nearZ * powf(farZ / nearZ, (float)(i + 1) / 3);
		float uniformDepth = nearZ + (farZ - nearZ) * (float)(i + 1) / 3;
		m_cascadeEnd[i + 1] = 0.9f * logDepth + (1.0f - 0.9f) * uniformDepth;
	}

	for (size_t i = 0; i < 3; ++i)
	{
		float xn = m_cascadeEnd[i] * tanHalfHFov;
		float xf = m_cascadeEnd[i + 1] * tanHalfHFov;
		float yn = m_cascadeEnd[i] * tanHalfVFov;
		float yf = m_cascadeEnd[i + 1] * tanHalfVFov;

		_vector frustumCorners[8] =
		{
			//near Face
			{xn,yn,m_cascadeEnd[i],1.0f},
			{-xn,yn,m_cascadeEnd[i],1.0f},
			{xn,-yn,m_cascadeEnd[i],1.0f},
			{-xn,-yn,m_cascadeEnd[i],1.0f},
			//far Face
			{xf,yf,m_cascadeEnd[i + 1],1.0f},
			{-xf,yf,m_cascadeEnd[i + 1],1.0f},
			{xf,-yf,m_cascadeEnd[i + 1],1.0f},
			{-xf,-yf,m_cascadeEnd[i + 1],1.0f}
		};

		for (size_t l = 0; l < 8; ++l)
			XMStoreFloat4(&m_cascadeFrustum[i][l], frustumCorners[l]);

		_vector centerPos = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		for (uint32_t j = 0; j < 8; ++j)
		{
			frustumCorners[j] = XMVector3TransformCoord(frustumCorners[j], camInv);
			centerPos += frustumCorners[j];
		}

		centerPos /= 8.0f;

		float radius = 0.0f;
		for (uint32_t k = 0; k < 8; ++k)
		{
			float distance = XMVectorGetX(XMVector3Length(frustumCorners[k] - centerPos));
			radius = max(radius, distance);
		}

		radius = std::ceil(radius * 16.0f) / 16.0f;

		_vector maxExtents = XMVectorSet(radius, radius, radius, 1.f);
		_vector minExtents = -maxExtents;

		_vector vDir = XMLoadFloat4(&m_LightDir);

		_float3 shadowCamPos;
		XMStoreFloat3(&shadowCamPos, centerPos + XMVector3Normalize(vDir) * (XMVectorGetZ(minExtents)));

		_vector vEyePos = XMVectorSetW(XMLoadFloat3(&shadowCamPos), 1.f);

		_matrix lightMatrix = XMMatrixLookAtLH(vEyePos, centerPos, XMVectorSet(0.f, 1.f, 0.f, 0.f));

		XMStoreFloat4x4(&m_shadowOrthoView[i], lightMatrix);

		_vector cascadeExtents = maxExtents - minExtents;

		XMStoreFloat4x4(&m_shadowOrthoProj[i], lightMatrix * XMMatrixOrthographicOffCenterLH(XMVectorGetX(minExtents), XMVectorGetX(maxExtents), XMVectorGetY(minExtents), XMVectorGetY(maxExtents), 0.f, XMVectorGetZ(maxExtents) * 2.f));
	}

	return S_OK;
}

void CShadow::Update()
{

	for (int i = 0; i < 3; ++i)
	{
		_vector vclip = XMVector3TransformCoord(XMVectorSet(0.0f, 0.0f, m_cascadeEnd[i + 1], 1.0f), m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));
		m_CascadeEndCliSpaceZ[i] = XMVectorGetZ(vclip);
	}

	_matrix matView = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);

	if (m_pPlayerTransform != nullptr)
	{
		_vector vPlayerPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		for (size_t i = 0; i < 3; ++i)
		{
			_vector centerPos = XMVectorSet(0.f, 0.f, 0.f, 0.f);

			_vector frustumCorners[8] =
			{
			};

			for (size_t l = 0; l < 8; ++l)
				frustumCorners[l] = XMLoadFloat4(&m_cascadeFrustum[i][l]);

			for (uint32_t j = 0; j < 8; ++j)
			{
				frustumCorners[j] = XMVector3TransformCoord(frustumCorners[j], matView);
				centerPos += frustumCorners[j];
			}

			centerPos /= 8.0;

			float radius = 0.0f;
			for (uint32_t j = 0; j < 8; ++j)
			{
				float distance = XMVectorGetX(XMVector3Length(frustumCorners[j] - centerPos));
				radius = max(radius, distance);
			}

			float fTexelSize = (radius * 2.f) / g_iMaxWidth;

			radius = std::ceil(radius * 16.0f) / 16.0f;

			// using radius ,  we made aabb box
			_vector maxExtents = XMVectorSet(radius, radius, radius, 1.f);
			_vector minExtents = -maxExtents;

			_vector vDir = XMLoadFloat4(&m_LightDir);

			_float3 shadowCamPos;
			XMStoreFloat3(&shadowCamPos, centerPos + XMVector3Normalize(vDir) * (XMVectorGetZ(minExtents)));

			_vector vEyePos = XMVectorSetW(XMLoadFloat3(&shadowCamPos), 1.f);

			_matrix lightMatrix = XMMatrixLookAtLH(vEyePos, centerPos, XMVectorSet(0.f, 1.f, 0.f, 0.f));

			XMStoreFloat4x4(&m_shadowOrthoView[i], lightMatrix);

			_vector cascadeExtents = maxExtents - minExtents;

			_matrix lightOrthoMatrix = XMMatrixOrthographicOffCenterLH(XMVectorGetX(minExtents), XMVectorGetX(maxExtents), XMVectorGetY(minExtents), XMVectorGetY(maxExtents), 0.1f, XMVectorGetZ(maxExtents) * 1.5f);

			_vector shadowOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			shadowOrigin = XMVector3TransformCoord(shadowOrigin, lightMatrix * lightOrthoMatrix);
			shadowOrigin = shadowOrigin * ((_float)g_iMaxHeight / 2.0f);

			_vector roundedOrigin = XMVectorSet(round(XMVectorGetX(shadowOrigin)), round(XMVectorGetY(shadowOrigin)), round(XMVectorGetZ(shadowOrigin)), round(XMVectorGetW(shadowOrigin)));
			_vector roundOffset = roundedOrigin - shadowOrigin;

			roundOffset = roundOffset * 2.0f / (_float)g_iMaxHeight;
			roundOffset.m128_f32[2] = 0.0f;
			roundOffset.m128_f32[3] = 0.0f;

			lightOrthoMatrix.r[3] += roundOffset;
			XMStoreFloat4x4(&m_shadowOrthoProj[i], lightMatrix * lightOrthoMatrix);

			//XMStoreFloat4x4(&m_shadowOrthoProj[i], lightMatrix * XMMatrixOrthographicOffCenterLH(XMVectorGetX(minExtents), XMVectorGetX(maxExtents), XMVectorGetY(minExtents), XMVectorGetY(maxExtents), 0.1f, XMVectorGetZ(maxExtents) * 1.5f));
		}
	}
}

HRESULT CShadow::Bind_ViewMatrix(CShader* pShader, const _char* pConstantName)
{
	if (FAILED(pShader->Bind_Matrices(pConstantName, &m_shadowOrthoView[0], 3)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShadow::Bind_ProjMatrix(CShader* pShader, const _char* pConstantName)
{

	if (FAILED(pShader->Bind_Matrices(pConstantName, &m_shadowOrthoProj[0], 3)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShadow::Delete_Shadow(CTransform* pTransform)
{
	if (m_pPlayerTransform == pTransform)
	{
		Safe_Release(m_pPlayerTransform);
		m_pPlayerTransform = nullptr;
	}

	return S_OK;
}

HRESULT CShadow::Bind_LightZ(CShader* pShader)
{
	if (FAILED(pShader->Bind_RawValue("g_LightEndClipSpaceZ1", &m_CascadeEndCliSpaceZ[0], sizeof(_float))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_LightEndClipSpaceZ2", &m_CascadeEndCliSpaceZ[1], sizeof(_float))))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_LightEndClipSpaceZ3", &m_CascadeEndCliSpaceZ[2], sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CShadow* CShadow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShadow* pInstance = new CShadow(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CShadow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShadow::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}