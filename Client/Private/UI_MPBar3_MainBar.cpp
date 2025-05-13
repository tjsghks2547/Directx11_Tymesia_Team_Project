#include "pch.h" 
#include "UI_MPBar3_MainBar.h"
#include "GameInstance.h"

CUI_MPBar3_MainBar::CUI_MPBar3_MainBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Image{ pDevice, pContext }
{
}

CUI_MPBar3_MainBar::CUI_MPBar3_MainBar(const CUI_MPBar3_MainBar& Prototype)
    : CUI_Image(Prototype)
{
}

HRESULT CUI_MPBar3_MainBar::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_MPBar3_MainBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_MPBar3_MainBar::Priority_Update(_float fTimeDelta)
{
}

void CUI_MPBar3_MainBar::Update(_float fTimeDelta)
{

	if (m_bRenderOpen)
	{

		if (m_fDeliverMp > m_fCurentMP)
		{
			m_fDelayTime = 0.0f;

		}

		if (m_fDelayMp > m_fCurentMP)
		{
			m_fDelayTime += fTimeDelta;
			if (m_fDelayTime > 2.0f)
			{
				m_fDelayMp = m_fDelayMp + (m_fCurentMP - m_fDelayMp) * fTimeDelta;// *0.2f;  
			}
			//a + (b - a) * t;


		}
		else
		{
			m_fDelayTime = 0.0f;
			m_fDelayMp = m_fCurentMP;
		}

		m_fDeliverMp = m_fCurentMP;

	}

}

void CUI_MPBar3_MainBar::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_MPBar3_MainBar::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fGageBar_Current", &m_fCurentMP, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fGageBar_Delay", &m_fDelayMp, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fGageBar_Max", &m_fMaxMP, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	
	if (FAILED(m_pTexBackCom->Bind_ShaderResource(m_pShaderCom, "g_TexEffect", 0)))
		return E_FAIL;

	


	m_pShaderCom->Begin(13);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MPBar3_MainBar::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar3_MainBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar4_Delay"),
		TEXT("Com_TexBack"), reinterpret_cast<CComponent**>(&m_pTexBackCom))))
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

CUI_MPBar3_MainBar* CUI_MPBar3_MainBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MPBar3_MainBar* pInstance = new CUI_MPBar3_MainBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_MPBar3_MainBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MPBar3_MainBar::Clone(void* pArg)
{
	CUI_MPBar3_MainBar* pInstance = new CUI_MPBar3_MainBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_MPBar3_MainBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MPBar3_MainBar::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexBackCom);
}
