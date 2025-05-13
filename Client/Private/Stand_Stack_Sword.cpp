#include "pch.h"
#include "Stand_Stack_Sword.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Player.h"

CStand_Stack_Sword::CStand_Stack_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CStand_Stack_Sword::CStand_Stack_Sword(const CStand_Stack_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CStand_Stack_Sword::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CStand_Stack_Sword::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");
    STAND_STACK_SWORD_DESC* pDesc = static_cast<STAND_STACK_SWORD_DESC*>(pArg);

    m_vPlayer_Position = pDesc->pPlayerPos;
    m_bNeed_Memory_Position = pDesc->bNeed_Memory_Position;
    m_bIs_Equipped_To_LeftHand = pDesc->bIs_Equipped_To_LeftHand;
    m_bIs_Stand_In_Ground = pDesc->bIs_Stand_In_Ground;
    m_bCollider_Change = pDesc->bCollider_Change;
    m_pParentState = pDesc->pParentState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentModelCom = pDesc->pParentModel;
    m_iCurrent_StackCount = pDesc->iCurrent_StackCount;
    m_iStack_Number = pDesc->iStack_Number;
    m_iMonster_Attack = pDesc->iAttack;

    m_pLeft_SocketMatrix = m_pParentModelCom->Get_BoneMatrix("weapon_l");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3(0.5f, 0.5f, 0.5f));

    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.15f }, _float3{ 1.f,0.f,0.f }, 0.f, this);
    m_pLargeActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pLargeActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pLargeActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);


    return S_OK;
}

void CStand_Stack_Sword::Priority_Update(_float fTimeDelta)
{
        //else if (!strcmp(iter.szName, "Effect_StackSword_1"))
     //{
     //  _float4 vPos = { m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f };
     //  m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_URD_STACK_SWORD_SURFACE, XMLoadFloat4(&vPos));
     //  m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_STACKSWORD_EXPLOSION, XMLoadFloat4(&vPos));
     //  m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_STACKSWORD_EXPLOSION, XMLoadFloat4(&vPos));
     //  m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_STACKSWORD_EXPLOSION, XMLoadFloat4(&vPos));
	 // m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_SPARK);
     //	iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
     //}
}

void CStand_Stack_Sword::Update(_float fTimeDelta)
{
    Store_CombinedMatrix();
    //왼손에 있지도 땅에 있지도 날아갈 필요도 없으면 그냥 허리에 차있으면 돼
    if (!*m_bIs_Equipped_To_LeftHand && !*m_bIs_Stand_In_Ground)
    {
        _matrix			SocketMatrix = {};
        SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
        XMStoreFloat4x4(&m_CombinedWorldMatrix,
            XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
            SocketMatrix *
            XMLoadFloat4x4(m_pParentWorldMatrix)
        );
    }
    else if (*m_bIs_Equipped_To_LeftHand)
    {
        //왼손에 있어야 할 타이밍이면 socket 왼손으로 바꿔
        _matrix			SocketMatrix = {};
        SocketMatrix = XMLoadFloat4x4(m_pLeft_SocketMatrix);
        XMStoreFloat4x4(&m_CombinedWorldMatrix,
            XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
            SocketMatrix *
            XMLoadFloat4x4(m_pParentWorldMatrix)
        );
    }
    else if (*m_bIs_Stand_In_Ground)
    {
        //땅에 박혀 있어야할때 땅에 박혀있었을때의 컴바인드 넣어
        m_CombinedWorldMatrix = m_Store_CombinedMatrix;
    }

#pragma region Effect

    for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            switch (m_iStack_Number)
            {
            case 0:
                if (!strcmp(iter.szName, "Effect_StackSword0"))
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_SPARK);

                    m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_1, &m_Store_CombinedMatrix, m_pTransformCom->Get_WorldMatrix_Ptr());
                    _vector vPos = { m_Store_CombinedMatrix._41, m_Store_CombinedMatrix._42, m_Store_CombinedMatrix._43, 1.f };
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_EXPLOSION, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_2, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_2, vPos);
                    iter.isPlay = true;
                }
                break;
            case 1:
                if (!strcmp(iter.szName, "Effect_StackSword1"))
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_SPARK);

                    m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_1, &m_Store_CombinedMatrix, m_pTransformCom->Get_WorldMatrix_Ptr());
                    _vector vPos = { m_Store_CombinedMatrix._41, m_Store_CombinedMatrix._42, m_Store_CombinedMatrix._43, 1.f };
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_EXPLOSION, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_2, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_2, vPos);
                    iter.isPlay = true;
                }
                break;
            case 2:
                if (!strcmp(iter.szName, "Effect_StackSword2"))
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_SPARK);

                    m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_1, &m_Store_CombinedMatrix, m_pTransformCom->Get_WorldMatrix_Ptr());
                    _vector vPos = { m_Store_CombinedMatrix._41, m_Store_CombinedMatrix._42, m_Store_CombinedMatrix._43, 1.f };
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_EXPLOSION, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_STACKSWORD_WORLD_EXPLOSION_2, vPos);
                    m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_STACKSWORD_EXPLOSION_2, vPos);
                    iter.isPlay = true;
                }
                break;
            }
        }
    }

