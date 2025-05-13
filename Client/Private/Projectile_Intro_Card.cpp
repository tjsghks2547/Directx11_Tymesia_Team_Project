#include "pch.h"
#include "Projectile_Intro_Card.h"
#include "GameInstance.h"

CProjectile_Intro_Card::CProjectile_Intro_Card(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CProjectile_Intro_Card::CProjectile_Intro_Card(const CProjectile_Intro_Card& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CProjectile_Intro_Card::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CProjectile_Intro_Card::Initialize(void* pArg)
{
    PROJECTILE_INTRO_CARD_DESC* pDesc = static_cast<PROJECTILE_INTRO_CARD_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentActive = pDesc->bActive;
    m_pRender = pDesc->bRender;
    m_pChangeModel = pDesc->bChangeModel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pTransformCom->Scaling(_float3{ 10.4f,10.4f, 10.4f });
    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

    return S_OK;
}

void CProjectile_Intro_Card::Priority_Update(_float fTimeDelta)
{
    if (*m_pParentState != STATE_INTRO || !*m_pRender)
        return;

}


void CProjectile_Intro_Card::Update(_float fTimeDelta)
{
    if (*m_pParentState != STATE_INTRO || !*m_pRender)
        return;


    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
}

void CProjectile_Intro_Card::Late_Update(_float fTimeDelta)
{
    if (*m_pParentState != STATE_INTRO || !*m_pRender)
        return;

    if (!*m_pChangeModel)
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
    else
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CProjectile_Intro_Card::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _bool bTest = false;
    _uint			iNumMeshes = m_pExodia_Card_ModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pExodia_Card_ModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pExodia_Card_ModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (FAILED(m_pShaderCom->Bind_RawValue("g_NeedNormal", &bTest, sizeof(_bool))))
            return E_FAIL;

        m_pShaderCom->Begin(17);
        m_pExodia_Card_ModelCom->Render(i);
    }

    return S_OK;
}


HRESULT CProjectile_Intro_Card::Render_Glow()
{
    if (*m_pParentState != STATE_INTRO || !*m_pRender)
        return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        m_pShaderCom->Begin(15);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CProjectile_Intro_Card::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Projectile_Card"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Exodia_Card"),
        TEXT("Com_Model_Exodia"), reinterpret_cast<CComponent**>(&m_pExodia_Card_ModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CProjectile_Intro_Card::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CProjectile_Intro_Card* CProjectile_Intro_Card::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CProjectile_Intro_Card* pInstance = new CProjectile_Intro_Card(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CProjectile_Intro_Card");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CProjectile_Intro_Card::Clone(void* pArg)
{
    CProjectile_Intro_Card* pInstance = new CProjectile_Intro_Card(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CProjectile_Intro_Card");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CProjectile_Intro_Card::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pExodia_Card_ModelCom);
}
