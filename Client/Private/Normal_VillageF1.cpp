#include "pch.h"
#include "Normal_VillageF1.h"
#include "GameInstance.h"
#include "Body_VillageF1.h"
#include "Weapon_Dagger.h"
#include "Animation.h"
#include "Monster_HP_Bar.h"
#include "Decorative_Mutation.h"
#include "Player.h"
#include "Locked_On.h"

CNormal_VillageF1::CNormal_VillageF1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CNormal_VillageF1::CNormal_VillageF1(const CNormal_VillageF1& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CNormal_VillageF1::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormal_VillageF1::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 10.f;
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

    m_pState_Manager = CState_Machine<CNormal_VillageF1>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 0.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,22.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_JAVELINSWORD;

    return S_OK;
}

void CNormal_VillageF1::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CNormal_VillageF1::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

}

void CNormal_VillageF1::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CNormal_VillageF1::Render()
{
    return S_OK;
}

HRESULT CNormal_VillageF1::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CNormal_VillageF1::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CBody_VillageF1::BODY_VillageF1_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_VillageF1"), LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1_Body"), &BodyDesc)))
        return E_FAIL;

    CWeapon_Dagger::WEAPON_DAGGER_DESC		Weapon_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_VillageF1"), TEXT("Com_Model")));
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

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Dagger"), LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Dagger"), &Weapon_Desc)))
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

    CDecorative_Mutation::DECORATIVE_MUTATION_DESC Deco_Desc{};
    Deco_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("clavicle_l");
    Deco_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Deco_Desc.pParentState = &m_iMonster_State;
    Deco_Desc.fSpeedPerSec = 0.f;
    Deco_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Decorative_Mutation"), LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Mutation"), &Deco_Desc)))
        return E_FAIL;

    return S_OK;
}
void CNormal_VillageF1::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}
void CNormal_VillageF1::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 2.f && m_fDistance <= 1.5f)
        {
            _uint iRandom = rand() % 3;
            switch (iRandom)
            {
            case 0:
                m_pState_Manager->ChangeState(new Attack_01, this);
                break;
            case 1:
                m_pState_Manager->ChangeState(new Attack_02, this);
                break;
            case 2:
                m_pState_Manager->ChangeState(new Attack_03, this);
                break;
            }

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CNormal_VillageF1::Active()
{
    m_bActive = true;
    if (!m_bFirstActive)
    {
        m_bFirstActive = true;
        m_pState_Manager->ChangeState(new CNormal_VillageF1::Idle_State(), this);
    }
}

void CNormal_VillageF1::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CNormal_VillageF1::Return_To_SpawnPoint_State(), this);
}

void CNormal_VillageF1::Stun()
{
    m_pState_Manager->ChangeState(new CNormal_VillageF1::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // ���⼭ �������鼭 ������ ����Ʈ         
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CNormal_VillageF1::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    //���߿� �ѹ��� �����ϱ�.

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
            uniform_int_distribution<> ParryCount_Random(3, 5);
            uniform_int_distribution<> Random_Pattern(0, 1);

            m_iParryReadyHits = ParryCount_Random(gen);
            _uint iRandom = Random_Pattern(gen);

            if (iRandom == 0)
            {
                m_pState_Manager->ChangeState(new CNormal_VillageF1::Parry_State(), this);
            }
            else
            {
                _uint iRandom = rand() % 2;
                switch (iRandom)
                {
                case 0:
                    m_pState_Manager->ChangeState(new Attack_02(), this);
                    break;
                case 1:
                    m_pState_Manager->ChangeState(new Attack_03(), this);
                    break;
                }
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
            m_fShieldHP -= (*m_Player_Attack / 7.5f);
        }
        else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower()) / 4.f;
            m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 50.f;
            if (m_fMonsterCurHP <= m_fShieldHP)
            {
                m_fMonsterCurHP = m_fShieldHP;
            }
        }

        if (m_bCanHit &&
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
            m_pState_Manager->ChangeState(new CNormal_VillageF1::Hit_State(m_iHit_Motion_Index), this);
        }

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

void CNormal_VillageF1::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CNormal_VillageF1::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CNormal_VillageF1* CNormal_VillageF1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormal_VillageF1* pInstance = new CNormal_VillageF1(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CNormal_VillageF1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormal_VillageF1::Clone(void* pArg)
{
    CNormal_VillageF1* pInstance = new CNormal_VillageF1(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CNormal_VillageF1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormal_VillageF1::Free()
{
    __super::Free();


    Safe_Release(m_pState_Manager);
}

#pragma region Idle_State

void CNormal_VillageF1::Idle_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 33;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_bCanHit = true;
    pObject->m_bPatternProgress = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Idle_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance >= 10.f)
    {
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    }
    else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance < 10.f)
    {
        pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
    }
}

