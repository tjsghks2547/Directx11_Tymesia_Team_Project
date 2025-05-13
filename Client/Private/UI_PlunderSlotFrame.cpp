#include "pch.h" 
#include "UI_PlunderSlotFrame.h"
#include "GameInstance.h"

CUI_PlunderSlotFrame::CUI_PlunderSlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Image{ pDevice, pContext }
{
}

CUI_PlunderSlotFrame::CUI_PlunderSlotFrame(const CUI_PlunderSlotFrame& Prototype)
    : CUI_Image(Prototype)
{
}

HRESULT CUI_PlunderSlotFrame::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_PlunderSlotFrame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_iBeforeIcon = m_iTexicon;
	m_iAfterIcon = m_iBeforeIcon;
	return S_OK;
}

void CUI_PlunderSlotFrame::Priority_Update(_float fTimeDelta)
{
}

void CUI_PlunderSlotFrame::Update(_float fTimeDelta)
{
	if (m_bSkillOn)
	{
		m_fSkill_CoolTime += fTimeDelta;
		m_fEffect_Alpha += fTimeDelta;
		if (m_fSkill_CoolTime > 2.5f)
		{
			m_fSkill_CoolTime = 0.0f;
			m_fEffect_Alpha = 0.0f;
			m_bSkillOn = false;
			m_bEffectOn = true;
		}

	}

	if (m_iBeforeIcon != m_iTexicon)
	{
		m_iBeforeIcon = m_iTexicon;

		if (m_iBeforeIcon == 0) // 아이콘이 없는 상태
		{
			m_bIconEffectClose = true; // 아이콘이 점점 사라지는 효과
			m_fIcon_CreativeTime = 0.0f;
		}
		else // 아이콘이 보여지는 상태
		{
			m_bTexIconON = true;
			m_bIconEffectOpen = true; // 아이콘이 점점 생겨나는 효과
			m_fIcon_CreativeTime = -1.0f;
			m_iAfterIcon = m_iBeforeIcon; // 아이콘이 보여질 때 세팅
		}
	}

	if (m_bIconEffectClose) // 켜졌으니 사라지는 시간 값 주기
	{
		m_fIcon_CreativeTime -= fTimeDelta * 1.8f;
		if (m_fIcon_CreativeTime < -1.f)
		{
			m_fIcon_CreativeTime = 0.0f;
			m_bIconEffectClose = false;
			m_bTexIconON = false;
			m_iAfterIcon = m_iBeforeIcon;
		}
	}

	if (m_bIconEffectOpen)
	{
		m_fIcon_CreativeTime += fTimeDelta * 0.8f;
		if (m_fIcon_CreativeTime > 2)
		{
			m_fIcon_CreativeTime = 1.0f;
			m_bIconEffectOpen = false;
		}
	}


}



void CUI_PlunderSlotFrame::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_PlunderSlotFrame::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bIconOn", &m_bTexIconON, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bSkillOn", &m_bSkillOn, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_SlotNum", &m_iTexNumber, sizeof(_uint))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTImeAlpha", &m_fEffect_Alpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fIcon_CreativeTime, sizeof(_float))))
		return E_FAIL;
	
	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;
	if (FAILED(m_pTexIconCom->Bind_ShaderResource(m_pShaderCom, "g_TexIcon", m_iAfterIcon)))
		return E_FAIL;
	if (FAILED(m_pTexEffectCom->Bind_ShaderResource(m_pShaderCom, "g_TexEffect", m_iTexEffect)))
		return E_FAIL;
	if (FAILED(m_pTexNoiseCom->Bind_ShaderResource(m_pShaderCom, "g_TexNoise", 0)))
		return E_FAIL;


	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_PlunderSlotFrame::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlunderSlotFrame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/*Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_SlotFrame"),
		TEXT("Com_TexEffect"), reinterpret_cast<CComponent**>(&m_pTexEffectCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillIcon"),
		TEXT("Com_TexIcon"), reinterpret_cast<CComponent**>(&m_pTexIconCom))))
		return E_FAIL;
	
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CloudNoise"),
		TEXT("Com_TexNoise"), reinterpret_cast<CComponent**>(&m_pTexNoiseCom))))
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

CUI_PlunderSlotFrame* CUI_PlunderSlotFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlunderSlotFrame* pInstance = new CUI_PlunderSlotFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_PlunderSlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlunderSlotFrame::Clone(void* pArg)
{
	CUI_PlunderSlotFrame* pInstance = new CUI_PlunderSlotFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_PlunderSlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlunderSlotFrame::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexIconCom);
	Safe_Release(m_pTexEffectCom);
	Safe_Release(m_pTexNoiseCom);
}
