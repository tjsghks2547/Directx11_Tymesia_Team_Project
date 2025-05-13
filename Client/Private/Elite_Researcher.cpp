#include "pch.h"
#include "Elite_Researcher.h"
#include "GameInstance.h"
#include "Body_Researcher.h"
#include "Animation.h"
#include "Player.h"
#include "Locked_On.h"
#include "Decorative_Researcher.h"
#include "Projectile_FireBall.h"
#include "Weapon_Book.h"
#include "Monster_HP_Bar.h"

CElite_Researcher::CElite_Researcher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CElite_Researcher::CElite_Researcher(const CElite_Researcher& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CElite_Researcher::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElite_Researcher::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 15.f;
    m_fActive_Distance = 5.f;
    m_fFireBall_Delete_Time = 5.f;
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

    m_pState_Manager = CState_Machine<CElite_Researcher>::Create();
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


    m_iMonsterSkill = PLAYER_SKILL::PLAYER_SKILL_CANESWORD;

    return S_OK;
}

void CElite_Researcher::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CElite_Researcher::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CElite_Researcher::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CElite_Researcher::Render()
{
    return S_OK;
}

void CElite_Researcher::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}

void CElite_Researcher::Active()
{
    m_bActive = true;
    m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), this);
}

void CElite_Researcher::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CElite_Researcher::Return_To_SpawnPoint_State(), this);
}

void CElite_Researcher::Stun()
{
    m_pState_Manager->ChangeState(new CElite_Researcher::Stun_State(), this);
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
#pragma region Effect_Stun
    m_pGameInstance->Play_Sound(L"Alert_KillChance.ogg", CHANNELID::SOUND_MONSTER_STUN, 0.3f); // 여기서 느려지면서 터지는 이펙트     
    m_pGameInstance->Set_SlowWorld(true);
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CElite_Researcher::Shoot_Calculate_Distance(_vector& vMyPos, _vector& vPlayerPos)
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

HRESULT CElite_Researcher::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CElite_Researcher::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CProjectile_FireBall::PROJECTILE_DESC ProjectileDesc = {};
    ProjectileDesc.pParent = this;
    ProjectileDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    ProjectileDesc.iCurLevel = iLevel;
    ProjectileDesc.fDelete_Time = &m_fFireBall_Delete_Time;
    ProjectileDesc.iDamage = &m_iMonster_Attack_Power;
    ProjectileDesc.fSpeedPerSec = 20.f;
    ProjectileDesc.fRotationPerSec = 0.f;
    ProjectileDesc.fPosition = m_vSpawnPoint;

    for (_uint i = 0; i < 5; i++)
    {
        if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_FireBall"), PROJECTILE_FIREBALL, &ProjectileDesc)))
            return E_FAIL;
    }

    CBody_Researcher::BODY_RESEARCHER_DESC BodyDesc{};
    BodyDesc.pParent = this;
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.iAttack = &m_iMonster_Attack_Power;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.bCatch_Special_Attack = &m_bCatch_Special_Attack;
    BodyDesc.bSpecial_Skill_Progress = &m_bSpecial_Skill_Progress;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Researcher"), LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Researcher_Body"), &BodyDesc)))
        return E_FAIL;

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Researcher"), TEXT("Com_Model")));
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

    _float3 fTest = { -90.f,0.f,-90.f };

    for (_uint i = 1; i <= 10; i++)
    {
        wstring strName = L"Part_Decorative_Researcher" + to_wstring(i);

        std::string BoneName = "BloodStab";

        if (i < 10)
            BoneName += "0";

        BoneName += std::to_string(i);

        CDecorative_Researcher::DECORATIVE_RESEARCHER_DESC Deco_Desc{};
        Deco_Desc.fPosition = _float4(1.f, 1.f, 1.f, 1.f);
        Deco_Desc.fScaling = _float3(0.025f, 0.001f, 0.025f);
        Deco_Desc.pRender = &m_bDecorative_Render;
        Deco_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix(BoneName.c_str());
        Deco_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
        Deco_Desc.pParentState = &m_iMonster_State;
        Deco_Desc.fSpeedPerSec = 0.f;
        Deco_Desc.fRotationPerSec = 0.f;

        if (FAILED(__super::Add_PartObject(strName.c_str(), LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Researcher"), &Deco_Desc)))
            return E_FAIL;
    }

    CWeapon_Book::WEAPON_BOOK_DESC Weapon_Desc{};
    Weapon_Desc.pParent = this;
    Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_02");
    Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_Desc.pParentModel = m_pModelCom;
    Weapon_Desc.pParentState = &m_iMonster_State;
    Weapon_Desc.fSpeedPerSec = 0.f;
    Weapon_Desc.fRotationPerSec = 0.f;
    Weapon_Desc.pNeed_Change_Anim = &m_bBook_Anim_Start;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Book"), LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Book"), &Weapon_Desc)))
        return E_FAIL;

    return S_OK;
}

