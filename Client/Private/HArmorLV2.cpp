#include "pch.h"
#include "HArmorLv2.h"
#include "GameInstance.h"
#include "Body_HArmorLv2.h"
#include "Weapon_GreatSword.h"
#include "Monster_HP_Bar.h"
#include "Player.h"
#include "Animation.h"
#include "Locked_On.h"

CHArmorLV2::CHArmorLV2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CHArmorLV2::CHArmorLV2(const CHArmorLV2& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CHArmorLV2::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHArmorLV2::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 15.f;
    m_fActive_Distance = 5.f;
    m_iParryReadyHits = 3;

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

    m_pState_Manager = CState_Machine<CHArmorLV2>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);


    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,25.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_GREADSWORD;

    m_iMonster_Attack_Power = 148;

    return S_OK;
}

void CHArmorLV2::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);

}

void CHArmorLV2::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CHArmorLV2::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CHArmorLV2::Render()
{
    return S_OK;
}

HRESULT CHArmorLV2::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CHArmorLV2::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);


    CBody_HArmorLV2::BODY_HArmorLV2_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_HArmorLV2"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2_Body"), &BodyDesc)))
        return E_FAIL;

    CWeapon_GreatSword::WEAPON_GreatSword_DESC		Weapon_GreatSword_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_HArmorLV2"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;
    Weapon_GreatSword_Desc.pParent = this;
    Weapon_GreatSword_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Weapon_GreatSword_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_GreatSword_Desc.pParentModel = m_pModelCom;
    Weapon_GreatSword_Desc.pParentState = &m_iMonster_State;
    Weapon_GreatSword_Desc.iAttack = &m_iMonster_Attack_Power;
    Weapon_GreatSword_Desc.fSpeedPerSec = 0.f;
    Weapon_GreatSword_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_GreatSword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_GreatSword"), &Weapon_GreatSword_Desc)))
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

void CHArmorLV2::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}

void CHArmorLV2::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 1.f && m_fDistance <= 5.f)
        {
            if (m_fDistance >= 3.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();
            m_bCanHit = false;
            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CHArmorLV2::Active()
{
    if (!m_bFirstActive)
    {
        m_pState_Manager->ChangeState(new CHArmorLV2::Intro_State(), this);
    }
    else
        m_pState_Manager->ChangeState(new CHArmorLV2::NotActive_Idle(), this);
}

void CHArmorLV2::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CHArmorLV2::Return_To_SpawnPoint_State(), this);
}

void CHArmorLV2::Stun()
{
    m_pState_Manager->ChangeState(new CHArmorLV2::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
    m_pGameInstance->Set_SlowWorld(true);   
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CHArmorLV2::Near_Pattern_Create()
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
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_01, this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_02, this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_03, this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_06, this);
        break;
    case 4:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_07, this);
        break;
    default:
        break;
    }
}

void CHArmorLV2::Far_Pattern_Create()
{
    m_pState_Manager->ChangeState(new Attack_Pattern_05, this);
}

_bool CHArmorLV2::Is_Need_Turn_Attack()
{
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vLook2 = XMLoadFloat4(&m_vPlayerPos) - vPos;

    vLook = XMVector3Normalize(vLook);
    vLook2 = XMVector3Normalize(vLook2);

    //회전해야 하는 각도
    _float fAngle = acos(XMVectorGetX(XMVector3Dot(vLook, vLook2)));
    fAngle = XMConvertToDegrees(fAngle);

    if (fAngle >= 150.f)
    {
        return true;
    }
    return false;
}

