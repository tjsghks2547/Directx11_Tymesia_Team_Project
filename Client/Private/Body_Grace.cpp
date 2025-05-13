#include "pch.h"
#include "Body_Grace.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Elite_Grace.h"
#include "Player.h"

CBody_Grace::CBody_Grace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBody_Grace::CBody_Grace(const CBody_Grace& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBody_Grace::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Grace::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    CBody_Grace::BODY_GRACE_DESC* pDesc = static_cast<CBody_Grace::BODY_GRACE_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_iMonster_Attack = pDesc->iAttack;
    m_bDead = pDesc->bDead;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pSocketMatrix[COLLIDER_HAND_RIGHT] = m_pModelCom->Get_BoneMatrix("ik_hand_r");
    m_pSocketMatrix[COLLIDER_HAND_LEFT] = m_pModelCom->Get_BoneMatrix("ik_hand_l");
    m_pSocketMatrix[COLLIDER_KICK_RIGHT] = m_pModelCom->Get_BoneMatrix("ik_foot_r");
    m_pSocketMatrix[COLLIDER_KICK_LEFT] = m_pModelCom->Get_BoneMatrix("ik_foot_l");

    m_pModelCom->SetUp_Animation(10, true);

    m_pActor[COLLIDER_HAND_RIGHT] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.6f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_HAND_LEFT] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.6f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_KICK_RIGHT] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.6f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_KICK_LEFT] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.6f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_PARRY] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_HAND_RIGHT], _fvector{ 0.f,0.f,90.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_HAND_LEFT], _fvector{ 0.f,0.f,90.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_KICK_RIGHT], _fvector{ 0.f,0.f,92.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_KICK_LEFT], _fvector{ 0.f,0.f,92.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_PARRY], _fvector{ 0.f,0.f,92.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_HAND_RIGHT], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_HAND_LEFT], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_KICK_RIGHT], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_KICK_LEFT], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_PARRY], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CBody_Grace::Priority_Update(_float fTimeDelta)
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

void CBody_Grace::Update(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true)
                {
                    if (!strncmp(iter.szName, "RightHand", strlen("RightHand")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_HAND_RIGHT]);
                    }
                    else if (!strncmp(iter.szName, "LeftHand", strlen("LeftHand")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_HAND_LEFT]);
                    }
                    else if (!strncmp(iter.szName, "RightKick", strlen("RightKick")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_KICK_RIGHT]);
                    }
                    else if (!strncmp(iter.szName, "LeftKick", strlen("LeftKick")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_KICK_LEFT]);
                    }
                    else if (!strncmp(iter.szName, "Parry", strlen("Parry")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_PARRY]);
                    }
                    iter.isPlay = true;
                }
            }
            else
            {
                if ((iter.eType == EVENT_COLLIDER && iter.isEventActivate == false) || m_bColliderOff == true)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND_RIGHT]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND_LEFT]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK_RIGHT]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK_LEFT]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_PARRY]);

                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                        iter.isPlay = false;
                }
            }
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND_RIGHT]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND_LEFT]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK_RIGHT]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK_LEFT]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_PARRY]);
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_HAND_RIGHT])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_HAND_RIGHT], XMLoadFloat4x4(m_pSocketMatrix[COLLIDER_HAND_RIGHT]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_HAND_LEFT])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_HAND_LEFT], XMLoadFloat4x4(m_pSocketMatrix[COLLIDER_HAND_LEFT]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_KICK_RIGHT])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_KICK_RIGHT], XMLoadFloat4x4(m_pSocketMatrix[COLLIDER_KICK_RIGHT]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_KICK_LEFT])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_KICK_LEFT], XMLoadFloat4x4(m_pSocketMatrix[COLLIDER_KICK_LEFT]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_PARRY])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_PARRY], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 500.f,0.f,1.f });

}

void CBody_Grace::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CBody_Grace::Render()
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
        else if (i == 2)
            m_iPassNum = 3;
        else
            m_iPassNum = 0;

        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_Grace::Render_Shadow()
{
    return S_OK;
}

HRESULT CBody_Grace::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;
    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Grace_Body"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Grace::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CBody_Grace::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;

        if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
        {
            _uint iHitRandom = rand() % 3;

            switch (iHitRandom)
            {
            case 0:
                m_pGameInstance->Play_Sound(TEXT("Foot_Hit_01.ogg"), CHANNELID::SOUND_MONSTER_WEAPON, 0.4f);
                break;

            case 1:
                m_pGameInstance->Play_Sound(TEXT("Foot_Hit_02.ogg"), CHANNELID::SOUND_MONSTER_WEAPON, 0.4f);
                break;

            case 2:
                m_pGameInstance->Play_Sound(TEXT("Foot_Hit_03.ogg"), CHANNELID::SOUND_MONSTER_WEAPON, 0.4f);
                break;
            }
        }
    }
}

void CBody_Grace::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBody_Grace::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBody_Grace* CBody_Grace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Grace* pInstance = new CBody_Grace(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Grace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Grace::Clone(void* pArg)
{
    CBody_Grace* pInstance = new CBody_Grace(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Grace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Grace::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
