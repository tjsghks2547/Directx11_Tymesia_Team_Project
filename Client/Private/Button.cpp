#include "pch.h"
#include "Button.h"
#include "GameInstance.h"
#include "GameItem.h"
#include "StaticObject.h"

CButton::CButton(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CUIObject{ _pDevice,_pContext }
{
}

CButton::CButton(const CButton& _Prototype)
    :CUIObject(_Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CButton::Initialize(void* _pArg)
{
    strcpy_s(m_szName, "BUTTON");

    BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(_pArg);

    switch (pDesc->_iButtonTypeIndex)
    {
    case 0:
        break;
    }

    pDesc->fSizeX = 40.f;
    pDesc->fSizeY = 40.f;
    pDesc->fX = 100.f;
    pDesc->fY = 100.f;
    pDesc->fZ = 0.0f;
    pDesc->fNear = 0.0f;
    pDesc->fFar = 1.0f;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CButton::Priority_Update(_float _fTimeDelta)
{
}

void CButton::Update(_float _fTimeDelta)
{

}

void CButton::Late_Update(_float _fTimeDelta)
{
    if (!m_bActivate && !m_bFadingOut)
        return;

    if (m_bFadingIn)
    {
        m_fAlphaValue += _fTimeDelta * 2.0f;
        if (m_fAlphaValue >= 1.0f)
        {
            m_fAlphaValue = 1.0f;
            m_bFadingIn = false;
        }
    }

    if (m_bFadingOut)
    {
        m_fAlphaValue -= _fTimeDelta * 2.0f;
        if (m_fAlphaValue <= 0.0f)
        {
            m_fAlphaValue = 0.0f;
            m_bFadingOut = false;
            m_bActivate = false;
        }
    }

    _vector vWorldPos = XMLoadFloat4(&m_vWorldPosition);
    _float4x4 matView = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW);
    _vector vViewPos = XMVector3TransformCoord(vWorldPos, XMLoadFloat4x4(&matView));

    _float4x4 matProj = m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ);
    _vector vProjPos = XMVector3TransformCoord(vViewPos, XMLoadFloat4x4(&matProj));
    //========================
    // (-1 ~ 1) -> (0 ~ 1) 
    _float2 vScreenPos;
    vScreenPos.x = (XMVectorGetX(vProjPos) + 1.0f) * 0.5f;
    vScreenPos.y = (1.0f - XMVectorGetY(vProjPos)) * 0.5f;

    _uint2 vViewportSize = m_pGameInstance->Get_ViewportSize();
    vScreenPos.x *= vViewportSize.x;
    vScreenPos.y *= vViewportSize.y;
    //========================

    vScreenPos.x -= vViewportSize.x * 0.5f;
    vScreenPos.y = vViewportSize.y * 0.5f - vScreenPos.y;

    vScreenPos.y -= 180.0f;

    m_fButtonPosition = { vScreenPos.x, vScreenPos.y, 0.0f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fButtonPosition.x, m_fButtonPosition.y, 0.0f, 1.0f));

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CButton::Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &m_fAlphaValue, sizeof(_float))))
        return E_FAIL;

    m_pShaderCom->Begin(16);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    _float3 fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);
    _float2 TextSize = m_pGameInstance->Get_TextSize(TEXT("Font_NotoSansKR18"), m_strText.c_str());
    _float2 vTextPosition;
    vTextPosition.x = (fMyPos.x - TextSize.x / 2) - 3.0f;
    vTextPosition.y = (fMyPos.y - TextSize.y / 2) - 1.0f;

    _float2 vSideTextPosition;
    vSideTextPosition.x = fMyPos.x + TextSize.x * 1.5f;
    vSideTextPosition.y = vTextPosition.y;

    _float4 vTextColor = { 1.f, 1.f, 1.f, m_fAlphaValue };

    m_pGameInstance->Render_World(TEXT("Font_NotoSansKR18"), m_strText.c_str(),
        { vTextPosition.x, vTextPosition.y }, vTextColor, 0.0f, { 0.0f,0.0f }, 1.0f);

    m_pGameInstance->Render_World(TEXT("Font_NotoSansKR18"), m_strDescriptionText.c_str(),
        { vSideTextPosition.x, vSideTextPosition.y }, vTextColor, 0.0f, { 0.0f,0.0f }, 1.0f);

    return S_OK;
}

void CButton::Activate_Button(_bool _bActivate)
{
    if (_bActivate)
    {
        m_bFadingIn = true;
        m_bFadingOut = false;
        m_bActivate = true;
    }
    else
    {
        m_bFadingIn = false;
        m_bFadingOut = true;
    }
}

void CButton::Set_WorldPosition(_float4 _vPosition)
{
    m_vWorldPosition = _vPosition;
}

void CButton::Set_ButtonText(const _wstring& _pText, const _wstring& _pDescText)
{
    m_strText = _pText;
    m_strDescriptionText = _pDescText;
}

HRESULT CButton::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InteractionButton"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

CButton* CButton::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CButton* pInstance = new CButton(_pDevice, _pContext);
    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CButton");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CGameObject* CButton::Clone(void* _pArg)
{
    CButton* pInstance = new CButton(*this);
    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CButton");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CButton::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
