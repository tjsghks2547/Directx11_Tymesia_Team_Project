#include "pch.h"
#include "Decorative_Hat.h"
#include "GameInstance.h"

CDecorative_Hat::CDecorative_Hat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CDecorative_Hat::CDecorative_Hat(const CDecorative_Hat& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CDecorative_Hat::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDecorative_Hat::Initialize(void* pArg)
{
    CDecorative_Hat::DECORATIVE_HAT_DESC* pDesc = static_cast<DECORATIVE_HAT_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3{ 0.4f, 0.4f, 0.4f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-120.f));

    _vector vPos = { -202.f,107.f,5.f,1.f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    return S_OK;
}

void CDecorative_Hat::Priority_Update(_float fTimeDelta)
{
    if (*m_pParentState == STATE_DEAD)
    {
        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;
    }
}

void CDecorative_Hat::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
}

void CDecorative_Hat::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CDecorative_Hat::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (*m_pParentState == STATE_DEAD)
        {
            m_iPassNum = 9;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
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

HRESULT CDecorative_Hat::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Hat"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CDecorative_Hat::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CDecorative_Hat* CDecorative_Hat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDecorative_Hat* pInstance = new CDecorative_Hat(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CDecorative_Hat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDecorative_Hat::Clone(void* pArg)
{
    CDecorative_Hat* pInstance = new CDecorative_Hat(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CDecorative_Hat");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDecorative_Hat::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