void CElite_Researcher::PatternCreate()
{
    if (m_bActive && !m_bSpecial_Skill_Progress)
    {
        m_fSpecial_Skill_CoolTime += m_fTimeDelta;
    }

    if (!m_bPatternProgress && !m_bSpecial_Skill_Progress && m_bActive && !m_IsStun)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= m_fCoolTime && m_fDistance <= 5.f)
        {
            if (m_fSpecial_Skill_CoolTime >= 10.f)
            {
                m_pState_Manager->ChangeState(new CElite_Researcher::Attack_Special(), this);
            }
            else if (m_fDistance >= 3.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();

            m_fDelayTime = 0.f;
            m_bCanHit = false;
            m_bPatternProgress = true;
        }
    }
}

void CElite_Researcher::Near_Pattern_Create()
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
        m_pState_Manager->ChangeState(new CElite_Researcher::Attack_ComboA, this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CElite_Researcher::Attack_ComboB, this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CElite_Researcher::Attack_ComboC, this);
        break;
    default:
        break;
    }
}

void CElite_Researcher::Far_Pattern_Create()
{
    m_pState_Manager->ChangeState(new CElite_Researcher::Shoot_Attack_A, this);
}

void CElite_Researcher::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
            uniform_int_distribution<> Random_Parry(0, 1);

            m_iParryReadyHits = ParryCount_Random(gen);
            _uint iRandom = Random_Pattern(gen);

            if (iRandom == 0)
            {
                switch (Random_Parry(gen))
                {
                case 0:
                    m_pState_Manager->ChangeState(new CElite_Researcher::Parry_Attack_A(), this);
                    break;
                case 1:
                    m_pState_Manager->ChangeState(new CElite_Researcher::Parry_Attack_B(), this);
                    break;
                }
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
            m_pState_Manager->ChangeState(new CElite_Researcher::Hit_State(m_iHit_Motion_Index), this);
        }
    }
}

void CElite_Researcher::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CElite_Researcher::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CElite_Researcher* CElite_Researcher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElite_Researcher* pInstance = new CElite_Researcher(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CElite_Researcher");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CElite_Researcher::Clone(void* pArg)
{
    CElite_Researcher* pInstance = new CElite_Researcher(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CElite_Researcher");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CElite_Researcher::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

void CElite_Researcher::Idle_State::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 10;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_bCanHit = true;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Idle_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    pObject->RotateDegree_To_Player();
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
        pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
}

void CElite_Researcher::Idle_State::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

void CElite_Researcher::Move_State::State_Enter(CElite_Researcher* pObject)
{
    if (pObject->m_fDistance > 1.f)
        m_iIndex = 21;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 20;
            break;
        case 1:
            m_iIndex = 22;
            break;
        case 2:
            m_iIndex = 23;
            break;
        }
    }
    pObject->m_bCanHit = true;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Researcher::Move_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    pObject->RotateDegree_To_Player();
    if (m_iIndex == 21 && pObject->m_fDistance > pObject->m_fRootDistance)
        pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
    else if (m_iIndex == 20)
        pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
    else if (m_iIndex == 22)
        pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
    else if (m_iIndex == 23)
        pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
}

