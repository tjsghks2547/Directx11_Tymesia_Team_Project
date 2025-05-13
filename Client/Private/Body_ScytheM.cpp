#include "pch.h"
#include "Body_ScytheM.h"
#include "GameInstance.h"
#include "Normal_ScytheM.h"

CBody_ScytheM::CBody_ScytheM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBody_ScytheM::CBody_ScytheM(const CBody_ScytheM& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBody_ScytheM::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_ScytheM::Initialize(void* pArg)
{
    CBody_ScytheM::BODY_SCYTHEM_DESC* pDesc = static_cast<BODY_SCYTHEM_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_bDead = pDesc->bDead;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->SetUp_Animation(21, true);

    return S_OK;
}

void CBody_ScytheM::Priority_Update(_float fTimeDelta)
{
    if (*m_pParentState == STATE_DEAD)
    {
        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;
        if (m_fDeadTimer >= 1.5)
        {
            *m_bDead = true;
        }
    }
}

void CBody_ScytheM::Update(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CBody_ScytheM::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CBody_ScytheM::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        if (*m_pParentState == STATE_DEAD)
        {
            m_iPassNum = 5;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 6)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
                return E_FAIL;
        }
        else
            m_iPassNum = 0;

        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_ScytheM::Render_Shadow()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    /*
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
        return E_FAIL;*/


    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(2);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_ScytheM::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_ScytheM_Body"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_ScytheM::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_ScytheM* CBody_ScytheM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_ScytheM* pInstance = new CBody_ScytheM(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_ScytheM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_ScytheM::Clone(void* pArg)
{
    CBody_ScytheM* pInstance = new CBody_ScytheM(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_ScytheM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_ScytheM::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
