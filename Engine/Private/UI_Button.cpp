#include "UI_Button.h"
#include "GameInstance.h"

CUI_Button::CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Button::CUI_Button(const CUI_Button& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Button::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Button::Initialize(void* pArg)
{
	m_iUIType = UI_BUTTON;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Button::Priority_Update(_float fTimeDelta)
{
}

void CUI_Button::Update(_float fTimeDelta)
{
}

void CUI_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Button::Render()
{
	return S_OK;
}

_bool CUI_Button::Mouse_Select_Talent(HWND hWnd, MOUSEKEYSTATE eMouseKey)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	if (ptMouse.x >= m_fPos.x - m_fSize.x / 3 &&
		ptMouse.x <= m_fPos.x + m_fSize.x / 3 &&
		ptMouse.y >= m_fPos.y - m_fSize.y / 3 &&
		ptMouse.y <= m_fPos.y + m_fSize.y / 3)
	{
		if (m_pGameInstance->isMouseEnter(eMouseKey))
		{
			iTestCount++;
		}
	}
	if (0 == iTestCount % 2)
	{
		return false;

	}
	else if (1 == iTestCount % 2)
	{
		return true;
	}
	return false;
}

_bool CUI_Button::Mouse_Select(HWND hWnd, MOUSEKEYSTATE eMouseKey, _float fSize)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	if (ptMouse.x >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - m_pTransformCom->Compute_Scaled().x / fSize &&
		ptMouse.x <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x + m_pTransformCom->Compute_Scaled().x / fSize &&
		ptMouse.y >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - m_pTransformCom->Compute_Scaled().y / fSize &&
		ptMouse.y <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y + m_pTransformCom->Compute_Scaled().y / fSize)
	{
		if (m_pGameInstance->isMouseEnter(eMouseKey))
		{
			return true;
		}
	}
	return false;

}

_bool CUI_Button::On_Mouse_UI(HWND hWnd, _uint iRange)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	if (ptMouse.x >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - m_pTransformCom->Compute_Scaled().x / iRange &&
		ptMouse.x <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x + m_pTransformCom->Compute_Scaled().x / iRange &&
		ptMouse.y >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - m_pTransformCom->Compute_Scaled().y / iRange &&
		ptMouse.y <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y + m_pTransformCom->Compute_Scaled().y / iRange)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

void CUI_Button::Free()
{
	__super::Free();
}