void CHArmorLV2::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) || !strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
    {
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        m_fDelayTime -= m_fTimeDelta;
        m_fRecoveryTime = 0.f;
        m_bCanRecovery = false;

        if (m_iHitCount >= m_iParryReadyHits)
        {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> ParryCount_Random(2, 4);
            uniform_int_distribution<> Random_Pattern(0, 1);
            m_iParryReadyHits = ParryCount_Random(gen);
            _uint iRandom = Random_Pattern(gen);

            if (iRandom == 0)
            {
                m_pState_Manager->ChangeState(new CHArmorLV2::Parry_State(), this);
            }
            else
            {
                m_bCanHit = false;
                Near_Pattern_Create();
            }

            m_iHitCount = 0;
            m_bCanHit = false;
            m_bPatternProgress = true;
            m_fDelayTime = 0.f;
            return;
        }

        if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= *m_Player_Attack / 50.f;
            m_fShieldHP -= (*m_Player_Attack / 10.f);
        }
        else if(!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 4.f;
            m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 50.f;
            if (m_fMonsterCurHP <= m_fShieldHP)
            {
                m_fMonsterCurHP = m_fShieldHP;
            }
        }

        if (m_bCanHit &&
            m_iMonster_State != STATE_ATTACK &&
            m_iMonster_State != STATE_SPECIAL_ATTACK &&
            m_fMonsterCurHP > 0.f &&
            m_iHitCount < m_iParryReadyHits)
        {
            _uint iRandom = rand() % 2;
            while (true)
            {
                if (iRandom == m_iHit_Motion_Index)
                    iRandom = rand() % 2;
                else
                {
                    m_iHit_Motion_Index = iRandom;
                    break;
                }
            }
            m_pState_Manager->ChangeState(new CHArmorLV2::Hit_State(m_iHit_Motion_Index), this);
        }

#pragma region Sound
        _uint iRandSoundFileNum = {};
        iRandSoundFileNum = rand() % 4;

        switch (iRandSoundFileNum)
        {
        case 0:
            m_pGameInstance->Play_Sound(L"Armor_HitByPlayer1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
            break;
        case 1:
            m_pGameInstance->Play_Sound(L"Armor_HitByPlayer2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
            break;
        case 2:
            m_pGameInstance->Play_Sound(L"Armor_HitByPlayer3.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
            break;
        case 3:
            m_pGameInstance->Play_Sound(L"Armor_HitByPlayer4.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.5f);
            break;
        }
#pragma endregion
    }
}

void CHArmorLV2::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
  
}

void CHArmorLV2::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CHArmorLV2* CHArmorLV2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHArmorLV2* pInstance = new CHArmorLV2(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CHArmorLv2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHArmorLV2::Clone(void* pArg)
{
    CHArmorLV2* pInstance = new CHArmorLV2(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CHArmorLv2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHArmorLV2::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Intro_State

void CHArmorLV2::Intro_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 34;

    pObject->m_bActive = true;
    pObject->m_bFirstActive = true;
    pObject->m_bPatternProgress = true;

    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Intro_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Intro"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Intro.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
        vLook *= -1;
        pObject->m_pTransformCom->Look(vLook);
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
    }
}

void CHArmorLV2::Intro_State::State_Exit(CHArmorLV2* pObject)
{
}

#pragma endregion

#pragma region Idle_State

void CHArmorLV2::Idle_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 26;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_bCanHit = true;
    pObject->m_bPatternProgress = false;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Idle_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
}

void CHArmorLV2::Idle_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion


#pragma region Move_State


void CHArmorLV2::Move_State::State_Enter(CHArmorLV2* pObject)
{
    if (pObject->m_fDistance > 1.f)
        m_iIndex = 47;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 44;
            break;
        case 1:
            m_iIndex = 51;
            break;
        case 2:
            m_iIndex = 52;
            break;
        }
    }
    pObject->m_bCanHit = true;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::Move_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Walk1"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 2;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                }
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Walk2"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 2;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                }
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_fDistance >= 3.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else if (pObject->m_fDistance < 3.f)
    {
        pObject->RotateDegree_To_Player();
        if (m_iIndex == 47 && pObject->m_fDistance > pObject->m_fRootDistance)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 44)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 51)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 52)
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CHArmorLV2::Move_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion


#pragma region Run_State

