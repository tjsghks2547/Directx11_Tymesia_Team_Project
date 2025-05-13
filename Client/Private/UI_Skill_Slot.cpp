#include "pch.h" 
#include "UI_Skill_Slot.h"
#include "GameInstance.h"

CUI_Skill_Slot::CUI_Skill_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button{ pDevice, pContext }
{
}

CUI_Skill_Slot::CUI_Skill_Slot(const CUI_Skill_Slot& Prototype)
	: CUI_Button(Prototype)
{
}

HRESULT CUI_Skill_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Skill_Slot::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Skill_Slot::Priority_Update(_float fTimeDelta)
{
}

void CUI_Skill_Slot::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (__super::On_Mouse_UI(g_hWnd, 3))
		{
			if (m_eSlotState == SKILL_OPEN_OFF)
				m_eSlotState = SKILL_OPEN_ON;
			if (m_eSlotState == SKILL_CLOSE_OFF)
				m_eSlotState = SKILL_CLOSE_ON;
			m_bImageOn = true;
			if (m_bSoundOnOff && m_bOpen)
			{
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Inventory_Material_Stone_UI_1.ogg"), CHANNELID::SOUND_UI, 0.2f);
				m_bSoundOnOff = false;
			}
		}
		else
		{
			if (m_eSlotState == SKILL_OPEN_ON)
				m_eSlotState = SKILL_OPEN_OFF;
			if (m_eSlotState == SKILL_CLOSE_ON)
				m_eSlotState = SKILL_CLOSE_OFF;
			m_bImageOn = false;
			if (!m_bSoundOnOff && m_bOpen)
			{
				m_bSoundOnOff = true;
			}
		}
		if (m_bOpenContion && __super::Mouse_Select(g_hWnd, DIM_LB, 3)) // ÇØ±Ý Àü
		{
			m_bMouseSelectOn = true;
			if (m_eSlotState == SKILL_CLOSE_ON)
			{
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);
				m_eSlotState = SKILL_OPEN_ON;
				m_iTexNumber = m_iIconChange;
				m_pGameInstance->Use_Item(m_eNeedItem, 3);
				m_bOpenContion = false;
			}
		}
		if(m_eSlotState == SKILL_OPEN_OFF)
			m_iTexNumber = m_iIconChange;

		m_fCurrentTime += fTimeDelta;

		if (1 <= m_fCurrentTime)
		{
			m_fCurrentTime *= -1;
		}
	}
}

void CUI_Skill_Slot::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Skill_Slot::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_SLOT]->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexSlot)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_ICON]->Bind_ShaderResource(m_pShaderCom, "g_TexIcon", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_EDGE]->Bind_ShaderResource(m_pShaderCom, "g_TexEdge", m_iTexEdge)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_EFFECT]->Bind_ShaderResource(m_pShaderCom, "g_TexEffect", m_iTexEffect)))
		return E_FAIL;
	

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTImeAlpha", &m_fCurrentTime, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bIconOn", &m_bTexIconOpen, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexIconOff", &m_bTexIconOff, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexEdgeOff", &m_bTexEdgeOff, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bTexEffectOff", &m_bTexEffectOff, sizeof(_bool))))
		return E_FAIL;

	m_pShaderCom->Begin(m_iShaderPassNum );

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Skill_Slot::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Frame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_SLOT]))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillIcon"),
		TEXT("Com_TexIcon"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_ICON]))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_Frame"),
		TEXT("Com_TexEdge"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_EDGE]))))
		return E_FAIL;
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Attribute_Slot_Active"),
		TEXT("Com_TexEffect"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_EFFECT]))))
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

CUI_Skill_Slot* CUI_Skill_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Skill_Slot* pInstance = new CUI_Skill_Slot(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Skill_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Skill_Slot::Clone(void* pArg)
{
	CUI_Skill_Slot* pInstance = new CUI_Skill_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Skill_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Skill_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& Tex : m_pTextureCom)
		Safe_Release(Tex);

}
