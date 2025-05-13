#include "pch.h"
#include "Elite_Punch_Man.h"
#include "GameInstance.h"
#include "Body_Punch_Man.h"
#include "Animation.h"
#include "Player.h"
#include "Locked_On.h"
#include "Monster_HP_Bar.h"

CElite_Punch_Man::CElite_Punch_Man(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CElite_Punch_Man::CElite_Punch_Man(const CElite_Punch_Man& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CElite_Punch_Man::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElite_Punch_Man::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
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

    m_pState_Manager = CState_Machine<CElite_Punch_Man>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 0.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);


    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,25.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_pTransformCom->Scaling(_float3(0.003f, 0.003f, 0.003f));

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_HALBERD;

    return S_OK;
}

void CElite_Punch_Man::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CElite_Punch_Man::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CElite_Punch_Man::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CElite_Punch_Man::Render()
{
    return S_OK;
}

void CElite_Punch_Man::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}


void CElite_Punch_Man::Active()
{
    m_bActive = true;
    if (!m_bFirstActive)
    {
        m_bFirstActive = true;
        m_pState_Manager->ChangeState(new CElite_Punch_Man::Intro_State(), this);
    }
    else
        m_pState_Manager->ChangeState(new CElite_Punch_Man::Idle_State(), this);

}

void CElite_Punch_Man::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CElite_Punch_Man::Return_To_SpawnPoint_State(), this);
}

void CElite_Punch_Man::Stun()
{
    m_pState_Manager->ChangeState(new CElite_Punch_Man::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

HRESULT CElite_Punch_Man::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CElite_Punch_Man::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CBody_Punch_Man::BODY_PUNCH_MAN_DESC BodyDesc{};
    BodyDesc.pParent = this;
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.iAttack = &m_iMonster_Attack_Power;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Punch_Man"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man_Body"), &BodyDesc)))
        return E_FAIL;

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Punch_Man"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
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
void CElite_Punch_Man::PatternCreate()
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

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CElite_Punch_Man::Near_Pattern_Create()
{
    _uint iRandomPattern = rand() % 2;
    while (true)
    {
        if (iRandomPattern == m_iNearPatternIndex)
        {
            iRandomPattern = rand() % 2;
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
        m_pState_Manager->ChangeState(new CElite_Punch_Man::Attack_ComboA(), this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Punch_Man::Attack_ComboC(), this);
        break;

    }
}

void CElite_Punch_Man::Far_Pattern_Create()
{
    m_pState_Manager->ChangeState(new CElite_Punch_Man::Attack_ComboB(), this);
}

void CElite_Punch_Man::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
            m_pGameInstance->Play_Sound(L"DamageFromPlayer_PunchMan.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.3f);
        }
        else if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
        {
            m_fMonsterCurHP -= (*_pOther->Get_Skill_AttackPower() / 4.f);
            m_fShieldHP -= *_pOther->Get_Skill_AttackPower() / 50.f;
            if (m_fMonsterCurHP <= m_fShieldHP)
            {
                m_fMonsterCurHP = m_fShieldHP;
            }
            m_pGameInstance->Play_Sound(L"DamageFromPlayer_PunchMan_2.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.3f);
        }
    }
}

void CElite_Punch_Man::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CElite_Punch_Man::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CElite_Punch_Man* CElite_Punch_Man::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElite_Punch_Man* pInstance = new CElite_Punch_Man(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CElite_Punch_Man");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CElite_Punch_Man::Clone(void* pArg)
{
    CElite_Punch_Man* pInstance = new CElite_Punch_Man(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CElite_Punch_Man");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CElite_Punch_Man::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

void CElite_Punch_Man::Intro_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 10;
    pObject->m_bActive = true;
    pObject->m_bFirstActive = true;
    pObject->m_bPatternProgress = true;

    pObject->m_iMonster_State = STATE_INTRO;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Punch_Man::Intro_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Idle_State(), pObject);
    }
}

void CElite_Punch_Man::Intro_State::State_Exit(CElite_Punch_Man* pObject)
{
}

void CElite_Punch_Man::Idle_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 8;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Punch_Man::Idle_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
    pObject->RotateDegree_To_Player();
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
        pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Move_State(), pObject);
}

void CElite_Punch_Man::Idle_State::State_Exit(CElite_Punch_Man* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

void CElite_Punch_Man::Move_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 17;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Punch_Man::Move_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
    pObject->RotateDegree_To_Player();
    if (m_iIndex == 17 && pObject->m_fDistance > pObject->m_fRootDistance)
        pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
}

void CElite_Punch_Man::Move_State::State_Exit(CElite_Punch_Man* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

void CElite_Punch_Man::Stun_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 13;
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_bCan_Move_Anim = true;

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_PUNCH_MAN;

}

