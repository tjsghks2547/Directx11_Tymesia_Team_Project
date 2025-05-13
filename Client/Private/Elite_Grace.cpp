#include "pch.h"
#include "Elite_Grace.h"
#include "GameInstance.h"
#include "Body_Grace.h"
#include "Animation.h"
#include "Player.h"
#include "Locked_On.h"
#include "Projectile_Dagger.h"
#include "Monster_HP_Bar.h"

CElite_Grace::CElite_Grace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CElite_Grace::CElite_Grace(const CElite_Grace& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CElite_Grace::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElite_Grace::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 15.f;
    m_fActive_Distance = 5.f;
    m_fDagger_Delete_Time = 2.f;
    m_iParryReadyHits = 3;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects(pArg)))
        return E_FAIL;


    m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));
    m_iSpawn_Cell_Index = m_pNavigationCom->Get_CurCellIndex();
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
    m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
    m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();

    m_pState_Manager = CState_Machine<CElite_Grace>::Create();
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

    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_JAVELINSWORD;

    return S_OK;
}

void CElite_Grace::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CElite_Grace::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CElite_Grace::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CElite_Grace::Render()
{
    return S_OK;
}

void CElite_Grace::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}


void CElite_Grace::Active()
{
    m_bActive = true;
    m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), this);
}

void CElite_Grace::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CElite_Grace::Return_To_SpawnPoint_State(), this);
}

void CElite_Grace::Stun()
{
    m_pState_Manager->ChangeState(new CElite_Grace::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CElite_Grace::Shoot_Calculate_Distance(_vector& vMyPos, _vector& vPlayerPos)
{
    _vector pPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector pPlayerPos = XMLoadFloat4(&m_vPlayerPos);
    _float fPosY = XMVectorGetY(pPos);
    _float fPlayerPosY = XMVectorGetY(pPlayerPos);
    fPosY += 0.5f;
    fPlayerPosY += 0.5f;
    pPos = XMVectorSetY(pPos, fPosY);
    pPlayerPos = XMVectorSetY(pPlayerPos, fPlayerPosY);

    vMyPos = pPos;
    vPlayerPos = pPlayerPos;
}

HRESULT CElite_Grace::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CElite_Grace::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CProjectile_Dagger::PROJECTILE_DESC ProjectileDesc = {};
    ProjectileDesc.pParent = this;
    ProjectileDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    ProjectileDesc.iCurLevel = iLevel;
    ProjectileDesc.fDelete_Time = &m_fDagger_Delete_Time;
    ProjectileDesc.iDamage = &m_iMonster_Attack_Power;
    ProjectileDesc.fSpeedPerSec = 40.f;
    ProjectileDesc.fRotationPerSec = 0.f;
    ProjectileDesc.fPosition = m_vSpawnPoint;

    for (_uint i = 0; i < 10; i++)
    {
        if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Dagger"), PROJECTILE_DAGGER, &ProjectileDesc)))
            return E_FAIL;
    }

    CBody_Grace::BODY_GRACE_DESC BodyDesc{};
    BodyDesc.pParent = this;
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.iAttack = &m_iMonster_Attack_Power;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Grace"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace_Body"), &BodyDesc)))
        return E_FAIL;

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Grace"), TEXT("Com_Model")));
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

void CElite_Grace::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive && !m_IsStun)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 1.f && m_fDistance <= 5.f)
        {
            if (m_fDistance >= 3.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();

            m_fDelayTime = 0.f;
            m_bCanHit = false;
            m_bPatternProgress = true;
        }
    }

}