void CElite_Researcher::Move_State::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

CElite_Researcher::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}

void CElite_Researcher::Hit_State::State_Enter(CElite_Researcher* pObject)
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

void CElite_Researcher::Hit_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CElite_Researcher::Hit_State::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bPatternProgress = false;
}

void CElite_Researcher::Stun_State::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 17;
    pObject->m_iMonster_State = STATE_STUN;

    pObject->m_bCan_Move_Anim = true;
    pObject->m_bCanHit = false;

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_RESEARCHER;    
}

void CElite_Researcher::Stun_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    const _uint CurrentAnimIndex = pObject->m_pModelCom->Get_Current_Animation_Index();

  

    if (m_iIndex == 16 && CurrentAnimIndex == m_iIndex)
    {
        m_fTime += fTimeDelta;

        if (m_fTime >= 5.f)
        {
            m_iIndex = 15;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
        //플레이어 엑스큐전 상태가져오기
        if (pObject->m_bIsClosest  && *pObject->m_Player_State == CPlayer::STATE_RESEARCHER_EXECUTION )
        {
            pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Execution_State(), pObject);
            return;
        }
    }
    else if (m_iIndex == 17 && CurrentAnimIndex == m_iIndex)
    {
        if (pObject->m_bIsClosest && *pObject->m_Player_State == CPlayer::STATE_RESEARCHER_EXECUTION )
        {
            pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Execution_State(), pObject);
            return;
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            m_iIndex = 16;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
        }
    }
    else if (m_iIndex == 15 && CurrentAnimIndex == m_iIndex && pObject->m_pModelCom->GetAniFinish())
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

void CElite_Researcher::Stun_State::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

void CElite_Researcher::Execution_State::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 19;
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

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Execution_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
#pragma region EFFECT_EXECUTION
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
            {
                if (!strcmp(iter.szName, "Effect_Execution_1"))
                {
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_EXECUTION_1, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);

                    //Sound
                    pObject->m_pGameInstance->Play_Sound(TEXT("Researcher_ExecutIon_1.wav"), CHANNELID::SOUND_MONSTER_STUN, 10.f);
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로                
                }
                else if (!strcmp(iter.szName, "Effect_Execution_2"))
                {
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_EXECUTION_2, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
                    pObject->m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_EXECUTION_3, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));

                    //Sound
                    pObject->m_pGameInstance->Play_Sound(TEXT("Researcher_ExecutIon_2.wav"), CHANNELID::SOUND_MONSTER_STUN, 10.f);
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                }
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
        pObject->m_pGameInstance->Drop_Item(ITEM_TYPE::ITEM_HERB_5, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), pObject);
#pragma endregion
    }
}

void CElite_Researcher::Execution_State::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Return_To_SpawnPoint_State::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 21;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CElite_Researcher::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
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

void CElite_Researcher::Return_To_SpawnPoint_State::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
    pObject->m_bActive = false;
}

void CElite_Researcher::Attack_ComboA::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 0;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    //66
}

void CElite_Researcher::Attack_ComboA::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 116.f)
        {
            m_iIndex = 1;
            pObject->m_iMonster_Attack_Power = 66;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        }
    }
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }

#pragma region SOUND_COMBO_A
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (m_iIndex == 0 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Voice_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Voice_01.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

                if (!strcmp(iter.szName, "Event_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Attack_Combo_A_01.wav", CHANNELID::SOUND_MONSTER_VOICE, 20.f);
                    iter.isPlay = true;
                }

            }
        }

        if (m_iIndex == 1 && iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Attack_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Attack_Combo_A_02.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

            }

        }
    }
#pragma endregion
}

