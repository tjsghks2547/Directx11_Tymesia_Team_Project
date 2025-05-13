#include "pch.h" 
#include "UI_Arrow.h"
#include "GameInstance.h"

CUI_Arrow::CUI_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_Arrow::CUI_Arrow(const CUI_Arrow& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_Arrow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Arrow::Initialize(void* pArg)
{
	//m_pUITextContent = L"테스트로 적어봅니다";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Arrow::Priority_Update(_float fTimeDelta)
{
}

void CUI_Arrow::Update(_float fTimeDelta)
{
	if (m_bOpen)
	{
		if (__super::Mouse_Select(g_hWnd, DIM_LB, 2))
		{
			m_bMouseSelectOn = true;
			m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Ice_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);

		}
		else
			m_bMouseSelectOn = false;
	}
	else
		m_bMouseSelectOn = false;
}

void CUI_Arrow::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Arrow::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Arrow::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arrow"),
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

CUI_Arrow* CUI_Arrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Arrow* pInstance = new CUI_Arrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Arrow::Clone(void* pArg)
{
	CUI_Arrow* pInstance = new CUI_Arrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Arrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Arrow::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