void CHArmorLV2::Run_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 33;
    pObject->m_bCanHit = true;
    pObject->m_iMonster_State = STATE_RUN;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::Run_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Run1"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 2;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                }
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Run2"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 2;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_Ready2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.05f);
                    break;
                }
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_fDistance >= 5.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
    {
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    }

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

void CHArmorLV2::Run_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion


#pragma region Stun_State
void CHArmorLV2::Stun_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 24;
    pObject->m_iMonster_State = STATE_STUN;

    pObject->m_bCan_Move_Anim = true;
    pObject->m_bCanHit = false;

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_HARMOR;
}

void CHArmorLV2::Stun_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    _bool bMonster_Event = static_cast<CPlayer*>(pObject->m_pPlayer)->Get_MonsterEvent();

    const _uint iCurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 23 && iCurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 22;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        else if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_HARMOR_EXECUTION && bMonster_Event)    
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 24 && iCurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_HARMOR_EXECUTION && bMonster_Event) 
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 23;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 22 && iCurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
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

void CHArmorLV2::Stun_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion


#pragma region Attack_Pattern_01 (회전베기 후 앞 대쉬 이후 찍기 패턴)
void CHArmorLV2::Attack_Pattern_01::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 4;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_01::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_HorizonAttack1"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 2:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack3.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                }
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Dash"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Dash.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_VerticalAttack"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_VerticalAttack.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 61.f)
    {
        m_iIndex = 8;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(20.f);
    }

    if (m_iIndex == 8 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 85.f)
    {
        m_iIndex = 7;
        pObject->RotateDegree_To_Player();
        pObject->m_iMonster_Attack_Power = 88;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
        pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
    }

    if (m_iIndex == 7 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Attack_Pattern_01::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_02 (회전베기 (오른쪽으로끝남) 이후 날리는 콤보 패턴)
void CHArmorLV2::Attack_Pattern_02::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 5;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_02::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_HorizonAttack2"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 2:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack3.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                }
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_WarningAttack1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Warning_VerticalAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 57.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iMonster_Attack_Power = 88;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Attack_Pattern_02::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_03 (한번 찌르고 뒤로 백스탭 패턴)
void CHArmorLV2::Attack_Pattern_03::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 3;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_03::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Stab"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Stab.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Step"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Step.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 36;
        pObject->RotateDegree_To_Player();
        pObject->m_bCan_Move_Anim = true;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 36 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Attack_Pattern_03::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion


#pragma region Attack_Pattern_04 (Turn 공격)
void CHArmorLV2::Attack_Pattern_04::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 39;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    pObject->m_iMonster_Attack_Power = 88;
    XMStoreFloat4(&m_vOriginalLook, pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK));
    XMStoreFloat4(&m_vChangeLook, XMLoadFloat4(&m_vOriginalLook) * -1);

}

void CHArmorLV2::Attack_Pattern_04::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Intro"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Intro.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pTransformCom->Look(XMLoadFloat4(&m_vChangeLook));
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
    }
}

void CHArmorLV2::Attack_Pattern_04::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_05 (멀리있을때 점프뛰고 날리는 콤보 패턴)
void CHArmorLV2::Attack_Pattern_05::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 32;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 88;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_05::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_WarningAttack1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Warning_VerticalAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_JumpAttack"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_JumpAttack.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 32 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 70.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }

}

