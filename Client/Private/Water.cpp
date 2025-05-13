#include "pch.h" 
#include "..\Public\Water.h"
#include "GameInstance.h"

#include "EnvironmentObject.h"

CWater::CWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CWater::CWater(const CWater& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CWater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	WATERINFO* pDesc = static_cast<WATERINFO*>(pArg);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->fPosition));
	m_fDullBlendFactor = pDesc->fDullBlendFactor;
	m_fWaveFrequency = pDesc->fWaveFrequency;

	_uint2		vViewportSize = m_pGameInstance->Get_ViewportSize();
	m_fAspect = vViewportSize.x / _float(vViewportSize.y);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pGameInstance->Set_WaterPos(_float2(XMVectorGetX(vPosition), XMVectorGetZ(vPosition)));

	return S_OK;
}

void CWater::Priority_Update(_float fTimeDelta)
{
}

void CWater::Update(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	_matrix matCam = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);
	_vector vCamLook = matCam.r[2];
	_vector vCamPosition = matCam.r[3];
	_vector vCamRight = matCam.r[0];
	_vector vCamUp = matCam.r[1];

	vCamLook = vCamPosition + vCamLook;

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	float reflectionCamYCoord = -XMVectorGetY(vCamPosition) + 2 * XMVectorGetY(vPos);
	_vector reflectionCamPos = XMVectorSet(XMVectorGetX(vCamPosition), reflectionCamYCoord, XMVectorGetZ(vCamPosition), 1.f);

	float reflectionTargetYCoord = -XMVectorGetY(vCamLook) + 2 * XMVectorGetY(vPos);
	_vector reflectionCamTarget = XMVectorSet(XMVectorGetX(vCamLook), reflectionTargetYCoord, XMVectorGetZ(vCamLook), 1.f);

	_vector forward = reflectionCamTarget - reflectionCamPos;
	_vector reflectionCamUp;
	reflectionCamUp = XMVector3Cross(vCamRight, forward);

	float yCam = XMVectorGetY(vCamPosition);
	float yWater = XMVectorGetY(vPos);
	float yReflected = reflectionCamYCoord;

	XMStoreFloat4x4(&m_matReflectionView, XMMatrixLookAtLH(reflectionCamPos, reflectionCamTarget, reflectionCamUp));

	m_pGameInstance->Set_Reflection_Transform(CPipeLine::D3DTS_VIEW, XMLoadFloat4x4(&m_matReflectionView));
	m_pGameInstance->Set_Reflection_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.f), m_fAspect, 0.1f, 800.f));

	_float4 vCamPos = m_pGameInstance->Get_Reflection_CamPosition(); // 반사 카메라 행렬 계산.. Priority에서 하는게 맞을듯함
}

void CWater::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_WATER, this);
}

HRESULT CWater::Render()
{
	/* 네비이게이션 버퍼 확인용으로 잠깐 뺌*/

	return S_OK;
}

HRESULT CWater::Render_Reflection()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CWater::Ready_Components(void* _pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(_pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	/* Com_Texture */
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Texture_Water"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Noise_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Diffuse"),
		TEXT("Com_Ripple_Texture"), reinterpret_cast<CComponent**>(&m_pRippleTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Water"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_VIBuffer_Water"),
		TEXT("Com_VIBuffer_Terrain"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse_Copy"), m_pShaderCom, "g_RefractionTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Reflection"), m_pShaderCom, "g_ReflectionTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NormalTexture", 3)))
		return E_FAIL;

	if (FAILED(m_pRippleTextureCom->Bind_ShaderResource(m_pShaderCom, "g_RippleTexture", 43)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ReflectionView", &m_pGameInstance->Get_Reflection_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WaveSpeed", &m_fWaveSpeed, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WaveFrequency", &m_fWaveFrequency, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WaveAmplitude", &m_fWaveAmplitude, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WaveHeight", &m_fWaveHeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_WindDirection", &m_fWindDirection, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_FresnelMode", &m_iFresnelMode, sizeof(_int))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("xDrawMode", &m_iDrawMode, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_dullBlendFactor", &m_fDullBlendFactor, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("specPerturb", &m_fSpecPerturb, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("specPower", &m_fSpecPower, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RippleSRV(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWater::Bind_ShaderResources_Reflection()
{
	return S_OK;
}

CWater* CWater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWater* pInstance = new CWater(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWater::Clone(void* pArg)
{
	CWater* pInstance = new CWater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CWater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWater::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRippleTextureCom);
	//Safe_Release(m_pTextureORMCom);
}