void CElite_Researcher::Attack_ComboA::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Attack_ComboB::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 2;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 123;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Attack_ComboB::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 100.f && !m_bBonus_Attack_Check)
        {
            m_bBonus_Attack_Check = true;
            if (rand() % 2 == 0)
            {
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Attack_ComboA(), pObject);
                return;
            }
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }


#pragma region SOUND_COMBO_B
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Attack_Combo_B_01.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Attack_ComboB::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Attack_ComboC::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 3;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Attack_ComboC::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 3 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 70.f && !m_bSpawn_Deco)
        {
            m_bSpawn_Deco = true;
            pObject->m_bDecorative_Render = true;
        }
        else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 95.f && pObject->m_bDecorative_Render)
        {
            pObject->m_bDecorative_Render = false;
        }

        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }


#pragma region SOUND_COMBO_C
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Attack_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Attack_01.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
                else  if (!strcmp(iter.szName, "Event_Warning_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Boss_SP_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                    const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                    _float4x4 matHeadWorld = {};
                    XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                    pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Attack_ComboC::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Parry_Attack_A::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 11;
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

void CElite_Researcher::Parry_Attack_A::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f && !m_bIs_Fired)
        {
            m_bIs_Fired = true;
            _vector vStartPos, vEndPos = {};
            pObject->RotateDegree_To_Player();
            pObject->m_iMonster_Attack_Power = 48;
            pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_FIREBALL, vStartPos, vEndPos);
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }

#pragma region SOUND_PARRY_ATTACK_A
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Parry_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_L_Up_Real2.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

                if (!strcmp(iter.szName, "Event_Throw_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Throw_Attack_02.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Parry_Attack_A::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Parry_Attack_B::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 12;
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

void CElite_Researcher::Parry_Attack_B::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 12 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f && !m_bIs_Fired)
        {
            m_bIs_Fired = true;
            _vector vStartPos, vEndPos = {};
            pObject->RotateDegree_To_Player();
            pObject->m_iMonster_Attack_Power = 48;
            pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_FIREBALL, vStartPos, vEndPos);
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }

#pragma region SOUND_PARRY_ATTACK_B
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Parry_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_L_Up_Real2.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

                if (!strcmp(iter.szName, "Event_Throw_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Throw_Attack_02.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Parry_Attack_B::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Shoot_Attack_A::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 5;
    pObject->m_bCanHit = false;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 48;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    pObject->m_bBook_Anim_Start = true;
}

void CElite_Researcher::Shoot_Attack_A::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f && !m_bIs_Fired)
        {
            _vector vStartPos, vEndPos = {};
            m_bIs_Fired = true;
            pObject->Shoot_Calculate_Distance(vStartPos, vEndPos);
            pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_FIREBALL, vStartPos, vEndPos);
        }
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
                break;
            case 1:
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
                break;
            }
        }
    }



#pragma region SOUND_SHOOT_ATTACK_A
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Throw_Ready_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Throw_Attack_01.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }

                if (!strcmp(iter.szName, "Event_Throw_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Throw_Attack_02.wav", CHANNELID::SOUND_MONSTER_ACTION, 20.f);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Shoot_Attack_A::State_Exit(CElite_Researcher* pObject)
{
}

void CElite_Researcher::Attack_Special::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 13;
    pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
    pObject->m_bSpecial_Skill_Progress = true;
    pObject->m_bCanHit = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_RESEARCH_CATCH;  
    pObject->m_iMonster_Attack_Power = 0;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Attack_Special::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
    if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
    {
        if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f &&
            pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 110.f &&
            pObject->m_bCatch_Special_Attack)
        {
            m_bCatch_Success = true;
            pObject->m_bCatch_Special_Attack = false;
            pObject->m_pState_Manager->ChangeState(new Attack_Special_Catch(), pObject);
            return;
        }

        //실패
        if (pObject->m_pModelCom->GetAniFinish())
        {
            _uint iRandom = rand() % 2;
            if (iRandom == 0)
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
            else
                pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
        }
    }


