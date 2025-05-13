#include "pch.h"
#include "Bat_Spike.h"
#include "GameInstance.h"

CBat_Spike::CBat_Spike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBat_Spike::CBat_Spike(const CBat_Spike& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBat_Spike::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBat_Spike::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER");

    BAT_SPIKE_DESC* pDesc = static_cast<BAT_SPIKE_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pRender = pDesc->bRender;
    m_pPivot_Position = pDesc->pPivot_Position;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.8f,0.15f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::MONSTER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::DESTRUCT, settingColliderGroup);

    m_fMaxScale = { 0.003f,0.003f,0.003f };
    m_fMyScale = { 0.003f,0.00001f,0.003f };

    return S_OK;
}

void CBat_Spike::Priority_Update(_float fTimeDelta)
{
    if (*m_pRender)
        Spawn(fTimeDelta);
    else
        Reset(fTimeDelta);
}

void CBat_Spike::Update(_float fTimeDelta)
{
   /* if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f, 0.f,1.f });*/

    if (!m_bUpdating)
    {
        if (m_bHitted)
        {
            m_fLifeTime += fTimeDelta;
            m_pGameInstance->Sub_Actor_Scene(m_pActor);
            m_fExplosionPower += fTimeDelta * 3.0f;
            m_fFallingTime += fTimeDelta;

            if (m_fLifeTime >= 4.0f)
            {
                m_bUpdating = true;
            }
        }
        if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
            m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f, 0.f,1.f });
    }
}

void CBat_Spike::Late_Update(_float fTimeDelta)
{
    if (*m_pRender)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
    }
}

HRESULT CBat_Spike::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_EmissiveTexture", 0)))
            return E_FAIL;

        //m_pShaderCom->Begin(21);
        m_pShaderCom->Begin(4);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

void CBat_Spike::Spawn(_float fTimeDelta)
{
    m_pGameInstance->Add_Actor_Scene(m_pActor);
    m_fLinear += fTimeDelta * 0.2f;
    if (m_fLinear >= 1.f)
        m_fLinear = 1.f;
    _float3 fNewScale = {};
    _vector vNewScale = XMVectorLerp(XMLoadFloat3(&m_fMyScale), XMLoadFloat3(&m_fMaxScale), m_fLinear);
    XMStoreFloat3(&fNewScale, vNewScale);
    m_pTransformCom->Scaling(fNewScale);
    if (!m_bFirst)
    {
        m_bFirst = true;
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(m_pPivot_Position));

        XMStoreFloat4(&m_fModelPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        m_fModelHeightCenterY = m_fModelPos.y + 0.76f;
    }
}

void CBat_Spike::Reset(_float fTimeDelta)
{
    m_bFirst = false;
    m_pGameInstance->Sub_Actor_Scene(m_pActor);
    m_fLinear = 0.f;
    m_fLifeTime = 0.f;
    m_fExplosionPower = 0.f;
    m_fFallingTime = 0.f;
    m_bUpdating = false;
    m_bHitted = false;
    _float3 fNewScale = {};
    _vector vNewScale = XMVectorLerp(XMLoadFloat3(&m_fMyScale), XMLoadFloat3(&m_fMaxScale), m_fLinear);
    XMStoreFloat3(&fNewScale, vNewScale);
    m_pTransformCom->Scaling(fNewScale);
}

HRESULT CBat_Spike::Ready_Components()
{
    /* Com_Shader */
    //if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
    //    TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
    //    return E_FAIL;


    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxDestructMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Spike"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Emissive"),
        TEXT("Com_Emissive"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBat_Spike::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    // =========================== 지메쉐로 드가자아잇 ===========================
    // 얼마나 세게
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fExplosionPower", &m_fExplosionPower, sizeof(_float))))
        return E_FAIL;

    // 얼마나 오래 떨어질꺼?
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFallingTime", &m_fFallingTime, sizeof(_float))))
        return E_FAIL;

    // 모델의 위치점 (처음 피킹 배치 될 때 그 x y z 를 지메쉐에서 나눠서 쓸 꺼임
    if (FAILED(m_pShaderCom->Bind_RawValue("g_ModelPosition", &m_fModelPos, sizeof(_float4))))
        return E_FAIL;

    // 모델의 y 중심점 (등분 할 때 쓸꺼임) 
    if (FAILED(m_pShaderCom->Bind_RawValue("g_fModelHeightCenterY", &m_fModelHeightCenterY, sizeof(_float))))
        return E_FAIL;




    return S_OK;
}

void CBat_Spike::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("MONSTER_WEAPON", _pOther->Get_Name()) && !m_bHitted)
    {
        m_bHitted = true;
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_pGameInstance->Play_Sound(TEXT("BatSpike_Destory_01"), CHANNELID::SOUND_STRUCT, 0.3f);
            break;
        case 1:
            m_pGameInstance->Play_Sound(TEXT("BatSpike_Destory_02"), CHANNELID::SOUND_STRUCT, 0.3f);
            break;
        case 2:
            m_pGameInstance->Play_Sound(TEXT("BatSpike_Destory_03"), CHANNELID::SOUND_STRUCT, 0.3f);
            break;
        }
    }
}

void CBat_Spike::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBat_Spike::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBat_Spike* CBat_Spike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBat_Spike* pInstance = new CBat_Spike(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBat_Spike");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBat_Spike::Clone(void* pArg)
{
    CBat_Spike* pInstance = new CBat_Spike(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBat_Spike");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBat_Spike::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