void CHArmorLV2::Attack_Pattern_05::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_06(대각선으로 베고 난뒤 회전베기)
void CHArmorLV2::Attack_Pattern_06::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 1;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_06::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_HorizonAttack1"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 2:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack3.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                }
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_SmallAttack"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_SmallAttack.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 49.f)
    {
        m_iIndex = 4;
        pObject->RotateDegree_To_Player();
        pObject->m_iMonster_Attack_Power = 70;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Attack_Pattern_06::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_07(대각선 베기 한 뒤 날리기 패턴)
void CHArmorLV2::Attack_Pattern_07::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 1;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_07::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_WarningAttack1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Warning_VerticalAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_SmallAttack"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_SmallAttack.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iMonster_Attack_Power = 88;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        if (pObject->Is_Need_Turn_Attack())
        {
            pObject->m_pState_Manager->ChangeState(new Attack_Pattern_04(), pObject);
            return;
        }
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Attack_Pattern_07::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion

#pragma region Execution_State
void CHArmorLV2::Execution_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 53;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bCanHit = false;

    _float teleportDistance = 0.9f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);

    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

    pObject->m_pTransformCom->LookAt(vPlayerPos);
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->m_pTransformCom->LookAt(vPlayerPos);

    /* 선환 추가 */
    pObject->m_pModelCom->Get_VecAnimation().at(53)->SetLerpTime(0.f);
    pObject->m_pModelCom->Set_LerpFinished(true);
    /* =========  */

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CHArmorLV2::Execution_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Execution_1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Execution_1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Execution_2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Execution_2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish() && pObject->m_iMonster_State != STATE_DEAD)
    {
        pObject->m_iMonster_State = STATE_DEAD;

#pragma region UI상호작용
        // 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
        dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1544);
        pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1544);
        // 몬스터 사망 시 아이템 드랍 추가하기
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_KEY2, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_6, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion
    }
    //죽음 처리
}

void CHArmorLV2::Execution_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Hit_State

CHArmorLV2::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}
void CHArmorLV2::Hit_State::State_Enter(CHArmorLV2* pObject)
{
    switch (m_iHitNum)
    {
    case 0:
        m_iIndex = 20;
        break;
    case 1:
        m_iIndex = 21;
        break;
    }
    pObject->m_iHitCount++;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_HIT;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Hit_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Hit_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bPatternProgress = false;
}
#pragma endregion

#pragma region Return_To_SpawnPoint_State

void CHArmorLV2::Return_To_SpawnPoint_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 49;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    pObject->m_pNavigationCom->Start_Astar(pObject->m_iSpawn_Cell_Index);
    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta, pObject->m_pNavigationCom);
    }

    if (pObject->m_fDistance <= 1.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }

    if (pObject->m_fSpawn_Distance <= 3.f)
    {
        pObject->m_pState_Manager->ChangeState(new NotActive_Idle(), pObject);
    }
}

void CHArmorLV2::Return_To_SpawnPoint_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}

#pragma endregion

#pragma region NotActive_Idle

void CHArmorLV2::NotActive_Idle::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 27;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::NotActive_Idle::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 5.f)
    {
        m_iIndex = 28;
        pObject->m_bActive = true;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 28 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
    }

}

void CHArmorLV2::NotActive_Idle::State_Exit(CHArmorLV2* pObject)
{
}

#pragma endregion

void CHArmorLV2::Parry_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 29;
    pObject->RotateDegree_To_Player();
    pObject->m_bCanHit = false;
    pObject->m_iMonster_Attack_Power = 0;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_PARRY;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Parry_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Parry"))
            {
                pObject->m_pGameInstance->Play_Sound(L"HArmor_Parry.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
    {
        pObject->m_pState_Manager->ChangeState(new Parry_Attack_State(), pObject);
    }
}

void CHArmorLV2::Parry_State::State_Exit(CHArmorLV2* pObject)
{
}

void CHArmorLV2::Parry_Attack_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 6;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_PARRY_ATTACK;
    pObject->m_iMonster_Attack_Power = 70;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Parry_Attack_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_HorizonAttack3"))
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3;

                switch (iRandSoundFileNum)
                {
                case 0:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 1:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                case 2:
                    pObject->m_pGameInstance->Play_Sound(L"HArmor_HorizonAttack3.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    break;
                }
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Move_State(), pObject);
            break;
        }
    }
}

void CHArmorLV2::Parry_Attack_State::State_Exit(CHArmorLV2* pObject)
{
}
