#include "pch.h" 
#include "UI_MapEntryFrame.h"
#include "GameInstance.h"

CUI_MapEntryFrame::CUI_MapEntryFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_MapEntryFrame::CUI_MapEntryFrame(const CUI_MapEntryFrame& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_MapEntryFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MapEntryFrame::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_MapEntryFrame::Priority_Update(_float fTimeDelta)
{
}

void CUI_MapEntryFrame::Update(_float fTimeDelta)
{
	if (m_bRenderOpen) // UI 가 보여지고 있을 때에만 기능 작동
	{
		if (__super::On_Mouse_UI(g_hWnd, 3))
		{
			m_bImageOn = true;
			m_iTexNumber = 1;
			if (m_bSoundOnOff && m_bOpen)
			{
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Inventory_Material_Stone_UI_1.ogg"), CHANNELID::SOUND_UI, 0.2f);
				m_bSoundOnOff = false;
			}
		}
		else
		{
			m_bImageOn = false;
			m_iTexNumber = 0;
			if (!m_bSoundOnOff && m_bOpen)
			{
				m_bSoundOnOff = true;
			}
		}

		if (m_bImageOn)
		{
			if (__super::Mouse_Select(g_hWnd, DIM_LB, 3))
			{
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);
				m_bMouseSelectOn = true; // 최초에 마우스 클릭이 있는지 체크
			}
			else
			{
				m_bMouseSelectOn = false; // 

			}
		}
	}

}

void CUI_MapEntryFrame::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_MapEntryFrame::Render()
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

HRESULT CUI_MapEntryFrame::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapEntryFrame"),
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

CUI_MapEntryFrame* CUI_MapEntryFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MapEntryFrame* pInstance = new CUI_MapEntryFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_MapEntryFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MapEntryFrame::Clone(void* pArg)
{
	CUI_MapEntryFrame* pInstance = new CUI_MapEntryFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_MapEntryFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MapEntryFrame::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