void CElite_Grace::Near_Pattern_Create()
{
    _uint iRandomPattern = rand() % 3;
    while (true)
    {
        if (iRandomPattern == m_iNearPatternIndex)
        {
            iRandomPattern = rand() % 3;
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
        m_pState_Manager->ChangeState(new CElite_Grace::Attack_ComboA, this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Grace::Attack_ComboB, this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CElite_Grace::Attack_ComboC, this);
        break;
    default:
        break;
    }
}

void CElite_Grace::Far_Pattern_Create()
{
    _uint iRandomPattern = rand() % 4;
    while (true)
    {
        if (iRandomPattern == m_iFarPatternIndex)
        {
            iRandomPattern = rand() % 4;
        }
        else
        {
            m_iFarPatternIndex = iRandomPattern;
            break;
        }
    }
    switch (m_iFarPatternIndex)
    {
    case 0:
        m_pState_Manager->ChangeState(new CElite_Grace::Shoot_Attack_A, this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Grace::Shoot_Attack_B, this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CElite_Grace::Attack_ComboB, this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CElite_Grace::Attack_ComboC, this);
        break;
    }

}

void CElite_Grace::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
            uniform_int_distribution<> ParryCount_Random(3, 5);
            uniform_int_distribution<> Random_Pattern(0, 1);

            m_iParryReadyHits = ParryCount_Random(gen);
            _uint iRandom = Random_Pattern(gen);

            if (iRandom == 0)
            {
                m_pState_Manager->ChangeState(new CElite_Grace::Parry_State(), this);
            }
            else
            {
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
            m_pState_Manager->ChangeState(new CElite_Grace::Hit_State(m_iHit_Motion_Index), this);
        }

        //Sound
        _uint iHitRandom = rand() % 4;

        switch (iHitRandom)
        {
        case 0:
            m_pGameInstance->Play_Sound(TEXT("Hit1.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;

        case 1:
            m_pGameInstance->Play_Sound(TEXT("Hit2.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;

        case 2:
            m_pGameInstance->Play_Sound(TEXT("Hit3.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;

        case 3:
            m_pGameInstance->Play_Sound(TEXT("Hit4.wav"), CHANNELID::SOUND_MONSTER_DAMAGE, 0.6f);
            break;
        }
    }

}

void CElite_Grace::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CElite_Grace::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CElite_Grace* CElite_Grace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElite_Grace* pInstance = new CElite_Grace(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CElite_Grace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CElite_Grace::Clone(void* pArg)
{
    CElite_Grace* pInstance = new CElite_Grace(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CElite_Grace");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CElite_Grace::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Idle_State

void CElite_Grace::Idle_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 10;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_bCanHit = true;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Idle_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
        pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
}

void CElite_Grace::Idle_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion

#pragma region Move_State

void CElite_Grace::Move_State::State_Enter(CElite_Grace* pObject)
{
    if (pObject->m_fDistance > 1.f)
        m_iIndex = 36;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 35;
            break;
        case 1:
            m_iIndex = 37;
            break;
        case 2:
            m_iIndex = 38;
            break;
        }
    }
    pObject->m_bCanHit = true;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Grace::Move_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (pObject->m_fDistance >= 4.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else if (pObject->m_fDistance < 4.f)
    {
        pObject->RotateDegree_To_Player();
        if (m_iIndex == 36 && pObject->m_fDistance > pObject->m_fRootDistance)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 35)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 37)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 38)
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }

}

void CElite_Grace::Move_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Stun_State

void CElite_Grace::Stun_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 27;
    pObject->m_iMonster_State = STATE_STUN;

    pObject->m_bCan_Move_Anim = true;
    pObject->m_bCanHit = false;

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_GRACE;
}

void CElite_Grace::Stun_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    _bool bMonster_Event = static_cast<CPlayer*>(pObject->m_pPlayer)->Get_MonsterEvent();

    const _uint CurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

    if (m_iIndex == 26 && CurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 25;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        //플레이어 엑스큐전 상태가져오기  
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_GRACE_Execution && bMonster_Event)
        {
            pObject->m_pState_Manager->ChangeState(new CElite_Grace::Execution_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 27 && CurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_GRACE_Execution && bMonster_Event)
        {
            pObject->m_pState_Manager->ChangeState(new CElite_Grace::Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 26;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 25 && CurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
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

void CElite_Grace::Stun_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Execution_State

void CElite_Grace::Execution_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 29;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->m_bCanHit = false;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bExecution_Start = false;

    _float fTeleportDistance = 0.8f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);
    vPlayerRight = XMVector3Normalize(vPlayerRight);

    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, fTeleportDistance));
    vNewPos = XMVectorAdd(vNewPos, XMVectorScale(vPlayerRight, -0.11f));

    pObject->m_pTransformCom->LookAt(vPlayerPos);
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->m_pTransformCom->LookAt(vPlayerPos);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

    /* 선환 추가 */
    pObject->m_pModelCom->Get_VecAnimation().at(29)->SetLerpTime(0.f);
    pObject->m_pModelCom->Set_LerpFinished(true);
    /* =========  */

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Execution_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
#pragma region Effect_Execution

    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            if (!strcmp(iter.szName, "Effect_Blood"))
            {
                pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_GRACE_EXECUTION_BLOOD, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

                //Sound

                pObject->m_pGameInstance->Play_Sound(L"Grace_Execution_1.wav", CHANNELID::SOUND_MONSTER_VOICE, 0.5f);
                iter.isPlay = true;
            }
        }
        if (iter.eType == EVENT_SOUND && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT
        {
            if (!strcmp(iter.szName, "Event_Fall_1"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 0.8f);

                iter.isPlay = true;
            }
            else if (!strcmp(iter.szName, "Sound_Vocal"))
            {
                pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Death_04.ogg", CHANNELID::SOUND_MONSTER_DAMAGE, 0.3f);

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


#pragma region SOUND_EXECUTION
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Fall_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Villager_GotKicked.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);
                    iter.isPlay = true;
                }

            }

        }
    }
