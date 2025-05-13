#include "pch.h"
#include "Body_Magician.h"
#include "GameInstance.h"
CBody_Magician::CBody_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CBody_Magician::CBody_Magician(const CBody_Magician& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CBody_Magician::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Magician::Initialize(void* pArg)
{
	CBody_Magician::BODY_MAGICIAN_DESC* pDesc = static_cast<BODY_MAGICIAN_DESC*>(pArg);

	m_IsDissolveOn = pDesc->IsDissolveOn;
	m_IsDissolveOff = pDesc->IsDissolveOff;
	m_bRender = pDesc->bRender;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CBody_Magician::Priority_Update(_float fTimeDelta)
{
	if (*m_IsDissolveOn)
	{
		m_eDissolveState = DISSOLVE_ON;
	}
	else if (*m_IsDissolveOff)
	{
		m_eDissolveState = DISSOLVE_OFF;
	}
	else
	{
		m_eDissolveState = DISSOLVE_NONE;
	}

	switch (m_eDissolveState)
	{
	case eDissolveState::DISSOLVE_ON:
		m_fDissolveTimer += fTimeDelta * 2.f;
		m_fDissolveTimer = min(m_fDissolveTimer, 1.5f);
		break;

	case eDissolveState::DISSOLVE_OFF:
		m_fDissolveTimer -= fTimeDelta * 2.f;
		m_fDissolveTimer = max(m_fDissolveTimer, 0.f);

		if (m_fDissolveTimer <= 0.f)
		{
			m_eDissolveState = eDissolveState::DISSOLVE_NONE;
		}
		break;

	case eDissolveState::DISSOLVE_NONE:
		break;
	}

}

void CBody_Magician::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CBody_Magician::Late_Update(_float fTimeDelta)
{
	if (*m_bRender)
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody_Magician::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		//´«¾Ë
		if (i == 8)
		{
			continue;
		}
		else if (i == 9)
		{
			m_iPassNum = 3;
		}
		else
		{
			m_iPassNum = 0;
		}
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // ¿©±â¼­ ÀÌµ¿°ªÀ» ¾ø¾ÖÁà¾ß°Ù³×
			return E_FAIL;


		if (m_eDissolveState != eDissolveState::DISSOLVE_NONE)
		{
			m_iPassNum = 5;

			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 13)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveTimer, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveTimer, sizeof(_float))))
				return E_FAIL;
		}
		else
		{
			m_iPassNum = 0;
		}

		m_pShaderCom->Begin(m_iPassNum);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CBody_Magician::Render_Shadow()
{
	return S_OK;
}

HRESULT CBody_Magician::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Body"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Magician::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CBody_Magician* CBody_Magician::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Magician* pInstance = new CBody_Magician(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBody_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Magician::Clone(void* pArg)
{
	CBody_Magician* pInstance = new CBody_Magician(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBody_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Magician::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
