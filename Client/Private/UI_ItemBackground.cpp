#include "pch.h" 
#include "UI_ItemBackground.h"
#include "GameInstance.h"

CUI_ItemBackground::CUI_ItemBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_ItemBackground::CUI_ItemBackground(const CUI_ItemBackground& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_ItemBackground::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ItemBackground::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_ItemBackground::Priority_Update(_float fTimeDelta)
{
}

void CUI_ItemBackground::Update(_float fTimeDelta)
{
	if (__super::On_Mouse_UI(g_hWnd, 2))
	{
		m_bMouseOn = true;
		if (m_bSoundOnOff && m_bOpen)
		{
			m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Inventory_Material_Stone_UI_1.ogg"), CHANNELID::SOUND_UI, 0.2f);
			m_bSoundOnOff = false;
		}
	}
	else
	{
		m_bMouseOn = false;
		if (!m_bSoundOnOff && m_bOpen)
		{
			m_bSoundOnOff = true;
		}
	}
	if (m_pGameInstance->isMouseEnter(DIM_RB))
	{
		if (__super::Mouse_Select(g_hWnd, DIM_RB,2.5))
		{
			m_bMouseSelectOn = true;
		}
	}
	
}

void CUI_ItemBackground::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_ItemBackground::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bIconOn", &m_bItemIconOn,sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTexIconCom->Bind_ShaderResource(m_pShaderCom, "g_TexIcon", m_iTexicon)))
		return E_FAIL;
	
	m_iShaderPassNum = 4;

	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	if (lstrcmp(m_strContentText.c_str(), TEXT("%d")))
	{
		_float3 fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
		_float3 fMySize = m_pTransformCom->Compute_Scaled();

		_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = (fMyPos.x + (fMySize.x/2)) - TextSize.x;
		m_fTextPosition.y = (fMyPos.y + (fMySize.y / 2)) - TextSize.y;
		m_fTextPosition.z = fMyPos.z;
		m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), { m_fTextPosition.x,m_fTextPosition.y }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 1.0f,m_fTextPosition.z);
	}


	return S_OK;
}

HRESULT CUI_ItemBackground::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ItemBackground"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Item_Icon"),
		TEXT("Com_TexIcon"), reinterpret_cast<CComponent**>(&m_pTexIconCom))))
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

CUI_ItemBackground* CUI_ItemBackground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ItemBackground* pInstance = new CUI_ItemBackground(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_ItemBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ItemBackground::Clone(void* pArg)
{
	CUI_ItemBackground* pInstance = new CUI_ItemBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_ItemBackground");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ItemBackground::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexIconCom);
}
