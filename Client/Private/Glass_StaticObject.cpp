#include "pch.h"

#include "Glass_StaticObject.h"

#include "GameInstance.h"

CGlass_StaticObject::CGlass_StaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CObject(pDevice, pContext)
{
}

CGlass_StaticObject::CGlass_StaticObject(const CGlass_StaticObject& Prototype)
	: CObject(Prototype)
{
}

HRESULT CGlass_StaticObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlass_StaticObject::Initialize(void* pArg)
{
	GLASSOBJECTDESC* pDesc = static_cast<GLASSOBJECTDESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iObjectType = OBJECT_GLASS;

	m_iGlassObject = pDesc->iGlassNum;
	
	if (m_iGlassObject >= 10)
		m_bAllGlass = true;

	return S_OK;
}

void CGlass_StaticObject::Priority_Update(_float fTimeDelta)
{
}

void CGlass_StaticObject::Update(_float fTimeDelta)
{
}

void CGlass_StaticObject::Late_Update(_float fTimeDelta)
{
	if(m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fFrustumRadius))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);

}

HRESULT CGlass_StaticObject::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	if (*m_pDitheringOnOff)
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (m_bAllGlass)
			{
				m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_OPACITY, "g_MaskTexture", 0);

				m_pShaderCom->Begin(22);
				m_pModelCom->Render(i);
			}
			else
			{
				if (i == m_iGlassObject)
				{
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_OPACITY, "g_MaskTexture", 0);

					m_pShaderCom->Begin(22);
					m_pModelCom->Render(i);
				}
				else
				{
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0);
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

					m_pShaderCom->Begin(m_iPassIndex);
					m_pModelCom->Render(i);
				}
			}
		}
	}
	else
	{
		for (_uint i = 0; i < iNumMeshes; i++)
		{
			if (m_bAllGlass)
			{
				m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_OPACITY, "g_MaskTexture", 0);

				m_pShaderCom->Begin(22);
				m_pModelCom->Render(i);
			}
			else
			{
				if (i == m_iGlassObject)
				{
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_OPACITY, "g_MaskTexture", 0);

					m_pShaderCom->Begin(23);
					m_pModelCom->Render(i);
				}
				else
				{
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0);
					m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

					m_pShaderCom->Begin(19);
					m_pModelCom->Render(i);
				}
			}
		}
	}

	return S_OK;
}

HRESULT CGlass_StaticObject::Ready_Components()
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

	return S_OK;
}

HRESULT CGlass_StaticObject::Bind_ShaderResources()
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

	_float4 vGlassColor = _float4(1.f, 1.f, 1.f, 0.8f);
	_float4 vBaseColor = _float4(0.037f, 0.047f, 0.34f, 0.1f);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vGlassBaseColor", &vBaseColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vGlassColor", &vGlassColor, sizeof(_float4))))
		return E_FAIL;
	return S_OK;
}

CGlass_StaticObject* CGlass_StaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGlass_StaticObject* pInstance = new CGlass_StaticObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGlass_StaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGlass_StaticObject::Clone(void* pArg)
{
	CGlass_StaticObject* pInstance = new CGlass_StaticObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CGlass_StaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlass_StaticObject::Free()
{
	__super::Free();
}
