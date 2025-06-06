#include "pch.h" 
#include "UI_SquareFrame_Hover.h"
#include "GameInstance.h"

CUI_SquareFrame_Hover::CUI_SquareFrame_Hover(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_SquareFrame_Hover::CUI_SquareFrame_Hover(const CUI_SquareFrame_Hover& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_SquareFrame_Hover::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SquareFrame_Hover::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_SquareFrame_Hover::Priority_Update(_float fTimeDelta)
{
}

void CUI_SquareFrame_Hover::Update(_float fTimeDelta)
{
	/*if (__super::On_Mouse_UI(g_hWnd,6))
		m_bImageOn = true;
	else
		m_bImageOn = false;*/
}

void CUI_SquareFrame_Hover::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_SquareFrame_Hover::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bImageOn", &m_bImageOn, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SquareFrame_Hover::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SquareFrame_Hover"),
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

CUI_SquareFrame_Hover* CUI_SquareFrame_Hover::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SquareFrame_Hover* pInstance = new CUI_SquareFrame_Hover(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_SquareFrame_Hover");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SquareFrame_Hover::Clone(void* pArg)
{
	CUI_SquareFrame_Hover* pInstance = new CUI_SquareFrame_Hover(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_SquareFrame_Hover");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SquareFrame_Hover::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
