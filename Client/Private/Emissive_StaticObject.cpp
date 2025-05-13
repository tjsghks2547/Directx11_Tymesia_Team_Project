#include "pch.h"

#include "Emissive_StaticObject.h"

#include "GameInstance.h"

CEmissive_StaticObject::CEmissive_StaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CObject(pDevice, pContext)
{
}

CEmissive_StaticObject::CEmissive_StaticObject(const CEmissive_StaticObject& Prototype)
	: CObject(Prototype)
{
}

HRESULT CEmissive_StaticObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEmissive_StaticObject::Initialize(void* pArg)
{
	EmissiveObject* pDesc = static_cast<EmissiveObject*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iObjectType = OBJECT_EMISSIVE;

	m_fEmissivePower = pDesc->fEmissivePower;

	return S_OK;
}

void CEmissive_StaticObject::Priority_Update(_float fTimeDelta)
{
}

void CEmissive_StaticObject::Update(_float fTimeDelta)
{
}

void CEmissive_StaticObject::Late_Update(_float fTimeDelta)
{
	if(m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fFrustumRadius))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

}

HRESULT CEmissive_StaticObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (*m_pDitheringOnOff)
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
				return E_FAIL;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 1)))
				return E_FAIL;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_RoughnessTexture", 2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
				return E_FAIL;

			m_pShaderCom->Begin(24);
			m_pModelCom->Render(i);
		}
	}
	else
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
				return E_FAIL;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 1)))
				return E_FAIL;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_RoughnessTexture", 2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
				return E_FAIL;

			m_pShaderCom->Begin(25);
			m_pModelCom->Render(i);
		}
	}

	return S_OK;
}

HRESULT CEmissive_StaticObject::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	string strComponentName = "Prototype_Component_Model_";
	strComponentName += m_szName;

	_tchar		szComponentName[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), (_uint)strlen(strComponentName.c_str()), szComponentName, MAX_PATH);

	/* Com_Model */
	if (FAILED(__super::Add_Component(m_eCurrentLevel, szComponentName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(m_eCurrentLevel, TEXT("Prototype_Component_Texture_Gem_Mtrl"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	return S_OK;
}

HRESULT CEmissive_StaticObject::Bind_ShaderResources()
{
	if (m_pGameInstance->Get_Current_Level_Index() >= 3)
	{
		m_pCamera = m_pGameInstance->Get_GameObject_To_Layer(m_pGameInstance->Get_Current_Level_Index(), TEXT("Layer_Camera"), "Camera_Free");

		if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldCamPos", &m_pCamera->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), sizeof(_float4))))
			return E_FAIL;
	}

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float4 vColor = _float4(1.f, 0.f, 0.018f, 1.f);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vEmissiveColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fEmissivePower", &m_fEmissivePower, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CEmissive_StaticObject* CEmissive_StaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEmissive_StaticObject* pInstance = new CEmissive_StaticObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEmissive_StaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEmissive_StaticObject::Clone(void* pArg)
{
	CEmissive_StaticObject* pInstance = new CEmissive_StaticObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CEmissive_StaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEmissive_StaticObject::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
