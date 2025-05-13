#include "pch.h" 
#include "UI_Attribute_Slot_Active.h"
#include "GameInstance.h"

CUI_Attribute_Slot_Active::CUI_Attribute_Slot_Active(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_Attribute_Slot_Active::CUI_Attribute_Slot_Active(const CUI_Attribute_Slot_Active& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_Attribute_Slot_Active::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Attribute_Slot_Active::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Attribute_Slot_Active::Priority_Update(_float fTimeDelta)
{
}

void CUI_Attribute_Slot_Active::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (__super::Mouse_Select_Talent(g_hWnd, DIM_LB))
		{
			//m_bRenderOpen = true;
			m_iTexNumber = 1;
		}
		else
		{
			//m_bRenderOpen = false;
			m_iTexNumber = 0;
		}

		m_bImageLoopOn = true;

		m_fCurrentTime += fTimeDelta;

		if (1 <= m_fCurrentTime)
		{
			m_fCurrentTime *= -1;
		}
	}
}

void CUI_Attribute_Slot_Active::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Attribute_Slot_Active::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bImageLoopOn", &m_bImageLoopOn, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTImeAlpha", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;

	m_iShaderPassNum = 1;

	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Attribute_Slot_Active::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Attribute_Slot_Active"),
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

CUI_Attribute_Slot_Active* CUI_Attribute_Slot_Active::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Attribute_Slot_Active* pInstance = new CUI_Attribute_Slot_Active(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Attribute_Slot_Active");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Attribute_Slot_Active::Clone(void* pArg)
{
	CUI_Attribute_Slot_Active* pInstance = new CUI_Attribute_Slot_Active(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Attribute_Slot_Active");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Attribute_Slot_Active::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