#pragma endregion
}

void CElite_Grace::Execution_State::State_Exit(CElite_Grace* pObject)
{
}

#pragma endregion

#pragma region Return_To_SpawnPoint_State

void CElite_Grace::Return_To_SpawnPoint_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 36;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Grace::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
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

void CElite_Grace::Return_To_SpawnPoint_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}

#pragma endregion

void CElite_Grace::Run_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 18;
    pObject->m_iMonster_State = STATE_RUN;
    pObject->m_bPatternProgress = true;
    pObject->m_bNot_Need_Root = true;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Grace::Run_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (pObject->m_fDistance >= 7.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
    {
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    }
    else
    {
        pObject->Near_Pattern_Create();
        return;
    }

    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta * 5.f, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->RotateDegree_To_Player();
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 5.f, pObject->m_pNavigationCom);
    }
}

void CElite_Grace::Run_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_bNot_Need_Root = false;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

void CElite_Grace::Attack_ComboA::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 3;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 52;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CElite_Grace::Attack_ComboA::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 65.f)
        {
            m_iIndex = 2;
            pObject->m_iMonster_Attack_Power = 75;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(31.f);
        }
    }
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 50.f)
        {
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }

    }

#pragma region SOUND_ATTACK_COMBO_A_01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Voice_Ready_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Attack_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_1.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Attack_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_2.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.8f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Attack_Claw_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_3.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.8f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Kick_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.8f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion

}

void CElite_Grace::Attack_ComboA::State_Exit(CElite_Grace* pObject)
{
}

void CElite_Grace::Attack_ComboB::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 11;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 52;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Attack_ComboB::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 85.f)
        {
            m_iIndex = 2;
            pObject->m_iMonster_Attack_Power = 75;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(31.f);
        }
    }
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 50.f)
        {
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }

    }


#pragma region SOUND_ATTACK_COMBO_B_01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (m_iIndex == 11 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Attack_Kick_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Attack_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion

#pragma region SOUND_ATTACK_COMBO_B_02
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (m_iIndex == 2 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Attack_Claw_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_2.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Kick_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Kick_01.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion
}

void CElite_Grace::Attack_ComboB::State_Exit(CElite_Grace* pObject)
{
}

//초록색 공격
void CElite_Grace::Attack_ComboC::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 18;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 75;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Attack_ComboC::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    _vector vStartPos, vEndPos = {};
    if (m_iIndex == 18 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 10.f)
        {
            m_iIndex = 20;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(110.f);
        }
    }

    if (m_iIndex == 20 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 127.f)
        {
            m_iIndex = 21;
            pObject->m_bCan_Move_Anim = true;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
    }
    if (m_iIndex == 21 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {

        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 16.f && !m_bIs_Fired)
        {
            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
            pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos, 3);

            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_01.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.1f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_02.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.1f);
        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }
    }


#pragma region SOUND_ATTACK_COMBO_C_01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (m_iIndex == 20 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Voice_Ready_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Attack_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_3.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Attack_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_1.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Kick_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Kick_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_2.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion


#pragma region SOUND_ATTACK_COMBO_C_01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (m_iIndex == 21 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Throw_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion

}

