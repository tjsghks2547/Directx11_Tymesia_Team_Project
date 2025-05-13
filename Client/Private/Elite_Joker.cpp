#include "pch.h"
#include "Elite_Joker.h"
#include "GameInstance.h"
#include "Body_Joker.h"
#include "Joker_Weapon.h"
#include "Player.h"
#include "Animation.h"
#include "Monster_HP_Bar.h"
#include "Locked_On.h"

CElite_Joker::CElite_Joker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CElite_Joker::CElite_Joker(const CElite_Joker& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CElite_Joker::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElite_Joker::Initialize(void* pArg)
{

    m_fHP_Bar_Height = 800.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 15.f;
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

    m_pState_Manager = CState_Machine<CElite_Joker>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.5f,0.5f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);


    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,25.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_HALBERD;

    return S_OK;
}

void CElite_Joker::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CElite_Joker::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CElite_Joker::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CElite_Joker::Render()
{
    return S_OK;
}

HRESULT CElite_Joker::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CElite_Joker::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CBody_Joker::BODY_JOKER_DESC BodyDesc{};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.bActive = &m_bActive;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Joker"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker_Body"), &BodyDesc)))
        return E_FAIL;

    CJoker_Weapon::JOKER_WEAPON_DESC		Joker_Weapon_Desc{};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Joker"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    Joker_Weapon_Desc.pParent = this;
    Joker_Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_Hammer");
    Joker_Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Joker_Weapon_Desc.pParentModel = m_pModelCom;
    Joker_Weapon_Desc.pParentState = &m_iMonster_State;
    Joker_Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
    Joker_Weapon_Desc.fSpeedPerSec = 0.f;
    Joker_Weapon_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Joker_Weapon"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker_Weapon"), &Joker_Weapon_Desc)))
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

void CElite_Joker::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}

void CElite_Joker::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 2.f)
        {
            if (m_fDistance >= 5.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }

}

void CElite_Joker::Active()
{
    m_pState_Manager->ChangeState(new CElite_Joker::Intro_State(), this);
}

void CElite_Joker::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CElite_Joker::Return_To_SpawnPoint_State(), this);
}

void CElite_Joker::Stun()
{
    m_pState_Manager->ChangeState(new CElite_Joker::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CElite_Joker::Near_Pattern_Create()
{
    _uint iRandomPattern = rand() % 5;
    while (true)
    {
        if (iRandomPattern == m_iNearPatternIndex)
        {
            iRandomPattern = rand() % 5;
        }
        else
        {
            m_iNearPatternIndex = iRandomPattern;
            break;
        }
    }
    switch (m_iNearPatternIndex)
    {
    case 0:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Combo_A(), this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Combo_B(), this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Wheel(), this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Shock(), this);
        break;
    case 4:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Strong(), this);
        break;
    default:
        break;
    }

}

void CElite_Joker::Far_Pattern_Create()
{
    m_iFarPatternIndex = rand() % 2;
    switch (m_iFarPatternIndex)
    {
    case 0:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Run(), this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Joker::Attack_Jump(), this);
        break;
    }
}

void CElite_Joker::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if ((!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name())) && m_fMonsterCurHP > 0.f)
    {
        m_fRecoveryTime = 0.f;
        m_bCanRecovery = false;
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= *m_Player_Attack / 50.f;
            m_fShieldHP -= (*m_Player_Attack / 10.f);

#pragma region Sound
            _uint iRandSoundFileNum = {};
            iRandSoundFileNum = rand() % 2;

            switch (iRandSoundFileNum)
            {
            case 0:
                m_pGameInstance->Play_Sound(L"Joker_HitByPlayer1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Joker_HitByPlayer2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
                break;
            }
#pragma endregion
        }
        else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower() / 4.f);
            m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 50.f;
            if (m_fMonsterCurHP <= m_fShieldHP)
            {
                m_fMonsterCurHP = m_fShieldHP;
            }

#pragma region Sound

            _uint iRandSoundFileNum = {};
            iRandSoundFileNum = rand() % 2;

            switch (iRandSoundFileNum)
            {
            case 0:
                m_pGameInstance->Play_Sound(L"Joker_HitByPlayer1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Joker_HitByPlayer2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
                break;
            }

#pragma endregion

        }
    }
}

void CElite_Joker::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
   
}

