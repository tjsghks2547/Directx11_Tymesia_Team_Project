#include "pch.h"
#include "Weapon_Magician2_Sword.h"
#include "Animation.h"
#include "GameInstance.h"
#include "Player.h"

CWeapon_Magician2_Sword::CWeapon_Magician2_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
}

CWeapon_Magician2_Sword::CWeapon_Magician2_Sword(const CWeapon_Magician2_Sword& Prototype)
    :CPartObject(Prototype)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
}

HRESULT CWeapon_Magician2_Sword::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Magician2_Sword::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    MAGICIAN2_SWORD_DESC* pDesc = static_cast<MAGICIAN2_SWORD_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_iMonster_Attack = pDesc->iAttack;
    m_pSocket_Matrix = pDesc->pSocketMatrix;
    m_bSpecial_Skill_Progress = pDesc->bSpecial_Skill_Progress;
    m_bCatch_Special_Attack = pDesc->bCatch_Special_Attack;

    m_pSocket_Hand_Matrix = m_pParentModelCom->Get_BoneMatrix("hand_l");
    m_pSocket_Mutation_Matrix = m_pParentModelCom->Get_BoneMatrix("Bone_Tentacle04");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));


    m_pActor[COLLIDER_SWORD] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 1.f,0.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_HAND] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_MUTATION] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,2.f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_SLASH] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 0.8f,0.8f,10.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_BURST] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 3.f,3.f,3.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_SPECIAL] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,2.f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);


    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SWORD], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_HAND], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_MUTATION], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SLASH], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_BURST], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SPECIAL], _fvector{ 0.f,0.f,100.f,1.f });


    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SWORD], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_HAND], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_MUTATION], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SLASH], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_BURST], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SPECIAL], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CWeapon_Magician2_Sword::Priority_Update(_float fTimeDelta)
{
    if (*m_pParentState == STATE_DEAD)
    {
        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;
    }
}

void CWeapon_Magician2_Sword::Update(_float fTimeDelta)
{
    if (*m_pParentState == STATE_INTRO)
        return;

    _matrix			SocketMatrix = {};

    SocketMatrix = XMLoadFloat4x4(m_pSocket_Matrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                //내가 넣은 콜라이더 시간에 진입했을때
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate)
                {
                    if (!strncmp(iter.szName, "Hand", strlen("Hand")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_HAND]);
                    }
                    else if (!strncmp(iter.szName, "Sword", strlen("Sword")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SWORD]);
                    }

                    else if (!strncmp(iter.szName, "Mutation", strlen("Mutation")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_MUTATION]);
                    }
                    else if (!strncmp(iter.szName, "Slash", strlen("Slash")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SLASH]);
                    }
                    else if (!strncmp(iter.szName, "Burst", strlen("Burst")))
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_BURST]);
                    }
                    else
                    {
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);
                    }


                    iter.isPlay = true;
                }
            }
            else
            {
                //내가 넣은 콜라이더 시간이 끝났을때나 플레이어한테 닿아서 데미지를 입혔을경우. 콜라이더를 꺼라.
                if ((iter.eType == EVENT_COLLIDER && !iter.isEventActivate) || m_bColliderOff)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_MUTATION]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SLASH]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_BURST]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);

                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                    {
                        iter.isPlay = false;
                    }
                }
            }

#pragma region Effect

            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT
            {
                if (!strcmp(iter.szName, "Weapon_Trail")) //Trail 시작해야하는 부분
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_MUTATION, &m_CombinedWorldMatrix);
                    }
                }
                if (!strcmp(iter.szName, "Weapon_Trail2")) //Trail 시작해야하는 부분
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_MUTATION, &m_CombinedWorldMatrix);
                    }
                }
            }
            else if (iter.eType == EVENT_EFFECT && iter.isEventActivate == false && true == iter.isPlay)
            {
                if (!strcmp(iter.szName, "Weapon_Trail")) //Trail이 꺼져야 하는 부분
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MUTATION);
                    iter.isPlay = false;
                }
                if (!strcmp(iter.szName, "Weapon_Trail2")) //Trail이 꺼져야 하는 부분
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MUTATION);
                    iter.isPlay = false;
                }
            }

#pragma endregion
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_MUTATION]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SLASH]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_BURST]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SWORD])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SWORD], XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ -100.f, 0.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_HAND])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_HAND], XMLoadFloat4x4(m_pSocket_Hand_Matrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 0.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_MUTATION])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_MUTATION], XMLoadFloat4x4(m_pSocket_Mutation_Matrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SPECIAL])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SPECIAL], XMLoadFloat4x4(m_pSocket_Mutation_Matrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SLASH])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SLASH], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 500.f, 3900.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_BURST])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_BURST], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 300.f, 0.f,1.f });

}

void CWeapon_Magician2_Sword::Late_Update(_float fTimeDelta)
{
    if (*m_pParentState != STATE_INTRO)
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Magician2_Sword::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (*m_pParentState == STATE_DEAD)
        {
            m_iPassNum = 9;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 6)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
                return E_FAIL;
        }
        else
            m_iPassNum = 17;// 0;


        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Magician2_Sword::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Magician2_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Magician2_Sword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;

        if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
        {
            _uint iRandSoundFileNum = {};
            iRandSoundFileNum = rand() % 4;

            switch (iRandSoundFileNum)
            {
            case 0:
                m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 2:
                m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 3:
                m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            }
        }
    }
    if (!strcmp("PLAYER", _pOther->Get_Name()) && *m_bSpecial_Skill_Progress)
        *m_bCatch_Special_Attack = true;
}

void CWeapon_Magician2_Sword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Magician2_Sword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Magician2_Sword* CWeapon_Magician2_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Magician2_Sword* pInstance = new CWeapon_Magician2_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Magician2_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Magician2_Sword::Clone(void* pArg)
{
    CWeapon_Magician2_Sword* pInstance = new CWeapon_Magician2_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Magician2_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Magician2_Sword::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
