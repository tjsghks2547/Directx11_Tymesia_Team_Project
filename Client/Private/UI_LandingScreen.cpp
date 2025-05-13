#include "pch.h" 
#include "UI_LandingScreen.h"
#include "GameInstance.h"

CUI_LandingScreen::CUI_LandingScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_LandingScreen::CUI_LandingScreen(const CUI_LandingScreen& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_LandingScreen::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_LandingScreen::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bImageOn = true;
	m_fMySize = m_fSize;

	m_mtrSaveWorld = m_pTransformCom->Get_WorldMatrix_Ptr();

	return S_OK;
}

void CUI_LandingScreen::Priority_Update(_float fTimeDelta)
{
}

void CUI_LandingScreen::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_bOpen)
		{
			if (1 <= m_fCurrentTime) // 1이 되는 순간
			{
				m_fCurrentTime *= -1; // -1 로 변경
			}
			else
			{
				m_fCurrentTime += fTimeDelta / 3; // 계속 델타타임 더하기
				m_fSizeTime = 1.0f + (fTimeDelta * 0.05f);

			}

			if (0 < m_iGroupID)
			{
				
					m_fMySize = { m_fMySize.x * m_fSizeTime, m_fMySize.y * m_fSizeTime };
					m_pTransformCom->Scaling(_float3(m_fMySize.x, m_fMySize.y, 1.f));

			}
				
		}
		else
		{
			m_fCurrentTime = 0.f;
			m_fSizeTime = 0.f;
			if (0 < m_iGroupID)
			{
				m_fMySize = m_fSize;
				m_pTransformCom->Set_WorldMatrix(*m_mtrSaveWorld);
			}
		}
	}
}

void CUI_LandingScreen::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
	
}

HRESULT CUI_LandingScreen::Render()
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

HRESULT CUI_LandingScreen::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LandingScreen"),
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

CUI_LandingScreen* CUI_LandingScreen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_LandingScreen* pInstance = new CUI_LandingScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_LandingScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LandingScreen::Clone(void* pArg)
{
	CUI_LandingScreen* pInstance = new CUI_LandingScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_LandingScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LandingScreen::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