void CElite_Grace::Attack_ComboC::State_Exit(CElite_Grace* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

void CElite_Grace::Parry_State::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 16;
    pObject->m_bCanHit = false;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 0;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_PARRY;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

#pragma region Effect
    pObject->m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, pObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), pObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CElite_Grace::Parry_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (m_iIndex == 16 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f)
        {
            if (rand() % 2 == 0)
                pObject->m_pState_Manager->ChangeState(new Parry_Attack_A(), pObject);
            else
                pObject->m_pState_Manager->ChangeState(new Parry_Attack_B(), pObject);
        }
    }


#pragma region SOUND_PARRY
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Parry_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Attack_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion

}

void CElite_Grace::Parry_State::State_Exit(CElite_Grace* pObject)
{
}

void CElite_Grace::Parry_Attack_A::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 13;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 75;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Parry_Attack_A::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 55.f)
        {
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }
    }


#pragma region SOUND_ATTACK_PARRY_A_01
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Kick_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Kick_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Kick_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Kick_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Attack_Combo_A_1_4.wav", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);
                    iter.isPlay = true;
                }
            }

        }
    }
#pragma endregion
}

void CElite_Grace::Parry_Attack_A::State_Exit(CElite_Grace* pObject)
{
}

void CElite_Grace::Parry_Attack_B::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 21;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 38;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Parry_Attack_B::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (m_iIndex == 21 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 16.f && !m_bIs_Fired)
        {  //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            _vector vStartPos, vEndPos = {};
            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
            pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos, 3);


        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }
    }
}

void CElite_Grace::Parry_Attack_B::State_Exit(CElite_Grace* pObject)
{
}

CElite_Grace::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}

void CElite_Grace::Hit_State::State_Enter(CElite_Grace* pObject)
{
    switch (m_iHitNum)
    {
    case 0:
        m_iIndex = 8;
        break;
    case 1:
        m_iIndex = 9;
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

void CElite_Grace::Hit_State::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);


#pragma region SOUND_HIT
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (m_iIndex == 8 && iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Hurt_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Hurt_01.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    iter.isPlay = true;
                }
            }

            if (m_iIndex == 9 && iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Hurt_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Hurt_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Grace::Hit_State::State_Exit(CElite_Grace* pObject)
{
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bPatternProgress = false;
}

void CElite_Grace::Shoot_Attack_A::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 30;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 38;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Shoot_Attack_A::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    _vector vStartPos, vEndPos = {};
    if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 62.f && !m_bIs_Fired)
        {
            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_02.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_02.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos);

        }
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 80.f)
        {
            m_iIndex = 32;
            m_bIs_Fired = false;
            pObject->RotateDegree_To_Player();
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(40.f);
        }
    }
    if (m_iIndex == 32 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 76.f && !m_bIs_Fired)
        {

            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_04.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos);

        }
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 120.f)
        {
            m_iIndex = 31;
            m_bIs_Fired = false;
            pObject->RotateDegree_To_Player();
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }

    }

    if (m_iIndex == 31 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 28.f && !m_bIs_Fired)
        {
            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_03.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_02.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos);

        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }
        }

    }
}

void CElite_Grace::Shoot_Attack_A::State_Exit(CElite_Grace* pObject)
{
}

void CElite_Grace::Shoot_Attack_B::State_Enter(CElite_Grace* pObject)
{
    m_iIndex = 33;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 38;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Grace::Shoot_Attack_B::State_Update(_float fTimeDelta, CElite_Grace* pObject)
{
    pObject->RotateDegree_To_Player();
    _vector vStartPos, vEndPos = {};
    if (m_iIndex == 33 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 24.f && !m_bIs_Fired)
        {
            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_01.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_02.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos, 3);

        }
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 31.f)
        {
            m_iIndex = 34;
            m_bIs_Fired = false;
            pObject->RotateDegree_To_Player();
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
    }
    if (m_iIndex == 34 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 24.f && !m_bIs_Fired)
        {
            //Sound
            pObject->m_pGameInstance->Play_Sound(L"Grace_Vocal_Slide_04.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.08f);
            pObject->m_pGameInstance->Play_Sound(L"Dagger_Small_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.08f);

            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            m_bIs_Fired = true;
            pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_DAGGER, vStartPos, vEndPos, 3);

        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Grace::Move_State(), pObject);
                break;
            }

        }
    }
}

void CElite_Grace::Shoot_Attack_B::State_Exit(CElite_Grace* pObject)
{
}