void CNormal_VillageF1::Idle_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion

#pragma region Move_State

void CNormal_VillageF1::Move_State::State_Enter(CNormal_VillageF1* pObject)
{
    if (pObject->m_fDistance > 1.f)
        m_iIndex = 46;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 47;
            break;
        case 1:
            m_iIndex = 48;
            break;
        case 2:
            m_iIndex = 45;
            break;
        }
    }
    pObject->m_bCanHit = true;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = false;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageF1::Move_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (pObject->m_fDistance >= 3.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else if (pObject->m_fDistance < 3.f)
    {
        pObject->RotateDegree_To_Player();
        if (m_iIndex == 46 && pObject->m_fDistance > pObject->m_fRootDistance)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 45)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 47)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 48)
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CNormal_VillageF1::Move_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Run_State

void CNormal_VillageF1::Run_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 35;
    pObject->m_iMonster_State = STATE_RUN;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Run_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (pObject->m_fDistance >= 10.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
    {
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    }

    if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 36;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
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
        pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        pObject->m_bPatternProgress = true;
        if (pObject->m_fDistance <= 1.5f)
        {
            pObject->m_fDelayTime = 0.f;
            pObject->m_bPatternProgress = true;
            pObject->m_pState_Manager->ChangeState(new Run_Attack(), pObject);
        }
    }
}

void CNormal_VillageF1::Run_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Run_Attack

void CNormal_VillageF1::Run_Attack::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 6;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_Attack_Power = 66;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(3.f);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CNormal_VillageF1::Run_Attack::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }


#pragma region SOUND_RUN_ATTACK
    if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_Heavy_Whoosh_1")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("WhooshHeavySword_05.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Run_Attack::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_01

void CNormal_VillageF1::Attack_01::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 2;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 60;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Attack_01::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 27.f)
        {
            m_iIndex = 1;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSL;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(0.f);
        }
    }
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 29.f && !m_Is_Done_Bonus_Attack)
        {
            m_iIndex = 2;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
            m_Is_Done_Bonus_Attack = true;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(15.f);
        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
        }
    }


#pragma region SOUND_ATTACK_01_1
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_Knife_Whoosh_1")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("Villager_WhooshKnife_01.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Attack_06.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion

#pragma region SOUND_ATTACK_01_2
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_Knife_Whoosh_2")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("Villager_WhooshKnife_04.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Attack_03.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Attack_01::State_Exit(CNormal_VillageF1* pObject)
{
}
#pragma endregion

#pragma region Attack_02

void CNormal_VillageF1::Attack_02::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 3;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_Attack_Power = 60;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Attack_02::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 21.f)
    {
        m_iIndex = 4;
        pObject->m_iMonster_Attack_Power = 72;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);


#pragma region SOUND_ATTACK_02_1
    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_Knife_Whoosh_3")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("Villager_WhooshKnife_02.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Attack_01.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion

#pragma region SOUND_ATTACK_02_2
    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_Knife_Whoosh_4")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("Villager_WhooshKnife_05.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Attack_04.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Attack_02::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_03

void CNormal_VillageF1::Attack_03::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 5;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 81;
    pObject->m_bCanHit = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Attack_03::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);


#pragma region SOUND_ATTACK_03_1
    if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Whoosh_5")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_ACTION);
                        pObject->m_pGameInstance->Play_Sound(TEXT("Villager_WhooshKnife_03.ogg"), CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Scream_03.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.1f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Attack_03::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bCanHit = true;
}

#pragma endregion

#pragma region Hit_State

CNormal_VillageF1::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}

void CNormal_VillageF1::Hit_State::State_Enter(CNormal_VillageF1* pObject)
{
    switch (m_iHitNum)
    {
    case 0:
        m_iIndex = 26;
        break;
    case 1:
        m_iIndex = 27;
        break;
    }

    pObject->m_iHitCount++;
    pObject->m_iMonster_State = STATE_HIT;
    pObject->RotateDegree_To_Player();
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageF1::Hit_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);


#pragma region SOUND_HIT_1
    if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Hurt_1")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Hurt_01.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.1f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion


#pragma region SOUND_HIT_2
    if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Hurt_2")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Hurt_02.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.1f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }

#pragma endregion

}

void CNormal_VillageF1::Hit_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bPatternProgress = false;
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Stun_State
void CNormal_VillageF1::Stun_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 31;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bCanHit = false;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);
}