#pragma region SOUND_SPECIAL_ATTACK
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Catch_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_GrabYou_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.6f);

                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Sound_Warning"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Boss_SP_01.ogg", CHANNELID::SOUND_MONSTER_ACTION, 0.5f);
                    pObject->m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_WARNING, pObject->m_pTransformCom->Get_WorldMatrix_Ptr());
                    const _float4x4* matHead = pObject->m_pModelCom->Get_BoneMatrix("head");
                    _float4x4 matHeadWorld = {};
                    XMStoreFloat4x4(&matHeadWorld, XMLoadFloat4x4(matHead) * XMLoadFloat4x4(pObject->Get_Transfrom()->Get_WorldMatrix_Ptr()));
                    pObject->m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_WARNING, matHeadWorld);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion
}

void CElite_Researcher::Attack_Special::State_Exit(CElite_Researcher* pObject)
{
    if (!m_bCatch_Success)
    {
        //실패하고 Idle로 돌아가는 상황일때
        pObject->m_bSpecial_Skill_Progress = false;
        pObject->m_fSpecial_Skill_CoolTime = 0.f;
    }
}

void CElite_Researcher::Attack_Special_Catch::State_Enter(CElite_Researcher* pObject)
{
    m_iIndex = 14;
    pObject->m_iMonster_Attack_Power = 48;
    pObject->m_bCan_Move_Anim = true;

    _float teleportDistance = 0.7f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);
    vPlayerRight = XMVector3Normalize(vPlayerRight);

    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));
    vNewPos = XMVectorAdd(vNewPos, XMVectorScale(vPlayerRight, 0.1f));

    pObject->m_pTransformCom->LookAt(vPlayerPos);
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->m_pTransformCom->LookAt(vPlayerPos);

    pObject->m_bDecorative_Render = true;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CElite_Researcher::Attack_Special_Catch::State_Update(_float fTimeDelta, CElite_Researcher* pObject)
{
//#pragma region EFFECT_CATCH
//    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
//    {
//        if (iter.isPlay == false)
//        {
//            if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
//            {
//                if (!strcmp(iter.szName, "Effect_Special"))
//                {
//                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
//                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
//                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
//                }
//            }
//        }
//    }
//#pragma endregion


#pragma region SOUND_SPECIAL_CATCH
    for (auto& iter : *pObject->m_pModelCom->Get_VecAnimation().at(pObject->m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_SOUND && iter.isEventActivate == true)
            {
                if (!strcmp(iter.szName, "Event_Damage_1"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Player_Hit.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 20.f);
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
                    static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Damage_2"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Player_Hit.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 20.f);
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
                    static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Damage_3"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Player_Hit.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 20.f);
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
                    static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
                    iter.isPlay = true;
                }
                else if (!strcmp(iter.szName, "Event_Damage_4"))
                {
                    pObject->m_pGameInstance->Play_Sound(L"Researcher_Player_Hit.wav", CHANNELID::SOUND_MONSTER_DAMAGE, 20.f);
                    const _float4x4* matSpine = pObject->m_pModelCom->Get_BoneMatrix("spine_01");
                    pObject->m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_RESEARCHER_WORLD_SPECIAL, pObject->m_pTransformCom->Get_WorldMatrix_Ptr(), matSpine);
                    static_cast<CPlayer*>(pObject->m_pPlayer)->Increase_PlayerHp(pObject->m_iMonster_Attack_Power * -1);
                    iter.isPlay = true;
                }
            }
        }
    }
#pragma endregion

    if (m_iIndex == 14 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        if (iRandom == 0)
            pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Idle_State(), pObject);
        else
            pObject->m_pState_Manager->ChangeState(new CElite_Researcher::Move_State(), pObject);
    }
}

void CElite_Researcher::Attack_Special_Catch::State_Exit(CElite_Researcher* pObject)
{
    pObject->m_fSpecial_Skill_CoolTime = 0.f;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bDecorative_Render = false;
    pObject->m_bSpecial_Skill_Progress = false;
}
