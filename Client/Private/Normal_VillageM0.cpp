#include "pch.h"
#include "Normal_VillageM0.h"
#include "GameInstance.h"
#include "Body_VillageM0.h"
#include "Weapon_Axe.h"
#include "Animation.h"
#include "Monster_HP_Bar.h"
#include "Locked_On.h"
#include "Player.h"

CNormal_VillageM0::CNormal_VillageM0(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CNormal_VillageM0::CNormal_VillageM0(const CNormal_VillageM0& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CNormal_VillageM0::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormal_VillageM0::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 10.f;
    m_fActive_Distance = 5.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects(pArg)))
        return E_FAIL;

    m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
    m_iSpawn_Cell_Index = m_pNavigationCom->Get_CurCellIndex();
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
    m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
    m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();

    m_pState_Manager = CState_Machine<CNormal_VillageM0>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 0.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);
    settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,22.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_AXE;

    return S_OK;
}

void CNormal_VillageM0::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CNormal_VillageM0::Update(_float fTimeDelta)
{

    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

}

void CNormal_VillageM0::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CNormal_VillageM0::Render()
{
    return S_OK;
}

HRESULT CNormal_VillageM0::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CNormal_VillageM0::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CBody_VillageM0::BODY_VillageM0_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_VillageM0"), LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0_Body"), &BodyDesc)))
        return E_FAIL;

    CWeapon_Axe::WEAPON_AXE_DESC		Weapon_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_VillageM0"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;
    Weapon_Desc.pParent = this;
    Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_Desc.pParentModel = m_pModelCom;
    Weapon_Desc.pParentState = &m_iMonster_State;
    Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
    Weapon_Desc.fSpeedPerSec = 0.f;
    Weapon_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Axe"), LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Axe"), &Weapon_Desc)))
        return E_FAIL;

    CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
    Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
    Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Locked_On_Desc.pParentState = &m_iMonster_State;
    Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
    Locked_On_Desc.fSpeedPerSec = 0.f;
    Locked_On_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
        return E_FAIL;

    CMonster_HP_Bar::Monster_HP_Bar_DESC Monster_HP_Bar_Desc = {};
    Monster_HP_Bar_Desc.pMonsterMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Monster_HP_Bar_Desc.fMaxHP = &m_fMonsterMaxHP;
    Monster_HP_Bar_Desc.fCurHP = &m_fMonsterCurHP;
    Monster_HP_Bar_Desc.fShieldHP = &m_fShieldHP;
    Monster_HP_Bar_Desc.bHP_Bar_Active = &m_bHP_Bar_Active;
    Monster_HP_Bar_Desc.bDead = &m_bDead;
    Monster_HP_Bar_Desc.fHeight = &m_fHP_Bar_Height;
    Monster_HP_Bar_Desc.fSpeedPerSec = 0.f;
    Monster_HP_Bar_Desc.fRotationPerSec = 0.f;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Bar"), iLevel, TEXT("Layer_MonsterHP"), &Monster_HP_Bar_Desc)))
        return E_FAIL;

    return S_OK;
}

void CNormal_VillageM0::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}

void CNormal_VillageM0::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 1.f && m_fDistance <= 1.5f)
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                m_pState_Manager->ChangeState(new Attack_01_State(), this);
                break;
            case 1:
                m_pState_Manager->ChangeState(new Attack_02_State(), this);
                break;
            }

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CNormal_VillageM0::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if ((!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name())) && m_fMonsterCurHP > 0.f)
    {
        m_fRecoveryTime = 0.f;
        m_bCanRecovery = false;
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= *m_Player_Attack / 4.f;
            m_fShieldHP -= (*m_Player_Attack) / 2.f;
        }
        else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 4.f;
            m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 8.f;
            if (m_fMonsterCurHP <= m_fShieldHP)
            {
                m_fMonsterCurHP = m_fShieldHP;
            }
        }

        if (m_bCanHit)
        {
            _uint iRandom = rand() % 2;
            while (true)
            {
                if (iRandom == m_iHit_Motion_Index)
                {
                    iRandom = rand() % 2;
                }
                else
                {
                    m_iHit_Motion_Index = iRandom;
                    break;
                }
            }
            m_pState_Manager->ChangeState(new CNormal_VillageM0::Hit_State(m_iHit_Motion_Index), this);
        }

        // 플레이어가 빌리지 남자M0 을 때릴 때 나는 소리.
        _uint iRandSoundFileNum = {};
        iRandSoundFileNum = rand() % 4 + 1;

        switch (iRandSoundFileNum)
        {
        case 1:
            m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;
        case 2:
            m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;
        case 3:
            m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;
        case 4:
            m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;
        }
    }
}