void CElite_Punch_Man::Stun_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
    const _uint iCurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 12 && iCurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 11;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        else if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_PUNCH_MAN_Execution)   
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject); 
            return; 
        }
    }
    else if (m_iIndex == 13 && iCurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_PUNCH_MAN_Execution)
        {
            pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 12;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 11 && iCurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
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

void CElite_Punch_Man::Stun_State::State_Exit(CElite_Punch_Man* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

void CElite_Punch_Man::Execution_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 16;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bExecution_Start = false;

    _float teleportDistance = 1.5f;
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

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Punch_Man::Execution_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Execution"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Execution.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish() && pObject->m_iMonster_State != STATE_DEAD)
    {
        pObject->m_iMonster_State = STATE_DEAD;


#pragma region UI상호작용
        // 드랍하지 않고 플레이어에게 적재되는 기억의 파편 추가
        dynamic_cast<CPlayer*>(pObject->m_pPlayer)->Increase_MemoryFragment(1544);
        pObject->m_pGameInstance->Find_TextBox_PlayerScreen(pObject->m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 101, 1544);
        // 몬스터 사망 시 아이템 드랍 추가하기
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_MEMORY, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_SKILLPIECE, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_1, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion

    }
}

void CElite_Punch_Man::Execution_State::State_Exit(CElite_Punch_Man* pObject)
{
}

void CElite_Punch_Man::Return_To_SpawnPoint_State::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 17;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);

}

void CElite_Punch_Man::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
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
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }
}

void CElite_Punch_Man::Return_To_SpawnPoint_State::State_Exit(CElite_Punch_Man* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}

void CElite_Punch_Man::Attack_ComboA::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 0;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    if (rand() % 2 == 0)
        m_bBonus_Attack = true;
    else
        m_bBonus_Attack = false;

}

void CElite_Punch_Man::Attack_ComboA::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{

#pragma region Sound

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strcmp(iter.szName, "Sound_Swing_1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Swing_1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Swing_2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Swing_2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Swing_ToIdle"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Swing_ToIdle.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_bBonus_Attack)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 135.f)
        {
            m_iIndex = 1;
            pObject->RotateDegree_To_Player();
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
    }

    if ((m_iIndex == 0 || m_iIndex == 1) && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Move_State(), pObject);
            break;
        }
    }

}

void CElite_Punch_Man::Attack_ComboA::State_Exit(CElite_Punch_Man* pObject)
{

}

#pragma region Attack_ComboB (멀리있으면 땅 찍고 달려와서 점프 찍기 공격)
void CElite_Punch_Man::Attack_ComboB::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 4;
    pObject->m_iMonster_Attack_Power = 71;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Punch_Man::Attack_ComboB::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
#pragma region Effect_Combo_B

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            if (!strcmp(iter.szName, "Effect_Strong"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_BIG, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_SPARK_PUNCH_BIG, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_PUNCHMAN_ATTACK, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run1"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run2"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run3"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Punch"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_SMALL, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_PUNCH_DISTORTION, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());

                iter.isPlay = true;
            }
        }
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strncmp(iter.szName, "Sound_Run", strlen("Sound_Run")))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Run.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Small_Punch"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Small_Punch.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Big_Punch_1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Big_Punch1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Big_Punch_2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Big_Punch2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    if (m_iIndex != 2)
    {
        pObject->RotateDegree_To_Player();
    }
    if (m_iIndex == 3)
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 5.f, pObject->m_pNavigationCom);

    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 3;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 2.f)
    {
        m_iIndex = 2;
        pObject->m_iMonster_Attack_Power = 128;
        pObject->m_bCan_Move_Anim = false;
        pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Move_State(), pObject);
            break;
        }
    }

}

void CElite_Punch_Man::Attack_ComboB::State_Exit(CElite_Punch_Man* pObject)
{
}

#pragma endregion 

#pragma region Attack_ComboC (여러번 땅 찍고 점프 찍기 공격)

void CElite_Punch_Man::Attack_ComboC::State_Enter(CElite_Punch_Man* pObject)
{
    m_iIndex = 4;
    pObject->m_iMonster_Attack_Power = 71;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Punch_Man::Attack_ComboC::State_Update(_float fTimeDelta, CElite_Punch_Man* pObject)
{
#pragma region Effect_Combo_B

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            if (!strcmp(iter.szName, "Effect_Strong"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_BIG, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_SPARK_PUNCH_BIG, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_MESH_INSTANCING_PUNCHMAN_ATTACK, (*pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run1"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run2"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Run3"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_RUN, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Effect_Punch"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_DUST_PUNCH_SMALL, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_PUNCH_DISTORTION, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());

                iter.isPlay = true;
            }
        }
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)
        {
            if (!strncmp(iter.szName, "Sound_Run", strlen("Sound_Run")))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Run.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Small_Punch"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Small_Punch.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Big_Punch_1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Big_Punch1.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);

#pragma region Effect_Warning

                pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                _float4x4 matHeadWorld = {};
                XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);

#pragma endregion

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Big_Punch_2"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Punchman_Big_Punch2.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.3f);
                iter.isPlay = true;
            }
        }
    }

#pragma endregion

    pObject->RotateDegree_To_Player();
    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 65.f)
    {
        pObject->m_pModelCom->Set_Continuous_Ani(true);
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(45.f);
        m_iLoopCount += 1;
    }
    if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && m_iLoopCount >= 5)
    {
        m_iIndex = 2;
        pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(0.f);
        pObject->m_iMonster_Attack_Power = 128;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CElite_Punch_Man::Move_State(), pObject);
            break;
        }
    }
}

void CElite_Punch_Man::Attack_ComboC::State_Exit(CElite_Punch_Man* pObject)
{
}
#pragma endregion 