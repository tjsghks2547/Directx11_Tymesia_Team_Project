#include "pch.h"
#include "Body_Varg.h"
#include "GameInstance.h"

CBody_Varg::CBody_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBody_Varg::CBody_Varg(const CBody_Varg& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBody_Varg::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Varg::Initialize(void* pArg)
{

    strcpy_s(m_szName, "VARG_BODY");

    CBody_Varg::BODY_VARG_DESC* pDesc = static_cast<CBody_Varg::BODY_VARG_DESC*>(pArg);


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->Set_BoneType("Bip001-Ponytail11",   BONE_TYPE::DYNAMIC_BONE, false); 
    m_pModelCom->Set_BoneType("Bip001-Ponytail12",   BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Ponytail13",   BONE_TYPE::DYNAMIC_BONE, false); 
    m_pModelCom->Set_BoneType("Bip001-Ponytail14",   BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Ponytail15",   BONE_TYPE::DYNAMIC_BONE, false); 
    m_pModelCom->Set_BoneType("Bip001-Ponytail16",   BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Ponytail17",   BONE_TYPE::DYNAMIC_BONE, false); 
    m_pModelCom->Set_BoneType("Bip001-Ponytail1Nub", BONE_TYPE::DYNAMIC_BONE, false);   



    m_pModelCom->Set_BoneType("Bip001-Xtra10",       BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra1002",     BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra1003",     BONE_TYPE::DYNAMIC_BONE, false);   
    m_pModelCom->Set_BoneType("Bip001-Xtra11",       BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra10Nub",    BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra11Opp",    BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra1102",     BONE_TYPE::DYNAMIC_BONE, false);   
    m_pModelCom->Set_BoneType("Bip001-Xtra11Opp02",  BONE_TYPE::DYNAMIC_BONE, false);    
    m_pModelCom->Set_BoneType("Bip001-Xtra1103",     BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra11Opp03",  BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra1104",     BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra11Opp04",  BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra11Nub",    BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra11OppNub", BONE_TYPE::DYNAMIC_BONE, false);




    m_pModelCom->SetUp_Animation(20, false);

    return S_OK;
}

void CBody_Varg::Priority_Update(_float fTimeDelta)
{
}

void CBody_Varg::Update(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));

}

void CBody_Varg::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody_Varg::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네
            return E_FAIL;

        if (i == 3)
        {
            m_pShaderCom->Begin(9);
            m_pModelCom->Render(i);
        }

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_Varg::Render_Shadow()
{
    return S_OK;
}

HRESULT CBody_Varg::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Varg_Body"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Varg::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_Varg* CBody_Varg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Varg* pInstance = new CBody_Varg(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Varg");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Varg::Clone(void* pArg)
{
    CBody_Varg* pInstance = new CBody_Varg(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Varg");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Varg::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
