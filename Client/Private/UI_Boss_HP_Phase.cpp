#include "pch.h"
#include "UI_Boss_HP_Phase.h"
#include "Boss_Varg.h"
#include "GameInstance.h"

CUI_Boss_HP_Phase::CUI_Boss_HP_Phase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI_Image(pDevice, pContext)
{
}

CUI_Boss_HP_Phase::CUI_Boss_HP_Phase(const CUI_Boss_HP_Phase& Prototype)
    :CUI_Image(Prototype)
{
}

HRESULT CUI_Boss_HP_Phase::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Boss_HP_Phase::Initialize(void* pArg)
{
    m_fSizeX = 19.f;
    m_fSizeY = 19.f;
    UI_Boss_HP_Phase_Desc* pDesc = static_cast<UI_Boss_HP_Phase_Desc*>(pArg);
    pDesc->fNear = 0.f;
    pDesc->fFar = 1.f;
    pDesc->fSizeX = m_fSizeX;
    pDesc->fSizeY = m_fSizeY;
    pDesc->fX = 1171;
    pDesc->fY = 50;
    pDesc->fZ = 0.08f;
    pDesc->fSpeedPerSec = 5.f;
    pDesc->fRotationPerSec = XMConvertToRadians(90.f);

    m_bBossActive = pDesc->bBossActive;
    m_iPhase = pDesc->iPhase;
    m_bBossDead = pDesc->bBossDead;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_RenderNum = 2;
    vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    return S_OK;
}

void CUI_Boss_HP_Phase::Priority_Update(_float fTimeDelta)
{
    if (!(*m_bBossActive))
        return;
    if (*m_iPhase == CBoss_Varg::PHASE_TWO)
        m_RenderNum = 1;

}

void CUI_Boss_HP_Phase::Update(_float fTimeDelta)
{
    if (!(*m_bBossActive))
        return;
}

void CUI_Boss_HP_Phase::Late_Update(_float fTimeDelta)
{
    if (*m_bBossDead)
        m_pGameInstance->Add_DeadObject(TEXT("Layer_UIScene"), this);

    if (!(*m_bBossActive))
        return;
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_HP_Phase::Render()
{
    if (!(*m_bBossActive))
        return S_OK;

    for (_uint i = 0; i < m_RenderNum; i++)
    {
        _vector pPos = vPos;
        _float fPosX = XMVectorGetX(vPos);
        fPosX += (i * 25.f);
        pPos = XMVectorSetX(pPos, fPosX);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, pPos);

        _float3 fColor = { 1.f,1.f,1.f };
        _bool m_bColor = true;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_ButtonColor", &fColor, sizeof(_float3))))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 4)))
            return E_FAIL;

        m_pShaderCom->Begin(17);

        m_pVIBufferCom->Bind_InputAssembler();

        m_pVIBufferCom->Render();
    }

    return S_OK;
}

HRESULT CUI_Boss_HP_Phase::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HP"),
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

CUI_Boss_HP_Phase* CUI_Boss_HP_Phase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Boss_HP_Phase* pInstance = new CUI_Boss_HP_Phase(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUI_Boss_HP_Phase");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Boss_HP_Phase::Clone(void* pArg)
{
    CUI_Boss_HP_Phase* pInstance = new CUI_Boss_HP_Phase(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUI_Boss_HP_Phase");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Boss_HP_Phase::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