#pragma endregion

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD && *m_bCollider_Change)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                //내가 넣은 콜라이더 시간에 진입했을때
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate)
                {
                    if (!strncmp(iter.szName, (string("Special_Collider") + to_string(m_iStack_Number)).c_str(), strlen((string("Special_Collider") + to_string(m_iStack_Number)).c_str())))
                    {
                        iter.isPlay = true;
                        m_pGameInstance->Add_Actor_Scene(m_pLargeActor);
                    }
                    //스킬 콜라이더이고, 현재 우르드가 꽂아야하는 칼 인덱스랑 내 칼인덱스랑 같을때이며, 땅에 꽂혀있지 않을때
                    else if (!strncmp(iter.szName, "Skill_Collider", strlen("Skill_Collider")) &&
                        m_iStack_Number == *m_iCurrent_StackCount &&
                        !*m_bIs_Stand_In_Ground)
                    {
                        iter.isPlay = true;
                        m_pGameInstance->Add_Actor_Scene(m_pActor);
                    }
                }
            }
            else
            {
                //내가 넣은 콜라이더 시간이 끝났을때나 플레이어한테 닿아서 데미지를 입혔을경우. 콜라이더를 꺼라.
                if ((iter.eType == EVENT_COLLIDER && !iter.isEventActivate) || m_bColliderOff)
                {
                    if (!strncmp(iter.szName, (string("Special_Collider") + to_string(m_iStack_Number)).c_str(),
                        strlen((string("Special_Collider") + to_string(m_iStack_Number)).c_str())))
                    {
                        m_pGameInstance->Sub_Actor_Scene(m_pLargeActor);
                        m_pGameInstance->Sub_Actor_Scene(m_pActor);

                        m_bColliderOff = false;
                        if (!iter.isEventActivate)
                        {
                            iter.isPlay = false;
                        }
                    }

                    else if (!strncmp(iter.szName, "Skill_Collider", strlen("Skill_Collider")) &&
                        m_iStack_Number == *m_iCurrent_StackCount)
                    {
                        m_pGameInstance->Sub_Actor_Scene(m_pLargeActor);
                        m_pGameInstance->Sub_Actor_Scene(m_pActor);
                        m_bColliderOff = false;
                        if (!iter.isEventActivate)
                        {
                            iter.isPlay = false;
                        }
                    }
                }
            }
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
        m_pGameInstance->Sub_Actor_Scene(m_pLargeActor);
    }


    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 0.f, 0.f, 0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pLargeActor)))
        m_pGameInstance->Update_Collider(m_pLargeActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 0.f, 500.f, 0.f,1.f });
}

void CStand_Stack_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CStand_Stack_Sword::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        m_pShaderCom->Begin(17); // 0
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CStand_Stack_Sword::Ready_Components(void* pArg)
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Urd_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CStand_Stack_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CStand_Stack_Sword::Store_CombinedMatrix()
{
    if (*m_bNeed_Memory_Position)
    {
        *m_bNeed_Memory_Position = false;
        m_Store_CombinedMatrix = m_CombinedWorldMatrix;

        _vector vPos = XMLoadFloat4x4(&m_Store_CombinedMatrix).r[3];
        m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_URD_STACK_SWORD_SURFACE, vPos);
        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_SKILL_SPARK, &m_Store_CombinedMatrix, m_pTransformCom->Get_WorldMatrix_Ptr());
    }
}

void CStand_Stack_Sword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;

        if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
        {
            _uint iRandSoundFileNum = {};
            iRandSoundFileNum = rand() % 2;

            switch (iRandSoundFileNum)
            {
            case 0:
                m_pGameInstance->Play_Sound(L"Player_DamageByJoker1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Player_DamageByJoker2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            }
        }
    }
}

void CStand_Stack_Sword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CStand_Stack_Sword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CStand_Stack_Sword* CStand_Stack_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStand_Stack_Sword* pInstance = new CStand_Stack_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CProjectile_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStand_Stack_Sword::Clone(void* pArg)
{
    CStand_Stack_Sword* pInstance = new CStand_Stack_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CProjectile_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStand_Stack_Sword::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
