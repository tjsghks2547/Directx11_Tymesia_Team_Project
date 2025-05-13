#include "UI_Image.h"
#include "GameInstance.h"

CUI_Image::CUI_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Image::CUI_Image(const CUI_Image& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Image::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Image::Initialize(void* pArg)
{
	m_iUIType = UI_IMAGE;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUI_Image::Priority_Update(_float fTimeDelta)
{
}

void CUI_Image::Update(_float fTimeDelta)
{
}

void CUI_Image::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Image::Render()
{
	return S_OK;
}

_bool CUI_Image::On_Mouse_UI(HWND hWnd, _int iSize)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	if (ptMouse.x >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - m_pTransformCom->Compute_Scaled().x / iSize &&
		ptMouse.x <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x + m_pTransformCom->Compute_Scaled().x / iSize &&
		ptMouse.y >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - m_pTransformCom->Compute_Scaled().y / iSize &&
		ptMouse.y <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y + m_pTransformCom->Compute_Scaled().y / iSize)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

_bool CUI_Image::On_Mouse_Side_Select(HWND hWnd, _int iSize, MOUSEKEYSTATE eMouseKey)
{
	// �̹��� ���� �ٱ����� Ŭ�� �ߴٴ� ��
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	if (ptMouse.x >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - m_pTransformCom->Compute_Scaled().x / iSize &&
		ptMouse.x <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x + m_pTransformCom->Compute_Scaled().x / iSize &&
		ptMouse.y >= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - m_pTransformCom->Compute_Scaled().y / iSize &&
		ptMouse.y <= m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y + m_pTransformCom->Compute_Scaled().y / iSize)
	{
		return false; // �̹��� ���� Ŭ��
	}
	else
	{
		if (m_pGameInstance->isMouseEnter(eMouseKey))
		{
			return true; // �̹��� �ٱ��� Ŭ����

		}
	}

	return false; // Ŭ�� ���� ����
}

void CUI_Image::Free()
{
	__super::Free();
}