void CNormal_VillageF1::Stun_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    _bool bMonster_Event = static_cast<CPlayer*>(pObject->m_pPlayer)->Get_MonsterEvent();

    const _uint iCurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 30 && iCurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 28;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        else if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_LV1Villager_M_Execution && bMonster_Event) 
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 31 && iCurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_LV1Villager_M_Execution && bMonster_Event)  
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 30;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 28 && iCurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_fShieldHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_IsStun = false;
        pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;

        pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
        pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);

        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }

#pragma region SOUND_STUN
    if (m_iIndex == 31 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Hurt_3")) //1������� �ִϸ��̼��� ���ҵǱ⿡ ���⿡ �ϳ��� �߰�
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Hurt_01.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion

}

void CNormal_VillageF1::Stun_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Execution_State

void CNormal_VillageF1::Execution_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 0;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = STATE_EXECUTION;

    _float teleportDistance = 1.f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);

    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

    pObject->m_pTransformCom->LookAt(vPlayerPos);
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->m_pTransformCom->LookAt(vPlayerPos);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);



    /* ��ȯ �߰� */
    pObject->m_pModelCom->Get_VecAnimation().at(0)->SetLerpTime(0.f);
    pObject->m_pModelCom->Set_LerpFinished(true);
    /* =========  */

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CNormal_VillageF1::Execution_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
#pragma region EFFECT_EXECUTION
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�    
            {
                if (!strcmp(iter.szName, "Effect_Execution_1"))
                {
                    const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_NORMAL_EXECUTION_STAB, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);

                    pObject->m_pGameInstance->Play_Sound(TEXT("Normal_Execution_1.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 1.f);
                    iter.isPlay = true;      // �� ���� ��� �Ǿ�� �ϹǷ�         
                }
                else if (!strcmp(iter.szName, "Effect_Execution_2"))
                {
                    const _float4x4* matWeapon_r = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_NORMAL_EXECUTION_KICK, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matWeapon_r);

                    pObject->m_pGameInstance->Play_Sound(TEXT("Normal_Execution_2.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 1.f);
                    iter.isPlay = true;      // �� ���� ��� �Ǿ�� �ϹǷ�         
                }
            }
        }
    }
#pragma endregion

    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f)
    {
        pObject->m_pState_Manager->ChangeState(new CNormal_VillageF1::Dead_State(), pObject);
    }
}

void CNormal_VillageF1::Execution_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

void CNormal_VillageF1::Return_To_SpawnPoint_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 46;
    pObject->m_fDelayTime = 0.f;
    pObject->m_bPatternProgress = true;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageF1::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
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

void CNormal_VillageF1::Return_To_SpawnPoint_State::State_Exit(CNormal_VillageF1* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}

void CNormal_VillageF1::Parry_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 34;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_Attack_Power = 0;
    pObject->m_iMonster_State = STATE_PARRY;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

#pragma region Effect
    pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CNormal_VillageF1::Parry_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (m_iIndex == 34 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new Attack_01, pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new Attack_02, pObject);
            break;
        case 2:
            pObject->m_pState_Manager->ChangeState(new Attack_03, pObject);
            break;
        }
    }


#pragma region SOUND_PARRY_1
    if (m_iIndex == 34 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Parry_1"))
                    {
                        pObject->m_pGameInstance->Play_Sound(TEXT("Player_Parry_Deflect_L_Real.wav"), CHANNELID::SOUND_MONSTER_WEAPON, 20.f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Parry_State::State_Exit(CNormal_VillageF1* pObject)
{

}

void CNormal_VillageF1::Dead_State::State_Enter(CNormal_VillageF1* pObject)
{
    m_iIndex = 19;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


#pragma region UI��ȣ�ۿ�
    // ������� �ʰ� �÷��̾�� ����Ǵ� ����� ���� �߰�
    dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1296);
    pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1296);
    // ���� ��� �� ������ ��� �߰��ϱ�
    pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
    pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);

#pragma endregion
}

void CNormal_VillageF1::Dead_State::State_Update(_float fTimeDelta, CNormal_VillageF1* pObject)
{
    if (m_iIndex == 19 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_iMonster_State = STATE_DEAD;
    }


#pragma region SOUND_DEAD_1
    if (m_iIndex == 19 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
                {

                    if (!strcmp(iter.szName, "Event_VillagerF_Die_1"))
                    {
                        pObject->m_pGameInstance->StopSound(CHANNELID::SOUND_MONSTER_VOICE);
                        pObject->m_pGameInstance->Play_Sound(TEXT("VillagerF_Voice_Die_01.ogg"), CHANNELID::SOUND_MONSTER_VOICE, 0.15f);
                        iter.isPlay = true;
                    }
                }
            }
        }
    }
#pragma endregion
}

void CNormal_VillageF1::Dead_State::State_Exit(CNormal_VillageF1* pObject)
{
}