void CNormal_VillageM0::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CNormal_VillageM0::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CNormal_VillageM0* CNormal_VillageM0::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormal_VillageM0* pInstance = new CNormal_VillageM0(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CVillageM0");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormal_VillageM0::Clone(void* pArg)
{
    CNormal_VillageM0* pInstance = new CNormal_VillageM0(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CVillageM_0");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormal_VillageM0::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

void CNormal_VillageM0::Active()
{
    m_bActive = true;
    if (!m_bFirstActive)
    {
        m_bFirstActive = true;
        m_pState_Manager->ChangeState(new CNormal_VillageM0::Intro_State(), this);
    }
}

void CNormal_VillageM0::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new Return_To_SpawnPoint_State(), this);
}

void CNormal_VillageM0::Stun()
{
    m_pState_Manager->ChangeState(new CNormal_VillageM0::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트        
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

#pragma region Idle_State

void CNormal_VillageM0::Idle_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 30;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM0::Idle_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance >= 10.f && pObject->m_bActive)
    {
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    }
    else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance < 10.f && pObject->m_bActive)
    {
        pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
    }
}

void CNormal_VillageM0::Idle_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion

#pragma region Intro_State

void CNormal_VillageM0::Intro_State::State_Enter(CNormal_VillageM0* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == 32)
        m_iIndex = 33;
    else if (pObject->m_pModelCom->Get_Current_Animation_Index() == 36)
        m_iIndex = 37;

    pObject->m_bPatternProgress = true;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM0::Intro_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM0::Intro_State::State_Exit(CNormal_VillageM0* pObject)
{

}

#pragma endregion

#pragma region Move_State
void CNormal_VillageM0::Move_State::State_Enter(CNormal_VillageM0* pObject)
{
    if (pObject->m_fDistance > 1.f)
        m_iIndex = 43;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 46;
            break;
        case 1:
            m_iIndex = 47;
            break;
        case 2:
            m_iIndex = 41;
            break;
        }
    }
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}
void CNormal_VillageM0::Move_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{

    if (pObject->m_fDistance >= 3.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else if (pObject->m_fDistance < 3.f)
    {
        pObject->RotateDegree_To_Player();
        if (m_iIndex == 43 && pObject->m_fDistance > pObject->m_fRootDistance)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 41)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 46)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 47)
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CNormal_VillageM0::Move_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Run_State

void CNormal_VillageM0::Run_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 1;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    pObject->m_iMonster_State = STATE_RUN;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
}

void CNormal_VillageM0::Run_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
    if (pObject->m_fDistance >= 10.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());

    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->RotateDegree_To_Player();
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
}

void CNormal_VillageM0::Run_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion

#pragma region Stun_State
void CNormal_VillageM0::Stun_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 29;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_NORMAL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM0::Stun_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
    const _uint iCurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 28 && iCurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 27;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        if (pObject->m_bIsClosest && (*pObject->m_Player_State == CPlayer::STATE_LIGHT_EXECUTION_L || *pObject->m_Player_State == CPlayer::STATE_CLAW_LONG_PLUNDER_ATTACK2))
        {
            pObject->m_pState_Manager->ChangeState(new Dead_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 29 && iCurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && (*pObject->m_Player_State == CPlayer::STATE_LIGHT_EXECUTION_L || *pObject->m_Player_State == CPlayer::STATE_CLAW_LONG_PLUNDER_ATTACK2))
        {
            pObject->m_pState_Manager->ChangeState(new Dead_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 28;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 27 && iCurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_fShieldHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_IsStun = false;
        pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;

        pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
        pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);

        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }
}

