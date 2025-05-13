#include "pch.h"
#include "VargKnife.h"
#include "Boss_Varg.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Camera_Free.h"
#include "Player.h"

CVargKnife::CVargKnife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
}

CVargKnife::CVargKnife(const CVargKnife& Prototype)
    :CPartObject(Prototype)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
}

HRESULT CVargKnife::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CVargKnife::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    VARGKNIFE_DESC* pDesc = static_cast<VARGKNIFE_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_iMonster_Attack = pDesc->iAttack;
    m_Is_Catch = pDesc->Is_Catch;

    m_pSocket_Hand_Matrix = m_pParentModelCom->Get_BoneMatrix("Bip001-L-Hand");


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3{ 0.4f, 0.4f, 0.4f });

    m_pActor[COLLIDER_SWORD] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.8f,0.15f }, _float3{ 0.f,1.f,0.f }, 90.f, this);
    m_pActor[COLLIDER_STUN] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 5.f,5.f,5.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_HAND] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.6f,0.5f,0.5f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SWORD], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_HAND], _fvector{ 0.f,0.f,102.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_STUN], _fvector{ 0.f,0.f,101.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SWORD], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_HAND], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_STUN], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);


    return S_OK;
}

void CVargKnife::Priority_Update(_float fTimeDelta)
{
    if (m_iPreAnimIndex != m_pParentModelCom->Get_Current_Animation_Index())
    {
        m_bColliderOff = false;
        m_iPreAnimIndex = m_pParentModelCom->Get_Current_Animation_Index();
    }

    if (m_pCamera == nullptr)
        m_pCamera = static_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Layer_Camera"), "Camera_Free"));

}

void CVargKnife::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );


    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_STATE && iter.isEventActivate == true)  
                {
                    if (!strcmp(iter.szName, "Camera_Shake"))   
                    {
                        m_pCamera->ShakeOn(600.f, 600.f, 10.f, 10.f);   
                    }   
                }   

                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true) // EVENT_COLLIDER 부분      
                {
                    // 그 구간에서는 계속 진행 

                    if (*m_pParentState == STATE_SPECIAL_ATTACK)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_STUN]);

                    else if (*m_pParentState == STATE_SPECIAL_ATTACK2)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_HAND]);

                    else
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SWORD]);

                    iter.isPlay = true;
                }
                
            }
            else
            {
                if ((iter.eType == EVENT_COLLIDER && iter.isEventActivate == false) || m_bColliderOff == true) // EVENT_COLLIDER 부분      
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_STUN]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND]);

                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                        iter.isPlay = false;

                }
                
            }


            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT
            {
                if (!strcmp(iter.szName, "Weapon_Trail")) //Trail 시작해야하는 부분
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_VARG, &m_CombinedWorldMatrix);
                    }
                }
                else if (!strcmp(iter.szName, "Weapon_Effect_Vertical_Dust"))
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_VERTICAL_DUST, vPos, vDir);
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, *m_pParentWorldMatrix);
                    }
                }
                else if (!strcmp(iter.szName, "Weapon_Effect_Narrow_Dust"))
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_NARROW_DUST_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_NARROW_SPARK_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, *m_pParentWorldMatrix);
                    }
                }
                else if (!strcmp(iter.szName, "Weapon_Effect_Horizon_Dust"))
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_HORIZON_DUST_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_HORIZON_SPARK_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, *m_pParentWorldMatrix);
                    }
                }
                else if (!strcmp(iter.szName, "Weapon_Effect_Left_Dust"))
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_LEFT_DUST_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, *m_pParentWorldMatrix);
                    }
                }
                else if (!strcmp(iter.szName, "Weapon_Effect_Right_Dust"))
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_RIGHT_DUST_VARG, vPos, vDir);
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, *m_pParentWorldMatrix);
                    }
                }
            }
            else if (iter.eType == EVENT_EFFECT && iter.isEventActivate == false && true == iter.isPlay)
            {
                if (!strcmp(iter.szName, "Weapon_Trail")) //Trail이 꺼져야 하는 부분
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_VARG);
                    iter.isPlay = false;
                }
            }
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_STUN]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_HAND]);
    }
#pragma endregion  

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SWORD])))
    {
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SWORD], XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f,-350.f,1.f });
    }
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_HAND])))
    {
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_HAND], XMLoadFloat4x4(m_pSocket_Hand_Matrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{0.f, 0.f,0.f,1.f});
        XMStoreFloat4x4(&m_HandWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(m_pSocket_Hand_Matrix), XMLoadFloat4x4(m_pParentWorldMatrix))); 
    }
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_STUN])))
    {
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_STUN], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 0.f,0.f,1.f });
    }

}

void CVargKnife::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CVargKnife::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    //for (_uint i = 0; i < iNumMeshes; i++)
    //{

    if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, 0, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
        return E_FAIL;

    m_pShaderCom->Begin(17);
    m_pModelCom->Render(0);
    //}

    return S_OK;
}


HRESULT CVargKnife::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Varg_Knife"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CVargKnife::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CVargKnife::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;
        if (*m_pParentState == STATE_SPECIAL_ATTACK2)
        {
            *m_Is_Catch = true;
        }
        else
        {
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
    }
}

void CVargKnife::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CVargKnife::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CVargKnife* CVargKnife::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVargKnife* pInstance = new CVargKnife(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVargKnife::Clone(void* pArg)
{
    CVargKnife* pInstance = new CVargKnife(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVargKnife::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