void CElite_Joker::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CElite_Joker* CElite_Joker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElite_Joker* pInstance = new CElite_Joker(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CElite_Joker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CElite_Joker::Clone(void* pArg)
{
    CElite_Joker* pInstance = new CElite_Joker(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CElite_Joker");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CElite_Joker::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Idle_State

void CElite_Joker::Idle_State::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 9;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Idle_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
    pObject->RotateDegree_To_Player();

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Walk_State(), pObject);
}

void CElite_Joker::Idle_State::State_Exit(CElite_Joker* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion

#pragma region Intro_State

void CElite_Joker::Intro_State::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 15;
    pObject->m_bActive = true;
    pObject->m_bPatternProgress = true;
    pObject->m_iMonster_State = STATE_INTRO;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Intro_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Intro"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Open.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Intro_State::State_Exit(CElite_Joker* pObject)
{
}

#pragma endregion

#pragma region Walk_State

void CElite_Joker::Walk_State::State_Enter(CElite_Joker* pObject)
{
    if (pObject->m_fDistance > 1.f)
    {
        m_iIndex = 30;
    }
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 29;
            break;
        case 1:
            m_iIndex = 31;
            break;
        case 2:
            m_iIndex = 32;
            break;
        }
    }
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Joker::Walk_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Foot3"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Foot3.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Foot1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Foot1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    pObject->RotateDegree_To_Player();

    if (m_iIndex == 30 && pObject->m_fDistance > pObject->m_fRootDistance)
        pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
    else if (m_iIndex == 29)
        pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
    else if (m_iIndex == 31)
        pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
    else
        pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
}

void CElite_Joker::Walk_State::State_Exit(CElite_Joker* pObject)
{

}

#pragma endregion

#pragma region Attack_Combo_A

void CElite_Joker::Attack_Combo_A::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 0;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    //rand() % 2
    _uint iRandom = 0;
    if (iRandom == 0)
        m_bBonusAttack = true;
    else
        m_bBonusAttack = false;

}

void CElite_Joker::Attack_Combo_A::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Grr1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Grr_Hit1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Grr2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Grr_Hit2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && m_bBonusAttack && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 85.f)
    {
        m_iIndex++;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Attack_Combo_A::State_Exit(CElite_Joker* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_B

void CElite_Joker::Attack_Combo_B::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 2;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    _uint iRandom = 0;
    if (iRandom == 0)
        m_bBonusAttack = true;
    else
        m_bBonusAttack = false;
}

void CElite_Joker::Attack_Combo_B::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Stab_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Stab_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Grr2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Grr_Hit2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && m_bBonusAttack && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 70.f)
    {
        m_iIndex++;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
    }
}

void CElite_Joker::Attack_Combo_B::State_Exit(CElite_Joker* pObject)
{
}

#pragma endregion

#pragma region Attack_Run

void CElite_Joker::Attack_Run::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 13;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_RUN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Attack_Run::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Stab_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Stab_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Foot1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Foot1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Foot2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Foot2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Stab"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Stab.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Warning"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_ShockWave_Start.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("Bip001-Head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 12)
    {
        m_fTimer += 1.f * fTimeDelta;
        pObject->m_pTransformCom->Go_Dir(pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK), pObject->m_pNavigationCom, fTimeDelta * 3.f);
    }
    if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 12;
        pObject->m_bCan_Move_Anim = true;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if ((m_iIndex == 12 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 1.5f) || m_fTimer >= 3.f)
    {
        m_iIndex = 11;
        pObject->m_bCan_Move_Anim = false;
        pObject->m_iMonster_Attack_Power = 48;
        pObject->m_iMonster_State = STATE_ATTACK;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURXXLF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 70)
    {
        pObject->m_iMonster_Attack_Power = 114;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    }

    if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Attack_Run::State_Exit(CElite_Joker* pObject)
{
}
#pragma endregion

#pragma region Attack_Wheel

void CElite_Joker::Attack_Wheel::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 36;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 48;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Attack_Wheel::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Wheel"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_WheelAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.4f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Wheel1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_WheelAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.4f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Wheel2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_WheelAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.4f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Stab_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Stab_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
        m_fTimer += 1.f * fTimeDelta;

    if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance > pObject->m_fRootDistance)
    {
        pObject->m_bNot_Need_Root = true;
        pObject->RotateDegree_To_Player();
        pObject->m_pTransformCom->Go_Dir(pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK), pObject->m_pNavigationCom, fTimeDelta * 2.f);
    }
    if (m_iIndex == 36 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 35;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && m_fTimer >= 4.f)
    {
        m_iIndex = (rand() % 2) + 33;
        pObject->m_bNot_Need_Root = false;
        pObject->m_iMonster_Attack_Power = 114;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if ((m_iIndex == 33 || m_iIndex == 34) && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
    }
}

