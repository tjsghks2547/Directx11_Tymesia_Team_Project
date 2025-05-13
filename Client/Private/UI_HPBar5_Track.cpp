#include "pch.h" 
#include "UI_HPBar5_Track.h"
#include "GameInstance.h"

CUI_HPBar5_Track::CUI_HPBar5_Track(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_HPBar5_Track::CUI_HPBar5_Track(const CUI_HPBar5_Track& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_HPBar5_Track::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_HPBar5_Track::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_iChangeX = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x;

	return S_OK;
}

void CUI_HPBar5_Track::Priority_Update(_float fTimeDelta)
{
}

void CUI_HPBar5_Track::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pTransformCom->Set_State_UIObj(CTransform::STATE_POSITION, { (_float)m_iChangeX ,m_fPos.y });
	}
}

void CUI_HPBar5_Track::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if(m_bImageOn) // 체력 게이지가 full 일때는 랜더를 끈다
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_HPBar5_Track::Render()
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

HRESULT CUI_HPBar5_Track::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HPBar5_Track"),
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

CUI_HPBar5_Track* CUI_HPBar5_Track::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_HPBar5_Track* pInstance = new CUI_HPBar5_Track(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_HPBar5_Track");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_HPBar5_Track::Clone(void* pArg)
{
	CUI_HPBar5_Track* pInstance = new CUI_HPBar5_Track(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_HPBar5_Track");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_HPBar5_Track::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
