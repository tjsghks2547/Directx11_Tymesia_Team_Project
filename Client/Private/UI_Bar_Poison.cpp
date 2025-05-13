#include "pch.h" 
#include "UI_Bar_Poison.h"
#include "GameInstance.h"

CUI_Bar_Poison::CUI_Bar_Poison(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_Bar_Poison::CUI_Bar_Poison(const CUI_Bar_Poison& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_Bar_Poison::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Bar_Poison::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Bar_Poison::Priority_Update(_float fTimeDelta)
{
}

void CUI_Bar_Poison::Update(_float fTimeDelta)
{
	if (m_bOpen)
	{
		m_fTimeDelta = fTimeDelta;
		if (m_fPoisonTime > m_fDeadTime)
		{
			m_fPoisonTime = m_fDeadTime;
			m_bDeadSet = true;
		}
		else
		{
			m_fPoisonTime += fTimeDelta;
		}
	}
}

void CUI_Bar_Poison::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if(m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Bar_Poison::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTexNoiseCom->Bind_ShaderResource(m_pShaderCom, "g_TexNoise", 0)))
		return E_FAIL;
	if (FAILED(m_pTexGlowCom->Bind_ShaderResource(m_pShaderCom, "g_TexGlow", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fGageBar_Current", &m_fPoisonTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fGageBar_Max", &m_fDeadTime, sizeof(_float))))
		return E_FAIL;

if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;



	m_pShaderCom->Begin(14);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Bar_Poison::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bar_Poison"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bar_Noise"),
		TEXT("Com_TexNoise"), reinterpret_cast<CComponent**>(&m_pTexNoiseCom))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BarGlow_Poison"),
		TEXT("Com_TexGlow"), reinterpret_cast<CComponent**>(&m_pTexGlowCom))))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CUI_Bar_Poison* CUI_Bar_Poison::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Bar_Poison* pInstance = new CUI_Bar_Poison(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Bar_Poison");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Bar_Poison::Clone(void* pArg)
{
	CUI_Bar_Poison* pInstance = new CUI_Bar_Poison(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Bar_Poison");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Bar_Poison::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexNoiseCom);
	Safe_Release(m_pTexGlowCom);
}
