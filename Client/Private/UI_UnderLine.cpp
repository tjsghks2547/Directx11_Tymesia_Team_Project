#include "pch.h" 
#include "UI_UnderLine.h"
#include "GameInstance.h"

CUI_UnderLine::CUI_UnderLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_UnderLine::CUI_UnderLine(const CUI_UnderLine& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_UnderLine::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_UnderLine::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bImageOn = true;

	return S_OK;
}

void CUI_UnderLine::Priority_Update(_float fTimeDelta)
{
	
}

void CUI_UnderLine::Update(_float fTimeDelta)
{
	_float3 fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
	_float3 fMySize = m_pTransformCom->Compute_Scaled();

	_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
	m_fTextPosition.x = fMyPos.x - (TextSize.x / 2);
	m_fTextPosition.y = (fMyPos.y - (TextSize.y / 2)) - 25.f;


	/*m_fTextPosition.x = (fMyPos.x + (fMySize.x / 2)) - TextSize.x;
	m_fTextPosition.y = (fMyPos.y + (fMySize.y / 2)) - TextSize.y;*/


	m_fTextPosition.z = fMyPos.z;

	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	if (ptMouse.x >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - m_pTransformCom->Compute_Scaled().x / 2 &&
		ptMouse.x <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x + m_pTransformCom->Compute_Scaled().x / 2 &&
		ptMouse.y >= (m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - m_pTransformCom->Compute_Scaled().y / 2) - TextSize.y &&
		ptMouse.y <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y + m_pTransformCom->Compute_Scaled().y / 2)
	{
		if (m_pGameInstance->isMouseEnter(DIM_LB))
		{
			m_bMouseSelectOn = true;
			if (m_bOpen)
			{
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Book_Page_Turn_1.ogg"), CHANNELID::SOUND_UI, 0.1f);
			}
		}
	}
	else
	 	m_bMouseSelectOn = false;
	
	if (m_bImageOn)
	{
		if (1.0f <= m_fCurrentTime)
		{
			m_fCurrentTime = 1.0f; ;
		}
		else
		{
			m_fCurrentTime += fTimeDelta;

		}
	}
	else
	{
		m_fCurrentTime = 0.0f;

	}


}

void CUI_UnderLine::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_UnderLine::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bImageOn", &m_bImageOn, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTImeAlpha", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(11);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	if (lstrcmp(m_strContentText.c_str(), TEXT("%d")))
	{
		
		m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), { m_fTextPosition.x,m_fTextPosition.y }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 1.0f, m_fTextPosition.z);
	}
	return S_OK;
}

HRESULT CUI_UnderLine::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_UnderLine"),
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

CUI_UnderLine* CUI_UnderLine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_UnderLine* pInstance = new CUI_UnderLine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_UnderLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_UnderLine::Clone(void* pArg)
{
	CUI_UnderLine* pInstance = new CUI_UnderLine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_UnderLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_UnderLine::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
