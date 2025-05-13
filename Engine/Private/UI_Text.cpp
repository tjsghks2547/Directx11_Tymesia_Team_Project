#include "UI_Text.h"

CUI_Text::CUI_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Text::CUI_Text(const CUI_Text& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Text::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Text::Initialize(void* pArg)
{
	m_iUIType = UI_TEXT;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUI_Text::Priority_Update(_float fTimeDelta)
{
}

void CUI_Text::Update(_float fTimeDelta)
{
}

void CUI_Text::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Text::Render()
{
	return S_OK;
}

void CUI_Text::Free()
{
	__super::Free();
}