void CElite_Joker::Attack_Wheel::State_Exit(CElite_Joker* pObject)
{
}

#pragma endregion

#pragma region Stun_State

void CElite_Joker::Stun_State::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 19;
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_JOKER;
}

void CElite_Joker::Stun_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
    _bool bMonster_Event = static_cast<CPlayer*>(pObject->m_pPlayer)->Get_MonsterEvent();   

    const _uint iCurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 18 && iCurrentAnimIndex == m_iIndex)
    {
        m_fTimer += fTimeDelta;

        if (m_fTimer >= 5.f)
        {
            m_iIndex = 17;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE::STATE_Joker_Execution && bMonster_Event)
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 19 && iCurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE::STATE_Joker_Execution && bMonster_Event)       
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 18;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 17 && iCurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
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

void CElite_Joker::Stun_State::State_Exit(CElite_Joker* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Execution_State

void CElite_Joker::Execution_State::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 22;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bCan_Move_Anim = true;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_EXECUTION;

    _float fTeleportDistance = 1.f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);
    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, fTeleportDistance));

    pObject->m_pTransformCom->LookAt(vPlayerPos);
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->m_pTransformCom->LookAt(vPlayerPos);


    /* 선환 추가 */
    pObject->m_pModelCom->Get_VecAnimation().at(22)->SetLerpTime(0.f);  
    pObject->m_pModelCom->Set_LerpFinished(true);   
    /* =========  */

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


}

void CElite_Joker::Execution_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

#pragma region Effect_Execution

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            if (!strcmp(iter.szName, "Effect_Execution")) 
            {
                const _float4x4* matNeck = pObject->m_pModelCom->Get_BoneMatrix("Bip001-Neck");
                pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_JOKER_EXECUTION, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matNeck);
                iter.isPlay = true;
            }
        }
#pragma region Sound

        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Execution"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Execution.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }

#pragma endregion

    }

#pragma endregion

    if (m_iIndex == 22 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish() && pObject->m_iMonster_State != STATE_DEAD)
    {
        m_iIndex = 21;
        pObject->m_iMonster_State = STATE_DEAD;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);

#pragma region UI상호작용
        // 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
        dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1544);
        pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1544);
        // 몬스터 사망 시 아이템 드랍 추가하기
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_4, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion
    }
}

void CElite_Joker::Execution_State::State_Exit(CElite_Joker* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Attack_Shock
void CElite_Joker::Attack_Shock::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 14;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 128;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Attack_Shock::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_ShockWave_Start"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_ShockWave_Start.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("Bip001-Head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_ShockWave_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_ShockWave_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    //특정 키프레임까지만 쳐다보게하기
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 70.f)
        pObject->RotateDegree_To_Player();

    if (m_iIndex == 14 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Attack_Shock::State_Exit(CElite_Joker* pObject)
{
}
#pragma endregion

#pragma region Attack_Strong
void CElite_Joker::Attack_Strong::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 16;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 114;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Attack_Strong::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Vocal"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Grr_Hit1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 16 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Attack_Strong::State_Exit(CElite_Joker* pObject)
{
}
#pragma endregion

#pragma region Attack_Jump
void CElite_Joker::Attack_Jump::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 10;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 114;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Joker::Attack_Jump::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Vocal"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Grr_Hit1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Smash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Joker_Smash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CElite_Joker::Idle_State(), pObject);
}

void CElite_Joker::Attack_Jump::State_Exit(CElite_Joker* pObject)
{
}

#pragma endregion

void CElite_Joker::Return_To_SpawnPoint_State::State_Enter(CElite_Joker* pObject)
{
    m_iIndex = 30;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Joker::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CElite_Joker* pObject)
{
    pObject->m_pNavigationCom->Start_Astar(pObject->m_iSpawn_Cell_Index);
    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_bCheck), 0.f);
    if (m_bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta, pObject->m_pNavigationCom);
    }
    if (pObject->m_fDistance <= 1.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }

    if (pObject->m_fSpawn_Distance <= 3.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }
}

void CElite_Joker::Return_To_SpawnPoint_State::State_Exit(CElite_Joker* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}
