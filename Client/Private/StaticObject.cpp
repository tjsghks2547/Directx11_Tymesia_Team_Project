#include "pch.h"
#include "StaticObject.h"
#include "GameInstance.h"

CStaticObject::CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CObject(pDevice, pContext)
{
}

CStaticObject::CStaticObject(const CStaticObject& Prototype)
	: CObject(Prototype)
{
}

HRESULT CStaticObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iObjectType = OBJECT_DEFAULT;

	return S_OK;
}

void CStaticObject::Priority_Update(_float fTimeDelta)
{

}

void CStaticObject::Update(_float fTimeDelta)
{

}

void CStaticObject::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fFrustumRadius))
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	}

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CStaticObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (*m_pDitheringOnOff)
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
				return E_FAIL;

			m_pShaderCom->Begin(m_iPassIndex);
			m_pModelCom->Render(i);
		}
	}
	else
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
				return E_FAIL;

			if (m_iPassIndex == 0)
			{
				m_pShaderCom->Begin(19);
			}
			else if (m_iPassIndex == 10)
			{
				m_pShaderCom->Begin(20);
			}

			m_pModelCom->Render(i);
		}
	}

	return S_OK;
}

HRESULT CStaticObject::Render_Shadow()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{

		m_pShaderCom->Begin(2);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CStaticObject::Render_Motion_Blur()
{
	if (FAILED(Bind_Motion_Blur_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(5);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CStaticObject::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	string strComponentName = "Prototype_Component_Model_";
	strComponentName += m_szName;

	_tchar		szComponentName[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), static_cast<_int>(strlen(strComponentName.c_str())), szComponentName, MAX_PATH);

	/* Com_Model */
	if (FAILED(__super::Add_Component(m_eCurrentLevel, szComponentName, TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Bind_ShaderResources()
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

	return S_OK;
}

HRESULT CStaticObject::Bind_Motion_Blur_ShaderResources()
{
	/* 물체는 멈춰있기 때문에 월드 매트릭스를 바인딩 안해도 되겠다. */
	if (FAILED(m_pShaderCom->Bind_Matrix("g_PreViewMatrix", &m_pGameInstance->Get_PreTransform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CStaticObject* CStaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStaticObject* pInstance = new CStaticObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticObject::Clone(void* pArg)
{
	CStaticObject* pInstance = new CStaticObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free()
{
	__super::Free();
}
