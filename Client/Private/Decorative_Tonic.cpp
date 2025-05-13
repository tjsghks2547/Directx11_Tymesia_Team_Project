#include "pch.h"
#include "Decorative_Tonic.h"
#include "GameInstance.h"

CDecorative_Tonic::CDecorative_Tonic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CDecorative_Tonic::CDecorative_Tonic(const CDecorative_Tonic& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CDecorative_Tonic::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDecorative_Tonic::Initialize(void* pArg)
{
    CDecorative_Tonic::DECORATIVE_TONIC_DESC* pDesc = static_cast<DECORATIVE_TONIC_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pHandSocketMatrix = pDesc->pHandSocketMatrix;
    m_bChange = pDesc->bChange;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

    return S_OK;
}

void CDecorative_Tonic::Priority_Update(_float fTimeDelta)
{

}

void CDecorative_Tonic::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = {};

    if (*m_bChange)
        SocketMatrix = XMLoadFloat4x4(m_pHandSocketMatrix);
    else
        SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
}

void CDecorative_Tonic::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CDecorative_Tonic::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        m_pShaderCom->Begin(17);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CDecorative_Tonic::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Tonic"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CDecorative_Tonic::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CDecorative_Tonic* CDecorative_Tonic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDecorative_Tonic* pInstance = new CDecorative_Tonic(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CDecorative_Tonic");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDecorative_Tonic::Clone(void* pArg)
{
    CDecorative_Tonic* pInstance = new CDecorative_Tonic(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CDecorative_Tonic");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDecorative_Tonic::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
