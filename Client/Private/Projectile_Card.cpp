#include "pch.h"
#include "Projectile_Card.h"
#include "GameInstance.h"

CProjectile_Card::CProjectile_Card(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CProjectile(pDevice, pContext)
{
}

CProjectile_Card::CProjectile_Card(const CProjectile_Card& Prototype)
    :CProjectile(Prototype)
{
}

HRESULT CProjectile_Card::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CProjectile_Card::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.3f,0.3f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CProjectile_Card::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    if (m_bIsFire)
        m_pGameInstance->Add_Actor_Scene(m_pActor);
    else
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
}

void CProjectile_Card::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f, 0.f,1.f });
}

void CProjectile_Card::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
}

HRESULT CProjectile_Card::Render_Glow()
{
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

void CProjectile_Card::Set_Projectile_Effect()
{
    //m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_PROJECTILE, m_pTransformCom->Get_WorldMatrix_Ptr());
}

void CProjectile_Card::Stop_Projectile_Effect()
{
    //m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PROJECTILE);
}

//HRESULT CProjectile_Card::Render()
//{
//    if (FAILED(Bind_ShaderResources()))
//        return E_FAIL;
//
//    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();
//
//    for (_uint i = 0; i < iNumMeshes; i++)
//    {
//        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
//            return E_FAIL;
//
//        m_pShaderCom->Begin(0);
//        m_pModelCom->Render(i);
//    }
//
//    return S_OK;
//}

HRESULT CProjectile_Card::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Projectile_Card"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CProjectile_Card::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CProjectile_Card::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()) || !strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
    {
        Set_IsFire(false);
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
        m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_CARD_DAMAGE, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    }
}

void CProjectile_Card::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile_Card::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}


CProjectile_Card* CProjectile_Card::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CProjectile_Card* pInstance = new CProjectile_Card(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CProjectile_Card");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CProjectile_Card::Clone(void* pArg)
{
    CProjectile_Card* pInstance = new CProjectile_Card(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CProjectile_Card");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CProjectile_Card::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}