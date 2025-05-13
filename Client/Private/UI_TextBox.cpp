#include "pch.h" 
#include "UI_TextBox.h"
#include "GameInstance.h"

CUI_TextBox::CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Text{ pDevice, pContext }
{
}

CUI_TextBox::CUI_TextBox(const CUI_TextBox& Prototype)
	: CUI_Text(Prototype)
{
}

HRESULT CUI_TextBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_TextBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_TextBox::Priority_Update(_float fTimeDelta)
{
}

void CUI_TextBox::Update(_float fTimeDelta)
{
	_float2 TextSize = {};
	_float3 fMyPos = {};
	switch (m_eTextSort)
	{
	case Client::CUI_TextBox::TEXT_CENTER:
		fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
		TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = fMyPos.x - TextSize.x / 2;
		m_fTextPosition.y = fMyPos.y - TextSize.y / 2;
		break;
	case Client::CUI_TextBox::TEXT_LEFT:
		TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x;
		m_fTextPosition.y = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y;
		break;
	case Client::CUI_TextBox::TEXT_RIGHT:
		fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
		_float3 fMySize = m_pTransformCom->Compute_Scaled();
		TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
		m_fTextPosition.x = (fMyPos.x - TextSize.x);
		m_fTextPosition.y = (fMyPos.y - (TextSize.y / 2));
		break;

	}
	if (m_eRenderType == Engine::TEXT_ALPHALOOP)
	{
		m_fAlpha += fTimeDelta;

		if (1 <= m_fAlpha)
		{
			m_fAlpha *= -1.f;
		}
	}
	if (m_eRenderType == Engine::TEXT_ALPHA)
	{
		m_fAlpha += fTimeDelta;
		if (1 <= m_fAlpha)
		{
			m_fAlpha = 1.f;
		}
	}

	if (m_bOpen)
	{
		if (m_eRenderType == Engine::TEXT_ALPHA_ANIM)
		{
			m_fAnimAlpha += fTimeDelta;
			if (1.f >= m_fAnimAlpha)
			{
				m_fAlpha = m_fAnimAlpha;
				if (1.f < m_fAlpha)
					m_fAlpha = 1.f;
			
			}
			else if (2.f >= m_fAnimAlpha)
			{
				m_fAlpha = m_fAnimAlpha;
				if (1.f < m_fAlpha)
					m_fAlpha = 1.f;
			}
			else if (3.f >= m_fAnimAlpha)
			{
				m_fAlpha = (3.0f - m_fAnimAlpha);

			}
			else
			{
				m_fAlpha = 0.0f;
				m_fAnimAlpha = 0.0f;
			}
		}
	}
	else
	{
		m_fAlpha = 0.0f;
		m_fAnimAlpha = 0.0f;
	}
}

void CUI_TextBox::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_bOpen)
			m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_TextBox::Render()
{
	if (m_bRenderOpen)
	{
		
		_float fZ = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).z;

		switch (m_eRenderType)
		{
		case Engine::TEXT_DEFALUT:
			m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor, 0.0f, { 0.0f,0.0f }, 1.0f, fZ);
			break;
		case Engine::TEXT_SHADOW:
			m_pGameInstance->Render_Shadow(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor);
			break;
		case Engine::TEXT_OUTLINE:
			m_pGameInstance->Render_Outline(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor);
			break;
		case Engine::TEXT_TWOCOLOR:
			m_pGameInstance->Render_Color(m_strFontName, m_strContentText.c_str(), m_strContentText2.c_str(), m_fTextPosition, m_fTextColor, m_fTextColor2);
			break;
		case Engine::TEXT_ALPHALOOP:
			m_pGameInstance->Render_Alpha(m_strFontName, m_strContentText.c_str(), m_fTextPosition,
				{ m_fTextColor.x,m_fTextColor.y,m_fTextColor.z,abs(m_fAlpha) }, 0.0f, { 0.0f,0.0f }, 1.0f);
			break;
		case Engine::TEXT_ALPHA:
			m_pGameInstance->Render_Alpha(m_strFontName, m_strContentText.c_str(), m_fTextPosition,
				{ m_fTextColor.x,m_fTextColor.y,m_fTextColor.z,abs(m_fAlpha) }, 0.0f, { 0.0f,0.0f }, 1.0f);
			break;
		case Engine::TEXT_ALPHA_ANIM:
			m_pGameInstance->Render_Alpha(m_strFontName, m_strContentText.c_str(), m_fTextPosition,
				{ m_fTextColor.x,m_fTextColor.y,m_fTextColor.z,abs(m_fAlpha) }, 0.0f, { 0.0f,0.0f }, 1.0f);
			break;
		}

	}
	return S_OK;
}

CUI_TextBox* CUI_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_TextBox* pInstance = new CUI_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_TextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_TextBox::Clone(void* pArg)
{
	CUI_TextBox* pInstance = new CUI_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_TextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TextBox::Free()
{
	__super::Free();

}
