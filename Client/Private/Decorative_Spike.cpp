#include "pch.h"
#include "Decorative_Spike.h"
#include "GameInstance.h"

CDecorative_Spike::CDecorative_Spike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CDecorative_Spike::CDecorative_Spike(const CDecorative_Spike& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CDecorative_Spike::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDecorative_Spike::Initialize(void* pArg)
{
	CDecorative_Spike::DECORATIVE_SPIKE_DESC* pDesc = static_cast<DECORATIVE_SPIKE_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_pRender = pDesc->pRender;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMConvertToRadians(pDesc->pRotate_Degree->x), XMConvertToRadians(pDesc->pRotate_Degree->y), XMConvertToRadians(pDesc->pRotate_Degree->z));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(pDesc->pPosition));

	m_fMaxScale = { 0.25f,0.4f,0.25f };
	m_fMyScale = { 0.25f,0.001f,0.25f };


	_vector vPos = { 1.f,1.5f,1.f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	return S_OK;
}

void CDecorative_Spike::Priority_Update(_float fTimeDelta)
{
	if (*m_pRender)
	{
		m_fLinear += fTimeDelta * 0.2f;
		if (m_fLinear >= 1.f)
			m_fLinear = 1.f;
	}
	else
		m_fLinear = 0.f;

	_float3 fNewScale = {};
	_vector vNewScale = XMVectorLerp(XMLoadFloat3(&m_fMyScale), XMLoadFloat3(&m_fMaxScale), m_fLinear);
	XMStoreFloat3(&fNewScale, vNewScale);
	m_pTransformCom->Scaling(fNewScale);

}

void CDecorative_Spike::Update(_float fTimeDelta)
{
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
		SocketMatrix *
		XMLoadFloat4x4(m_pParentWorldMatrix)
	);
}

void CDecorative_Spike::Late_Update(_float fTimeDelta)
{
	if (*m_pRender)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
	}
}

HRESULT CDecorative_Spike::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
			return E_FAIL;

		m_pShaderCom->Begin(21);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CDecorative_Spike::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Spike"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Emissive"),
		TEXT("Com_Emissive"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDecorative_Spike::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CDecorative_Spike* CDecorative_Spike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDecorative_Spike* pInstance = new CDecorative_Spike(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CDecorative_Spike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDecorative_Spike::Clone(void* pArg)
{
	CDecorative_Spike* pInstance = new CDecorative_Spike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDecorative_Spike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDecorative_Spike::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
