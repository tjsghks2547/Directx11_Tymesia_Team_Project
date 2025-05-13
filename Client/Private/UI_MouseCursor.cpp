#include "pch.h" 
#include "UI_MouseCursor.h"
#include "GameInstance.h"

CUI_MouseCursor::CUI_MouseCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_MouseCursor::CUI_MouseCursor(const CUI_MouseCursor& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_MouseCursor::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MouseCursor::Initialize(void* pArg)
{
	UI_COMPONENT_DESC pDesc = {};
	pDesc.fPosition.z = 0.0f;
	pDesc.fZ = 0.0f;
	pDesc.fSizeX = 50;
	pDesc.fSizeY = 50;
	pDesc.fNear = 0.f;
	pDesc.fFar = 1.f;

	pDesc.fSpeedPerSec = 5.f;
	pDesc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(&pDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_MouseCursor::Priority_Update(_float fTimeDelta)
{
	
	
	if(false == m_bRenderOpen)
	{
		_uint2 Size = m_pGameInstance->Get_ViewportSize();
		POINT ptMouse{ (LONG)Size.x >> 1, (LONG)Size.y >> 1 };
		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}
}

void CUI_MouseCursor::Update(_float fTimeDelta)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	_float2 vMousePos = { (_float)ptMouse.x,(_float)ptMouse.y };
	m_pTransformCom->Set_State_UIObj(CTransform::STATE_POSITION, { vMousePos.x + 15.f,vMousePos.y + 15.f });
}

void CUI_MouseCursor::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUI_MouseCursor::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_InputAssembler();	

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MouseCursor::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseCursor"),
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

CUI_MouseCursor* CUI_MouseCursor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_MouseCursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_MouseCursor::Clone(void* pArg)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_MouseCursor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MouseCursor::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