void CNormal_VillageM0::Stun_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Attack_01_State

void CNormal_VillageM0::Attack_01_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 4;
    pObject->m_bCanHit = false;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 60;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM0::Attack_01_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
#pragma region SOUND_ATTACK01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "VillagerM_WhooshAxe0"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Villager_WhooshAxe0.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.8f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "VillagerM_Voice_Attack1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Attack1.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion

    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM0::Attack_01_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_02_State

void CNormal_VillageM0::Attack_02_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 5;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 60;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM0::Attack_02_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
#pragma region SOUND_ATTACK02
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "VillagerM_WhooshAxe2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Villager_WhooshAxe2.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.8f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "VillagerM_WhooshAxe4"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Villager_WhooshAxe4.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.8f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "VillagerM_Voice_Attack0"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Attack0.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion

    if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 99.f)
    {
        m_iIndex = 6;
        pObject->RotateDegree_To_Player();
        pObject->m_iMonster_Attack_Power = 45;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM0::Attack_02_State::State_Exit(CNormal_VillageM0* pObject)
{
}
#pragma endregion

#pragma region Hit_State

CNormal_VillageM0::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}
void CNormal_VillageM0::Hit_State::State_Enter(CNormal_VillageM0* pObject)
{
    switch (m_iHitNum)
    {
    case 0:
        m_iIndex = 25;
        break;
    case 1:
        m_iIndex = 26;
        break;
    }
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_HIT;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM0::Hit_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
#pragma region SOUND_HURT
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Villager_Hurt") || !strcmp(iter.szName, "Villager_Hurt2"))
                {
                    // 빌리지 남자M0 을 맞을 때 나는 소리.
                    _uint iRandSoundFileNum = {};
                    iRandSoundFileNum = rand() % 4 + 1;

                    switch (iRandSoundFileNum)
                    {
                    case 1:
                        pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Hurt_01.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                        break;
                    case 2:
                        pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Hurt_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                        break;
                    case 3:
                        pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Hurt_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                        break;
                    case 4:
                        pObject->m_pGameInstance->Play_Sound(L"VillagerM_Voice_Hurt_04.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.8f);
                        break;
                    }
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM0::Hit_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bPatternProgress = false;
}
#pragma endregion

#pragma region Dead_State
void CNormal_VillageM0::Dead_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 10;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->RotateDegree_To_Player();
    pObject->m_bCan_Move_Anim = true;


#pragma region UI상호작용
    // 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
    dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1148);
    pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1148);
    // 몬스터 사망 시 아이템 드랍 추가하기
    pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
    pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);

#pragma endregion

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


}

void CNormal_VillageM0::Dead_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
#pragma region EFFECT_EXECUTION
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
            {
                if (!strcmp(iter.szName, "Effect_Execution"))
                {
                    const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_NORMAL_EXECUTION_STAB, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                }
            }
        }
    }
#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_iMonster_State = STATE_DEAD;
        //죽을때의 처리를 여기서 하면될듯.
    }
}

void CNormal_VillageM0::Dead_State::State_Exit(CNormal_VillageM0* pObject)
{
}
#pragma endregion

void CNormal_VillageM0::Return_To_SpawnPoint_State::State_Enter(CNormal_VillageM0* pObject)
{
    m_iIndex = 43;
    pObject->m_fDelayTime = 0.f;
    pObject->m_bPatternProgress = true;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM0::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CNormal_VillageM0* pObject)
{
    pObject->m_pNavigationCom->Start_Astar(pObject->m_iSpawn_Cell_Index);
    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta, pObject->m_pNavigationCom);
    }
    if (pObject->m_fDistance <= 3.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }

    if (pObject->m_fSpawn_Distance <= 3.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }
}

void CNormal_VillageM0::Return_To_SpawnPoint_State::State_Exit(CNormal_VillageM0* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}
