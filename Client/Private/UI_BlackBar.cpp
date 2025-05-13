#include "pch.h" 
#include "UI_BlackBar.h"
#include "GameInstance.h"

CUI_BlackBar::CUI_BlackBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Image{ pDevice, pContext }
{
}

CUI_BlackBar::CUI_BlackBar(const CUI_BlackBar& Prototype)
    : CUI_Image(Prototype)
{
}

HRESULT CUI_BlackBar::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_BlackBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_BlackBar::Priority_Update(_float fTimeDelta)
{
}

void CUI_BlackBar::Update(_float fTimeDelta)
{
}

void CUI_BlackBar::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_BlackBar::Render()
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

	if (lstrcmp(m_strContentText.c_str(), TEXT("UI 기본 출력 값")))
	{
		_float3 fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
		_float3 fMySize = m_pTransformCom->Compute_Scaled();

		_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = ((fMyPos.x + (fMySize.x / 2)) - TextSize.x) - 2.0f;
		m_fTextPosition.y = ((fMyPos.y + (fMySize.y / 2)) - TextSize.y) + 2.0f;
		m_fTextPosition.z = fMyPos.z;
		m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), { m_fTextPosition.x,m_fTextPosition.y }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 1.0f, m_fTextPosition.z);
	}
	return S_OK;
}

HRESULT CUI_BlackBar::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BlackBar"),
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

CUI_BlackBar* CUI_BlackBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BlackBar* pInstance = new CUI_BlackBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_BlackBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BlackBar::Clone(void* pArg)
{
	CUI_BlackBar* pInstance = new CUI_BlackBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_BlackBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BlackBar::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
