#include "pch.h"
#include "UI_Boss_HP_Bar_Gage.h"
#include "GameInstance.h"
CUI_Boss_HP_Bar_Gage::CUI_Boss_HP_Bar_Gage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI_Image(pDevice, pContext)
{
}

CUI_Boss_HP_Bar_Gage::CUI_Boss_HP_Bar_Gage(const CUI_Boss_HP_Bar_Gage& Prototype)
    :CUI_Image(Prototype)
{
}

HRESULT CUI_Boss_HP_Bar_Gage::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Boss_HP_Bar_Gage::Initialize(void* pArg)
{
    m_fSizeX = 660.f;
    m_fSizeY = 10.f;
    UI_Boss_HP_Bar_Gage_DESC* pDesc = static_cast<UI_Boss_HP_Bar_Gage_DESC*>(pArg);
    pDesc->fNear = 0.f;
    pDesc->fFar = 1.f;
    pDesc->fSizeX = m_fSizeX;
    pDesc->fSizeY = m_fSizeY;
    pDesc->fX = 801;
    pDesc->fY = 50;
    pDesc->fZ = 0.09f;
    pDesc->fSpeedPerSec = 5.f;
    pDesc->fRotationPerSec = XMConvertToRadians(90.f);

    m_bBossActive = pDesc->bBossActive;
    m_bBossDead = pDesc->bBossDead;

    m_fBoss_Max_HP = pDesc->fMaxHP;
    m_fBoss_Cur_HP = pDesc->fCurHP;
    m_fBoss_Shield_HP = pDesc->fShieldHP;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CUI_Boss_HP_Bar_Gage::Priority_Update(_float fTimeDelta)
{

    if (!(*m_bBossActive))
        return;
}

void CUI_Boss_HP_Bar_Gage::Update(_float fTimeDelta)
{
    if (!(*m_bBossActive))
        return;
}

void CUI_Boss_HP_Bar_Gage::Late_Update(_float fTimeDelta)
{
    if (*m_bBossDead)
        m_pGameInstance->Add_DeadObject(TEXT("Layer_UIScene"), this);
    if (!(*m_bBossActive))
        return;
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_HP_Bar_Gage::Render()
{
    if (!(*m_bBossActive))
        return S_OK;
    for (_uint i = 1; i <= 2; i++)
    {
        _float pCurHP = {};
        if (i == 1)
        {
            pCurHP = *m_fBoss_Cur_HP / *m_fBoss_Max_HP;
        }
        else
        {
            pCurHP = *m_fBoss_Shield_HP / *m_fBoss_Max_HP;
        }


        if (FAILED(m_pShaderCom->Bind_RawValue("g_CurHP", &pCurHP, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
            return E_FAIL;
        if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", i)))
            return E_FAIL;

        m_pShaderCom->Begin(13);

        m_pVIBufferCom->Bind_InputAssembler();

        m_pVIBufferCom->Render();
    }
    return S_OK;
}

HRESULT CUI_Boss_HP_Bar_Gage::Ready_Components()
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

CUI_Boss_HP_Bar_Gage* CUI_Boss_HP_Bar_Gage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Boss_HP_Bar_Gage* pInstance = new CUI_Boss_HP_Bar_Gage(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CUI_Boss_HP_Bar_Gage");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Boss_HP_Bar_Gage::Clone(void* pArg)
{
    CUI_Boss_HP_Bar_Gage* pInstance = new CUI_Boss_HP_Bar_Gage(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CUI_Boss_HP_Bar_Gage");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Boss_HP_Bar_Gage::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
