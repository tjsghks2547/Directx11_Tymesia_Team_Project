#include "pch.h" 
#include "UI_Frame_Poison.h"
#include "GameInstance.h"

CUI_Frame_Poison::CUI_Frame_Poison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_Frame_Poison::CUI_Frame_Poison(const CUI_Frame_Poison& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_Frame_Poison::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Frame_Poison::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Frame_Poison::Priority_Update(_float fTimeDelta)
{
}

void CUI_Frame_Poison::Update(_float fTimeDelta)
{
}

void CUI_Frame_Poison::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Frame_Poison::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTexIconCom->Bind_ShaderResource(m_pShaderCom, "g_TexIcon", 0)))
		return E_FAIL;

	if (FAILED(m_pTexNoiseCom->Bind_ShaderResource(m_pShaderCom, "g_TexNoise", 0)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_bIconOn", &m_bIconOn, sizeof(_bool))))
		return E_FAIL;

	m_pShaderCom->Begin(4); 

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Frame_Poison::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Frame_Poison"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_Poison"),
		TEXT("Com_TexIcon"), reinterpret_cast<CComponent**>(&m_pTexIconCom))))
		return E_FAIL;
	
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BarGlow_Poison"),
		TEXT("Com_TexGlow"), reinterpret_cast<CComponent**>(&m_pTexNoiseCom))))
		return E_FAIL;
	
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CUI_Frame_Poison* CUI_Frame_Poison::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Frame_Poison* pInstance = new CUI_Frame_Poison(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Frame_Poison");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Frame_Poison::Clone(void* pArg)
{
	CUI_Frame_Poison* pInstance = new CUI_Frame_Poison(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Frame_Poison");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Frame_Poison::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexIconCom);
	Safe_Release(m_pTexNoiseCom);
}
