#include "pch.h" 
#include "UI_KeyBox_Long.h"
#include "GameInstance.h"

CUI_KeyBox_Long::CUI_KeyBox_Long(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Button{ pDevice, pContext }
{
}

CUI_KeyBox_Long::CUI_KeyBox_Long(const CUI_KeyBox_Long& Prototype)
    : CUI_Button(Prototype)
{
}

HRESULT CUI_KeyBox_Long::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_KeyBox_Long::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_KeyBox_Long::Priority_Update(_float fTimeDelta)
{
}

void CUI_KeyBox_Long::Update(_float fTimeDelta)
{
	if (m_bRenderOpen) // UI 가 보여지고 있을 때에만 기능 작동
	{
		if (__super::Mouse_Select(g_hWnd, DIM_LB, 3))
		{
			m_bMouseSelectOn = true; // 최초에 마우스 클릭이 있는지 체크
			m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);
		}
		else
		{
			m_bMouseSelectOn = false; // 

		}
	}
}

void CUI_KeyBox_Long::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if(m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_KeyBox_Long::Render()
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

		_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = fMyPos.x - TextSize.x / 2;
		m_fTextPosition.y = fMyPos.y - TextSize.y / 2;
		m_fTextPosition.z = fMyPos.z;
		m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), { m_fTextPosition.x,m_fTextPosition.y }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 1.0f, m_fTextPosition.z);
	}
	return S_OK;
}

HRESULT CUI_KeyBox_Long::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_KeyBox_Long"),
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

CUI_KeyBox_Long* CUI_KeyBox_Long::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_KeyBox_Long* pInstance = new CUI_KeyBox_Long(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_KeyBox_Long");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_KeyBox_Long::Clone(void* pArg)
{
	CUI_KeyBox_Long* pInstance = new CUI_KeyBox_Long(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_KeyBox_Long");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_KeyBox_Long::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
