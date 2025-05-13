#include "pch.h" 
#include "UI_GameLogoImage.h"
#include "GameInstance.h"

CUI_GameLogoImage::CUI_GameLogoImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Image{ pDevice, pContext }
{
}

CUI_GameLogoImage::CUI_GameLogoImage(const CUI_GameLogoImage& Prototype)
    : CUI_Image(Prototype)
{
}

HRESULT CUI_GameLogoImage::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_GameLogoImage::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_bImageOn = true;
	return S_OK;
}

void CUI_GameLogoImage::Priority_Update(_float fTimeDelta)
{
}

void CUI_GameLogoImage::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{

		if (m_bOpen)
		{
			if (1 < m_fCurrentTime)
			{
				m_fCurrentTime = 1.f;
				m_LogoOpen = true;
			}
			else
				m_fCurrentTime += fTimeDelta / 5;


		}
	}
}

void CUI_GameLogoImage::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if(m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_GameLogoImage::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTImeAlpha", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bImageOn", &m_bImageOn, sizeof(_bool))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_GameLogoImage::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_GameLogoImage"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
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

CUI_GameLogoImage* CUI_GameLogoImage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_GameLogoImage* pInstance = new CUI_GameLogoImage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_GameLogoImage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_GameLogoImage::Clone(void* pArg)
{
	CUI_GameLogoImage* pInstance = new CUI_GameLogoImage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_GameLogoImage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_GameLogoImage::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
