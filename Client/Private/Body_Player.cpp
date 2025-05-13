#include "pch.h" 
#include "Body_Player.h" 
#include "GameInstance.h"
#include "Animation.h"
#include "Camera_Free.h"
#include "StateMgr.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    CBody_Player::BODY_PLAYER_DESC* pDesc = static_cast<CBody_Player::BODY_PLAYER_DESC*>(pArg);

    m_pParent = pDesc->pParent;
    m_pParentState = pDesc->pParentState;
    m_pPreParentState = pDesc->pPreParentState;
    m_pParentPhsaeState = pDesc->pParentPhaseState;
    m_pParentNextStateCan = pDesc->pParentNextStateCan;
    m_pParentStateMgr = pDesc->pParentStateMgr;
    m_pParentNavigationCom = pDesc->pParentNavigationCom;
    m_pParentActor = pDesc->pParentActor;
    m_pParentMonsterExecute = pDesc->pParentExectueMonsterState;
    m_pParentHp = pDesc->pParentHp;
    m_pParentMp = pDesc->pParentMp;
    m_pParentTalent = pDesc->pParentTalent;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    /* 플레이어 다이나믹 본 설정하는 코드 */
    //m_pModelCom->Set_BoneType("Bip001-Xtra04Opp",    BONE_TYPE::DYNAMIC_BONE, false);       
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp02", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp03", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp04", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp05", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp06", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp07", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp08", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Opp09", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05OppNub", BONE_TYPE::DYNAMIC_BONE, false);



    //m_pModelCom->Set_BoneType("Bip001-Xtra04",    BONE_TYPE::DYNAMIC_BONE, false);  // 애만 계속 업데이트    	    
    m_pModelCom->Set_BoneType("Bip001-Xtra05", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0502", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0503", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0504", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0505", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0506", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0507", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0508", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra0509", BONE_TYPE::DYNAMIC_BONE, false);
    m_pModelCom->Set_BoneType("Bip001-Xtra05Nub", BONE_TYPE::DYNAMIC_BONE, false);




    m_pModelCom->SetUp_Animation(0, true);

    m_iCurrentLevel = static_cast<LEVELID>(pDesc->iCurLevel); //종한 추가 Level 전환때문에

    m_pSet_Body_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Body_State();
    m_pSet_Claw_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Claw_Weapon_State();
    m_pSet_Halberd_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Halberd_State();
    m_pSet_Right_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Right_Weapon_State();
    m_pSet_Scythe_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Scythe_State();
    m_pSet_Axe_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Axe_State();
    m_pSet_Player_Camera_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Player_Camera_State();
    m_pSet_JavelinSword_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_JavelinSword_State();
    m_pSet_GreadSword_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_GreadSword_State();
    m_pSet_Cane_Weapon_States = dynamic_cast<CPlayer*>(m_pParent)->Get_Cane_State();
    m_pSet_Player_Skill_State = dynamic_cast<CPlayer*>(m_pParent)->Get_PlayerSkill_State();

    m_fRespawnPosPtr = dynamic_cast<CPlayer*>(m_pParent)->Get_RespawnPosPtr();


    m_pfClawParentTime = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawTimePtr();
    m_pfClawFinishTime = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawFinishTimePtr();
    m_pfClawAppearTimer = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawAppearTimerPtr();
    m_pbClawDeadOn = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawDeadOnPtr();
    m_pbClawAppear = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawAppearPtr();


    /* 파동을 위한 포인터 주소 저장 */
    m_mRightFootBoneMartix = m_pModelCom->Get_BoneMatrix("Bip001-R-Toe0");
    m_mLeftFootBoneMartix = m_pModelCom->Get_BoneMatrix("Bip001-L-Toe0");

    /* 스킬 쿨타임을 다루기 위한 변수*/
    m_pParentSkillCoolTime = static_cast<CPlayer*>(m_pParent)->Get_Skill_CoolTime_Ptr();

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    m_fDissolveAmount = 0.3f;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));

    if (*m_pParentState == CPlayer::STATE_DEAD)
    {
        m_fDeadStartTimer += fTimeDelta;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_LOCKON;
        /* 락온 풀기 */
        static_cast<CPlayer*>(m_pParent)->Set_Lockon(false);
    }


    if (*m_pParentState == CPlayer::STATE_DEAD && m_fDeadStartTimer > 1.f)
    {
        /* 플레이어 죽음 알림(유빈)*/
        if (0 == m_fDeadTimer)
        {
            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_Landing"), true);
            m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_1Dead")), true);
            m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_1Dead")), true);
        }

        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;


        m_pCamera->Target_Reset();

        if (m_fDeadTimer >= 2.5f && m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
        {
            m_pGameInstance->Pop_Item(ITEM_TYPE::ITEM_DEADBRANCH, m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), this, dynamic_cast<CPlayer*>(m_pParent)->Get_MemoryFragment());
            dynamic_cast<CPlayer*>(m_pParent)->Increase_MemoryFragment(-(dynamic_cast<CPlayer*>(m_pParent)->Get_MemoryFragment()));

            *m_pParentPhsaeState = 0;
            *m_pParentState = CPlayer::STATE_START_WALK;
            *m_pParentPhsaeState = CPlayer::PHASE_START;
            *m_pPreParentState = CPlayer::STATE_DEAD;
            dynamic_cast<CPlayer*>(m_pParent)->Increase_PlayerHp(dynamic_cast<CPlayer*>(m_pParent)->Get_FullHp());

            *m_pParentNextStateCan = true;

            m_fDeadTimer = 0.f;
            m_fFinishTime = 0.f;
            m_fDeadStartTimer = 0.f;

            // 시작 위치 


            m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&m_pCamera->Get_FirstCamDir()));

            _vector respawnPos = XMLoadFloat4(static_cast<CPlayer*>(m_pParent)->Get_RespawnPosPtr());
            _vector OffSetRespawnPos = XMVectorSetY(respawnPos, respawnPos.m128_f32[1] + 1.f);


            m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, OffSetRespawnPos);

            m_pGameInstance->Respawn_Monster(MONSTER_CATEGORY::CATEGORY_BOSS);
            m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.6f);

            switch (m_iCurrentLevel)
            {
            case LEVEL_TUTORIAL:
                m_pGameInstance->PlayBGM(L"TutoMapBGM.ogg", 0.15f);
                break;
            case LEVEL_SEAOFTREES:
                m_pGameInstance->PlayBGM(L"Sea_of_Trees_Level_-_Exploration.ogg", 0.15f);
                m_pGameInstance->Delete_GameObject_From_Layer(LEVEL_SEAOFTREES, TEXT("Layer_Boss_Map"));
                m_pGameInstance->Set_LoadObjects();
                break;
            case LEVEL_ROYALGARDEN:
                m_pGameInstance->PlayBGM(L"Garden_Level_-_Exploration.ogg", 0.15f);
                break;
            case LEVEL_OCEAN:
                break;
            }

            /* 락온 풀기 */
            //static_cast<CPlayer*>(m_pParent)->Set_Lockon(false);     

        }
    }

}

void CBody_Player::Update(_float fTimeDelta)
{

#pragma region  각 상황에 따른 함수 

    switch (*m_pParentState)
    {
    case CPlayer::STATE_IDLE:
        STATE_IDLE_Method();
        break;
    case CPlayer::STATE_RUN:
        STATE_RUN_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_B:
        STATE_LOCK_ON_RUN_B_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_BL:
        STATE_LOCK_ON_RUN_BL_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_BR:
        STATE_LOCK_ON_RUN_BR_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_FL:
        STATE_LOCK_ON_RUN_FL_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_FR:
        STATE_LOCK_ON_RUN_FR_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_L:
        STATE_LOCK_ON_RUN_L_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_R:
        STATE_LOCK_ON_RUN_R_Method();
        break;
    case CPlayer::STATE_ATTACK:
        STATE_ATTACK_Method();
        break;
    case CPlayer::STATE_ATTACK_L1:
        STATE_ATTACK_L1_Method();
        break;
    case CPlayer::STATE_ATTACK_L2:
        STATE_ATTACK_L2_Method();
        break;
    case CPlayer::STATE_ATTACK_L3:
        STATE_ATTACK_L3_Method();
        break;
    case CPlayer::STATE_ATTACK_L4:
        STATE_ATTACK_L4_Method();
        break;
    case CPlayer::STATE_ATTACK_L5:
        STATE_ATTACK_L5_Method();
        break;
    case CPlayer::STATE_ATTACK_LONG_CLAW_01:
        STATE_ATTACK_LONG_CLAW_01_Method();
        break;
    case CPlayer::STATE_ATTACK_LONG_CLAW_02:
        STATE_ATTACK_LONG_CLAW_02_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_B:
        STATE_LOCK_ON_EVADE_B_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_L:
        STATE_LOCK_ON_EVADE_L_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_R:
        STATE_LOCK_ON_EVADE_R_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_F:
        STATE_LOCK_ON_EVADE_F_Method();
        break;
    case CPlayer::STATE_PARRY_L:
        STATE_PARRY_L_Method();
        break;
    case CPlayer::STATE_PARRY_R:
        STATE_PARRY_R_Method();
        break;
    case CPlayer::STATE_HurtMFR_L:
        STATE_HurtMFR_L_Method();
        break;
    case CPlayer::STATE_HurtMFR_R:
        STATE_HurtMFR_R_Method();
        break;
    case CPlayer::STATE_HURT_LF:
        STATE_HURT_LF_Method();
        break;
    case CPlayer::STATE_HURT_SF:
        STATE_HURT_SF_Method();
        break;
    case CPlayer::STATE_HURT_SL:
        STATE_HURT_SL_Method();
        break;
    case CPlayer::STATE_HURT_HURXXLF:
        STATE_HURT_HURXXLF_Method();
        break;
    case CPlayer::STATE_HURT_KNOCKBACK:
        STATE_HURT_KNOCKBACK_Method();
        break;
    case CPlayer::STATE_HURT_KNOCKDOWN:
        STATE_HURT_KNOCKDOWN_Method();
        break;
    case CPlayer::STATE_HURT_FALLDOWN:
        STATE_HURT_FALLDOWN_Method();
        break;
    case CPlayer::STATE_HURT_FALLDOWN_END:
        STATE_HURT_FALLDOWN_END_Method();
        break;
    case CPlayer::STATE_WEAK_GETUP_F:
        STATE_WEAK_GETUP_F_Method();
        break;
    case CPlayer::STATE_REBOUND_R:
        STATE_REBOUND_R_Method();
        break;
    case CPlayer::STATE_STUNNED_START:
        STATE_STUNNED_START_Method();
        break;
    case CPlayer::STATE_STUNNED_LOOP:
        STATE_STUNNED_LOOP_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_L:
        STATE_PARRY_DEFLECT_L_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_L_UP:
        STATE_PARRY_DEFLECT_L_UP_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_R:
        STATE_PARRY_DEFLECT_R_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_R_UP:
        STATE_PARRY_DEFLECT_R_UP_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_R:
        STATE_NORMAL_EVADE_R_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_L:
        STATE_NORMAL_EVADE_L_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_FR:
        STATE_NORMAL_EVADE_FR_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_FL:
        STATE_NORMAL_EVADE_FL_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_F:
        STATE_NORMAL_EVADE_F_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_BR:
        STATE_NORMAL_EVADE_BR_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_BL:
        STATE_NORMAL_EVADE_BL_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_B:
        STATE_NORMAL_EVADE_B_Method();
        break;
    case CPlayer::STATE_HARMOR_EXECUTION:
        STATE_HARMOR_EXECUTION_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE:
        STATE_STUN_EXECUTE_Method();
        break;
    case CPlayer::STATE_LV1Villager_M_Execution:
        STATE_LV1Villager_M_Execution_Method();
        break;
    case CPlayer::STATE_Joker_Execution:
        STATE_Joker_Execution_Method();
        break;
    case CPlayer::STATE_Varg_Execution:
        STATE_Varg_Execution_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_START:
        STATE_ARCHIVE_SIT_START_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_LOOP:
        STATE_ARCHIVE_SIT_LOOP_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_GETUP:
        STATE_ARCHIVE_SIT_GETUP_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_LIGHT_UP:
        STATE_ARCHIVE_SIT_LIGHT_UP_Method();
        break;
    case CPlayer::STATE_HEAL:
        STATE_HEAL_Method();
        break;
    case CPlayer::STATE_DEAD:
        STATE_DEAD_Method();
        break;
    case CPlayer::STATE_START_WALK:
        STATE_START_WALK_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_START:
        STATE_CLAW_CHARGE_START_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_LOOP:
        STATE_CLAW_CHARGE_LOOP_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_FULL_ATTACK:
        STATE_CLAW_CHARGE_FULL_ATTACK_Method();
        break;
    case CPlayer::STATE_CLAW_LONG_PLUNDER_ATTACK2:
        STATE_CLAW_LONG_PLUNDER_ATTACK2_Method();
        break;
    case CPlayer::STATE_HALBERDS_B:
        STATE_HALBERDS_B_Method();
        break;
    case CPlayer::STATE_SCYTHE_B:
        STATE_SCYTHE_B_Method();
        break;
    case CPlayer::STATE_CATCHED:
        STATE_CATCHED_Method();
        break;
    case CPlayer::STATE_GET_UP:
        STATE_GET_UP_Method();
        break;
    case CPlayer::STATE_VARG_RUN_EXECUTION:
        STATE_VARG_RUN_EXECUTION_Method();
        break;
    case CPlayer::STATE_AXE:
        STATE_AXE_Method();
        break;
    case CPlayer::STATE_LIGHT_EXECUTION_L:
        STATE_LIGHT_EXECUTION_L_Method();
        break;
    case CPlayer::STATE_LIGHT_EXECUTION_R:
        STATE_LIGHT_EXECUTION_R_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_START:
        STATE_LADDER_CLIMB_START_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_L_DOWN:
        STATE_LADDER_CLIMB_L_DOWN_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_L_DOWN_END:
        STATE_LADDER_CLIMB_L_DOWN_END_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_L_UP:
        STATE_LADDER_CLIMB_L_UP_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_L_UP_END:
        STATE_LADDER_CLIMB_L_UP_END_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_L_IDLE:
        STATE_LADDER_CLIMB_L_IDEL_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_DOWN:
        STATE_LADDER_CLIMB_R_DOWN_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_DOWN_END:
        STATE_LADDER_CLIMB_R_DOWN_END_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_UP:
        STATE_LADDER_CLIMB_R_UP_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_UP_END:
        STATE_LADDER_CLIMB_R_UP_END_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_UP_REVERSE_END:
        STATE_LADDER_CLIMB_R_UP_REVERSE_END_Method();
        break;
    case CPlayer::STATE_LADDER_CLIMB_R_IDLE:
        STATE_LADDER_CLIMB_R_IDEL_Method();
        break;
    case CPlayer::STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START:
        STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START_Method();
        break;
    case CPlayer::STATE_MAGICIAN_Execution:
        STATE_MAGICIAN_Execution_Method();
        break;
    case CPlayer::STATE_MAGICIAN_MUTATION_Execution:
        STATE_MAGICIAN_MUTATION_Execution_Method();
        break;
    case CPlayer::STATE_GRACE_Execution:
        STATE_GRACE_Execution_Method();
        break;
    case CPlayer::STATE_PUNCH_MAN_Execution:
        STATE_PUNCH_MAN_Execution_Method();
        break;
    case CPlayer::STATE_MAGICIAN_CATCH:
        STATE_MAGICIAN_CATCH_Method();
        break;
    case CPlayer::STATE_CANE_SWORD_SP02:
        STATE_CANE_SWORD_SP02_Method();
        break;
    case CPlayer::STATE_GREATSWORD:
        STATE_GREATSWORD_Method();
        break;
    case CPlayer::STATE_JAVELIN_SWORD:
        STATE_JAVELIN_SWORD_Method();
        break;
    case CPlayer::STATE_SPRINT:
        STATE_SPRINT_Method();
        break;
    case CPlayer::STATE_SPRINT_ATTACK_L1:
        STATE_SPRINT_ATTACK_L1_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_URD:
        STATE_STUN_EXECUTE_START_URD_Method();
        break;
    case CPlayer::STATE_URD_EXECUTION:
        STATE_URD_EXECUTION_Method();
        break;
    case CPlayer::STATE_LOBBY_IDLE_01:
        STATE_LOBBY_IDLE_01_Method();
        break;
    case CPlayer::STATE_LOBBY_IDLE_01_END:
        STATE_LOBBY_IDLE_01_END_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_VARG:
        STATE_STUN_EXECUTE_START_VARG_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_PUNCHMAN:
        STATE_STUN_EXECUTE_START_PUNCHMAN_Method();
        break;
    case CPlayer::STATE_HURT_MUTATION_MAGICIAN_CATCH:
        STATE_HURT_MUTATION_MAGICIAN_CATCH_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_BAT:
        STATE_STUN_EXECUTE_START_BAT_Method();
        break;
    case CPlayer::STATE_BAT_EXECUTION:
        STATE_BAT_EXECUTION_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_MAGICIAN:
        STATE_STUN_EXECUTE_START_MAGICIAN_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN:
        STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_RESEARCHER:
        STATE_STUN_EXECUTE_START_RESEARCHER_Method();
        break;
    case CPlayer::STATE_RESEARCHER_EXECUTION:
        STATE_RESEARCHER_EXECUTION_Method();
        break;
    case CPlayer::STATE_HURT_RESEARCHER_CATCHED:
        STATE_HURT_RESEARCHER_CATCHED_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_HARMOR:
        STATE_STUN_EXECUTE_START_HARMOR_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_JOKER:
        STATE_STUN_EXECUTE_START_JOKER_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_GRACE:
        STATE_STUN_EXECUTE_START_GRACE_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE_START_VILLAGEM1:
        STATE_STUN_EXECUTE_START_VILLAGEM1_Method();
        break;
    default:
        break;
    }
#pragma endregion 

    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));


    CPlayer::STATE curState = (CPlayer::STATE)*m_pParentState;

#pragma region 이벤트 관련 작업
    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (m_pSet_Body_States->count(curState))
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_COLLIDER:
                    if (iter.isEventActivate == true)
                    {
                        if (!strcmp(iter.szName, "Evade"))
                        {
                            if (*m_pParentState != CPlayer::STATE_NORMAL_EVADE_F)
                                m_pGameInstance->Sub_Actor_Scene(m_pParentActor);
                        }
                    }

                    break;
                case EVENT_STATE:
                    if (iter.isEventActivate == true)
                    {
                        if (!strcmp(iter.szName, "Camera_Shake"))
                        {
                            // 카메라 포인터 가져오고 싶다.

                            m_pCamera->ShakeOn(400.f, 400.f, 4.f, 4.f);
                        }

                        if (!strcmp(iter.szName, "Camera_Parry_Zoom_In"))
                        {
                            m_pCamera->Set_Camera_ZoomInSpeed(10.f);
                            m_pCamera->ZoomIn();
                        }

                        if (!strcmp(iter.szName, "Zoom_In_Blur"))
                        {
                            m_fZoomBlurDeltaTime += fTimeDelta;
                            m_pCamera->ShakeOn(400.f, 400.f, 2.f, 2.f);
                            m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());
                            m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurDeltaTime * 0.3f);
                        }
                    }

                    else
                    {
                        if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                        {
                            if (!strcmp(iter.szName, "Camera_Parry_Zoom_In"))
                            {
                                // 카메라 포인터 가져오고 싶다.     
                                m_pCamera->ResetZoomInCameraPos(10.f);
                            }

                            if (!strcmp(iter.szName, "Zoom_In_Blur"))
                            {
                                m_fZoomBlurDeltaTime = 0.f;
                                m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
                            }
                        }
                    }
                    break;
                case EVENT_EFFECT:
                    if (iter.isEventActivate)
                    {
                        iter.isPlay = true;
                    }
                    break;
                }
            }

            else
            {
                m_pCamera->ResetZoomInCameraPos(1.f);
            }
        }
    }

    else
    {


        if (*m_pParentPhsaeState != CPlayer::PHASE_EXECUTION)
            m_pGameInstance->Add_Actor_Scene(m_pParentActor);


        if (*m_pParentPhsaeState != CPlayer::PHASE_EXECUTION
            && !(m_pSet_Claw_Weapon_States->count(curState))
            && !(m_pSet_Halberd_Weapon_States->count(curState))
            && !(m_pSet_Right_Weapon_States->count(curState))
            && !(m_pSet_Scythe_Weapon_States->count(curState))
            && !(m_pSet_Axe_Weapon_States->count(curState))
            && !(m_pSet_JavelinSword_Weapon_States->count(curState))
            && !(m_pSet_GreadSword_Weapon_States->count(curState))
            && !(m_pSet_Cane_Weapon_States->count(curState))
            && !(m_pSet_Player_Camera_States->count(curState))
            )
        {
            m_pCamera->ResetZoomInCameraPos(1.f);
            m_fZoomBlurDeltaTime = 0.f;
            m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        }
    }

#pragma endregion  

}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CBody_Player::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    switch (m_iRenderState)
    {
    case STATE_NORMAL_RENDER:
        STATE_NORMAL_Render();
        break;
    case STATE_CLAW_RENDER:
        STATE_ATTACK_LONG_CLAW_Render();
        break;
    case STATE_DEAD_RENDER:
        STATE_DEAD_Render();
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CBody_Player::Render_Shadow()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;/*
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
        return E_FAIL;*/


    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(2);
        m_pModelCom->Render(i);
    }

    return S_OK;
}



HRESULT CBody_Player::STATE_NORMAL_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    if (!(*m_pParentPhsaeState & CPlayer::PHASE_NO_RENDER))
    {
        for (_uint i = 0; i < iNumMeshes; i++)
        {
            /* 평상시 모드
            (i == 11  깃털 ),
            (i == 12(왼) , i == 13(오), 발톱, ),
            (i == 10(왼) , i == 9(오),  팔목 장식)
            (i == 4(왼) ,   i == 8(오),  어깨 장식)*/
            if (i == 11
                || i == 12
                || i == 5
                || i == 10
                || i == 9
                || i == 4  // 왼손 팔목 깃털  
                || i == 8
                || i == 13)   // 오른손 발톱 
                continue;

            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
                return E_FAIL;


            m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

            if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네
                return E_FAIL;


            switch (i)
            {
            case 0:
                if (FAILED(m_pORM_HeadTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture", 0)))
                    return E_FAIL;

                m_pShaderCom->Begin(10);
                m_pModelCom->Render(i);
                break;
            case 1:
                if (FAILED(m_pORM_BodyTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture", 0)))
                    return E_FAIL;

                m_pShaderCom->Begin(10);
                m_pModelCom->Render(i);
                break;
            case 2:
                if (FAILED(m_pORM_ClockTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture", 0)))
                    return E_FAIL;

                m_pShaderCom->Begin(11);
                m_pModelCom->Render(i);
                break;
            case 3:
                if (FAILED(m_pORM_LegTexture->Bind_ShaderResource(m_pShaderCom, "g_ORM_Texture", 0)))
                    return E_FAIL;

                m_pShaderCom->Begin(10);
                m_pModelCom->Render(i);
                break;

            default:
                m_pShaderCom->Begin(0);
                m_pModelCom->Render(i);
                break;
            }


        }
    }

    //m_fClawDeadTimer = 0.f;
    //m_fClawFinishTime = 0.f;

    return S_OK;
}

HRESULT CBody_Player::STATE_ATTACK_LONG_CLAW_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        /* 평상시 모드
        (i == 11  깃털 ),
        (i == 12(왼) , i == 5(오), 발톱, ),
        (i == 10(왼) , i == 9(오),  팔목 장식)
        (i == 4(왼) ,   i == 8(오),  어깨 장식)*/    //13, 5,

        if (i == 11
            || i == 12
            || i == 8  // 검정색 망토 
            || i == 4
            || i == 13)
            continue;

        else if (i == 5  // 오른쪽 발톱 생성 
            || i == 9
            || i == 10)
        {
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
                return E_FAIL;

            m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

            if (FAILED(m_pDissolveNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 9))) //19
                return E_FAIL;

            if (FAILED(m_pDissolveNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveNoiseTexture", 4)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", m_pfClawParentTime, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", m_pfClawFinishTime, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_ReverseDissolveTime", m_pfClawAppearTimer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Dead", m_pbClawDeadOn, sizeof(_bool))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Appear", m_pbClawAppear, sizeof(_bool))))
                return E_FAIL;

            if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네             
                return E_FAIL;

            m_pShaderCom->Begin(8);
            m_pModelCom->Render(i);
        }

        else
        {
            if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
                return E_FAIL;


            m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

            if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네 
                return E_FAIL;

            if (i == 2)
            {
                m_pShaderCom->Begin(9);
                m_pModelCom->Render(i);
            }

            m_pShaderCom->Begin(0);
            m_pModelCom->Render(i);
        }
    }
    return S_OK;
}

HRESULT CBody_Player::STATE_DEAD_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        /* 디졸브 텍스처 넣기 */
        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
            return E_FAIL;

        /* 디졸브 상수들 넣기 */
        if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeStart", &m_fDeadStartTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
            return E_FAIL;

        /* 평상시 모드
        (i == 11  깃털 ),
        (i == 12(왼) , i == 5(오), 발톱, ),
        (i == 10(왼) , i == 9(오),  팔목 장식)
        (i == 4(왼) ,   i == 8(오),  어깨 장식)*/
        if (i == 11
            || i == 12
            || i == 10
            || i == 4)
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네 
            return E_FAIL;

        m_pShaderCom->Begin(6);
        m_pModelCom->Render(i);
    }
    return S_OK;
}


void CBody_Player::STATE_IDLE_Method()
{
    m_pModelCom->SetUp_Animation(2, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_RUN_Method()
{
    m_pModelCom->SetUp_Animation(9, true);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_Current_Animation_Index() == 9)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(9)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                        else
                        {
                            if (!strcmp(iter.szName, "Walk_After_Effect_Left"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_R, m_pParentWorldMatrix, m_mRightFootBoneMartix);

                            }

                            else if (!strcmp(iter.szName, "Walk_After_Effect_Right"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_L, m_pParentWorldMatrix, m_mLeftFootBoneMartix);
                            }

                        }

                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.1f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.1f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.1f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.1f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 1.f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 1.f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    /* 3월 8일 추가 */
    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
}
void CBody_Player::STATE_WALK_Method()
{
}
void CBody_Player::STATE_ATTACK_Method()
{
}


void CBody_Player::STATE_SPRINT_ATTACK_L1_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(279, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 279)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(279)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LB_Attack_1.ogg", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }



    if (*m_pParentState == CPlayer::STATE_SPRINT_ATTACK_L1 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 80.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;

    }

    if (*m_pParentState == CPlayer::STATE_SPRINT_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(279)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);


        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }
}

void CBody_Player::STATE_LOBBY_IDLE_01_Method()
{
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(242, true);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_bFirstCameraPos)
    {
        _float4x4 CamWorldMatrix = {};

        CamWorldMatrix._11 = 0.421f * -1.f;
        CamWorldMatrix._12 = 0.0f;
        CamWorldMatrix._13 = 0.906f * -1.f;
        CamWorldMatrix._14 = 0.0f;

        CamWorldMatrix._21 = -0.15f;
        CamWorldMatrix._22 = 0.98f;
        CamWorldMatrix._23 = 0.07f;
        CamWorldMatrix._24 = 0.0f;

        CamWorldMatrix._31 = 0.893f;
        CamWorldMatrix._32 = 0.168f;
        CamWorldMatrix._33 = -0.41f;
        CamWorldMatrix._34 = 0.0f;

        CamWorldMatrix._41 = 0.321f;
        CamWorldMatrix._42 = 1.516f;
        CamWorldMatrix._43 = -14.156f;
        CamWorldMatrix._44 = 1.0f;

        m_pCamera->Get_Transfrom()->Set_WorldMatrix(CamWorldMatrix);
        m_bFirstCameraPos = false;
    }

    if (m_pGameInstance->isKeyEnter(DIK_W))
        *m_pParentState = CPlayer::STATE_LOBBY_IDLE_01_END;
}

void CBody_Player::STATE_LOBBY_IDLE_01_END_Method()
{

    m_pModelCom->SetUp_Animation(243, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(243)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_START;
        m_bFirstCameraPos = true;
    }
}


void CBody_Player::STATE_ATTACK_L1_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);
    m_pModelCom->SetUp_Animation(3, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 3)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(3)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LB_Attack_1.ogg", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L1 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 60.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
#pragma region 스킬 연계기   
        /* 여기다 넣어놓기 */


#pragma endregion 

    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L1
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                /* 4월 14일 여기다가 hitstop 애니메이션 스피드 이거 1.f로 설정해야할듯*/

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L1
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 


    m_iRenderState = STATE_NORMAL_RENDER;
    //*m_pParentNextStateCan = true;  
}

void CBody_Player::STATE_ATTACK_L2_Method()
{
    m_pModelCom->SetUp_Animation(4, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 4)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(4)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LB_Attack_2.ogg", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L2 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L2 && m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L2
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L2
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;
    //*m_pParentNextStateCan = true;
}
void CBody_Player::STATE_ATTACK_L3_Method()
{
    m_pModelCom->SetUp_Animation(5, false);


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 5)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(5)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LB_Attack_3.ogg", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L3 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L3 && m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L3
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L3
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;

}
void CBody_Player::STATE_ATTACK_L4_Method()
{
    m_pModelCom->SetUp_Animation(276, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 276)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(276)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    if (!strcmp(iter.szName, "Attack_Sound_1"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_LB_Attack_4_1.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    if (!strcmp(iter.szName, "Attack_Sound_2"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_LB_Attack_4_2.wav", CHANNELID::SOUND_PLAYER_ATTACK_2, 4.f);
                        iter.isPlay = true;
                    }
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L4 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L4 && m_pModelCom->Get_VecAnimation().at(276)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L4
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L4
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;

}
void CBody_Player::STATE_ATTACK_L5_Method()
{

    m_pModelCom->SetUp_Animation(277, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 277)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(277)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LB_Attack_5.ogg", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }



    if (*m_pParentState == CPlayer::STATE_ATTACK_L5 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L5 && m_pModelCom->Get_VecAnimation().at(277)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L5
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L5
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;

}

void CBody_Player::STATE_ATTACK_LONG_CLAW_01_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(0, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 0)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(0)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LongClaw_01.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW_RENDER;
    //*m_pParentNextStateCan = true;

}
void CBody_Player::STATE_ATTACK_LONG_CLAW_02_Method()
{
    m_pModelCom->SetUp_Animation(1, false);

    m_pModelCom->Get_VecAnimation().at(1)->Set_StartOffSetTrackPosition(16.f);


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 1)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(1)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_LongClaw_02_Real.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW_RENDER;
    //*m_pParentNextStateCan = true;

}

void CBody_Player::STATE_LOCK_ON_RUN_B_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(6, true);

    if (m_pModelCom->Get_Current_Animation_Index() == 6)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(6)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_BL_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(7, true);

    if (m_pModelCom->Get_Current_Animation_Index() == 7)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(7)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case true:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                default:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }


    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_BR_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(8, true);

    if (m_pModelCom->Get_Current_Animation_Index() == 8)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(8)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_FL_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(10, true);

    if (m_pModelCom->Get_Current_Animation_Index() == 10)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(10)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case true:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                default:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_FR_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(11, true);
    if (m_pModelCom->Get_Current_Animation_Index() == 11)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(11)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(12, true);

    if (m_pModelCom->Get_Current_Animation_Index() == 12)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(12)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_R_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(13, true);
    if (m_pModelCom->Get_Current_Animation_Index() == 13)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(13)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Walk_Water_Effect_Right"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                            }

                            if (!strcmp(iter.szName, "Walk_Water_Effect_Left"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                            }
                        }
                    }
                }
                break;
                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.05f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.05f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.5f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.5f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_EVADE_F_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(18, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 18)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(18)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Lock_On_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(18)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }



}
void CBody_Player::STATE_LOCK_ON_EVADE_B_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(17, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 17)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(17)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Lock_On_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(17)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(19, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 19)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(19)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Lock_On_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(19)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_R_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(20, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 20)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(20)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Lock_On_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(20)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_PARRY_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(15, false);


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 15)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(15)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_L_Real.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }



    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(15)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }

    /* 패링 조건 */
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() <= 70.f)
    {

        *m_pParentPhsaeState |= CPlayer::PHASE_PARRY; // 
    }

    else
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;

    *m_pParentNextStateCan = true; // 여기서 보간이 잘 안됨 이러면 

}
void CBody_Player::STATE_PARRY_R_Method()
{
    m_pModelCom->SetUp_Animation(16, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    m_pModelCom->Get_VecAnimation().at(16)->Set_StartOffSetTrackPosition(10.f);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 16)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(16)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_R.wav", CHANNELID::SOUND_PLAYER_ACTION_2, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(16)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }

    /* 패링 조건 */
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() <= 70.f
        || !m_pModelCom->Get_LerpFinished())
    {

        *m_pParentPhsaeState |= CPlayer::PHASE_PARRY;
    }

    else
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    *m_pParentNextStateCan = true; // 여기서 보간이 잘 안됨 
}
void CBody_Player::STATE_PARRY_DEFLECT_LARGE_Method()
{
    m_pModelCom->SetUp_Animation(55, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(55)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_L_UP_Method()
{

    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(56, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 56)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(56)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_L_Up_Real2.wav", CHANNELID::SOUND_PLAYER_PARRY_1, 75.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(56)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    }

    /* 여기 애니메이션 안끝났을 때 다른 동작시 문제 생김 */

}
void CBody_Player::STATE_PARRY_DEFLECT_L_Method()
{

    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);   
    m_pModelCom->Set_LerpFinished(true);    

    m_pModelCom->SetUp_Animation(54, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 54)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(54)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_L_Real.wav", CHANNELID::SOUND_PLAYER_PARRY_1, 75.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(54)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_UP_Method()
{

    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(59, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 59)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(59)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_R_UP.wav", CHANNELID::SOUND_PLAYER_PARRY_2, 75.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(59)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);  

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_Method()
{

    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);   
    m_pModelCom->Set_LerpFinished(true);    

    m_pModelCom->SetUp_Animation(58, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 58)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(58)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Parry_Deflect_R.wav", CHANNELID::SOUND_PLAYER_PARRY_2, 75.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(58)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);  

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_HurtMFR_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(31, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    if (m_pModelCom->Get_VecAnimation().at(31)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_HurtMFR_R_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(32, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(32)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_HURT_LF_Method()
{

    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(30, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(30)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_SF_Method()
{
    m_pModelCom->SetUp_Animation(33, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(33)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_SL_Method()
{
    m_pModelCom->SetUp_Animation(34, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(34)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}


void CBody_Player::STATE_HURT_HURXXLF_Method()
{
    m_pModelCom->SetUp_Animation(37, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(37)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_KNOCKBACK_Method()
{
    m_pModelCom->SetUp_Animation(38, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(38)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_KNOCKDOWN_Method()
{
    m_pModelCom->SetUp_Animation(29, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(29)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_WEAK_GETUP_F;

    }
}

void CBody_Player::STATE_HURT_FALLDOWN_Method()
{
    m_pModelCom->SetUp_Animation(27, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(27)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_HURT_FALLDOWN_END;
    }
}

void CBody_Player::STATE_HURT_FALLDOWN_END_Method()
{
    m_pModelCom->SetUp_Animation(28, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(28)->isAniMationFinish())
    {

        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_WEAK_GETUP_F_Method()
{
    m_pModelCom->SetUp_Animation(282, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(282)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}

void CBody_Player::STATE_REBOUND_R_Method()
{

    m_pModelCom->SetUp_Animation(62, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(62)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_STUNNED_START_Method()
{
    m_pModelCom->SetUp_Animation(25, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(25)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_STUNNED_LOOP;
    }
}

void CBody_Player::STATE_STUNNED_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(24, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(24)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}



void CBody_Player::STATE_NORMAL_EVADE_R_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_L_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }
    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_FR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_FL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_F_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_B_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 257)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(257)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Evade.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_HARMOR_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        //m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);    

        *m_pParentState = CPlayer::STATE_HARMOR_EXECUTION;
    }
}

void CBody_Player::STATE_HARMOR_EXECUTION_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);


    m_pModelCom->SetUp_Animation(222, false);
    m_iRenderState = STATE_NORMAL_RENDER;

#pragma region Effect_HArmor_Execution
    if (m_pModelCom->Get_Current_Animation_Index() == 222)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(222)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    if (!strcmp(iter.szName, "Effect_Start1"))
                    {
                        const _float4x4* matWeapon_r = m_pModelCom->Get_BoneMatrix("weapon_r");
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_HARMOR_EXECUTION_BLOOD_1, m_pParentWorldMatrix, matWeapon_r);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                    }
                    else if (!strcmp(iter.szName, "Effect_Start2"))
                    {
                        _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                        _vector vDir = { m_pParentWorldMatrix->_31 * -1.f, m_pParentWorldMatrix->_32 * -1.f, m_pParentWorldMatrix->_33 * -1.f, 0.f };
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                        const _float4x4* matWeapon_r = m_pModelCom->Get_BoneMatrix("weapon_r");
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_HARMOR_EXECUTION_BLOOD_2, m_pParentWorldMatrix, matWeapon_r);
                        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_HARMOR_EXECUTION_SPARK, vPos, vDir);
                    }
                }
            }
        }
    }
#pragma endregion

    if (m_pModelCom->Get_VecAnimation().at(222)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;

        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));

        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseMp_Amount = 100;
            _int IncreaseHp_Amount = 100;
            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 100); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 100); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}

void CBody_Player::STATE_STUN_EXECUTE_Method()
{
    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f)
    {
        switch (*m_pParentMonsterExecute)
        {
        case MONSTER_EXECUTION_CATEGORY::MONSTER_HARMOR:
            *m_pParentState = CPlayer::STATE_HARMOR_EXECUTION;
            break;
        case MONSTER_EXECUTION_CATEGORY::MONSTER_JOKER:
            *m_pParentState = CPlayer::STATE_Joker_Execution;
            break;
        case MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1:
            *m_pParentState = CPlayer::STATE_LV1Villager_M_Execution;
            m_pCamera->Set_Execute_CamereScene(MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1);
            m_pCamera->Set_Camera_Cut_Scene_OnOff(true);
            break;
        default:
            break;
        }
        // 여기를 이제 부모에서 받아온 몬스터 처형 스테이트로 switch case 문으로 하면 될듯 
    }

    /*if (m_pModelCom->Get_VecAnimation().at(291)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
    }*/
}

void CBody_Player::STATE_STUN_EXECUTE_START_VILLAGEM1_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);

        *m_pParentState = CPlayer::STATE_LV1Villager_M_Execution;
        m_pCamera->Set_Execute_CamereScene(MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1);
        m_pCamera->Set_Camera_Cut_Scene_OnOff(true);
    }
}



void CBody_Player::STATE_LV1Villager_M_Execution_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(52, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(52)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        m_pCamera->Set_Execute_CamereScene(MONSTER_EXECUTION_CATEGORY::MONSTER_START);
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;



        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 200;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                    
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 200); // HP        
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 200); // HP     
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때                  
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}


void CBody_Player::STATE_STUN_EXECUTE_START_JOKER_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        //m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);    

        *m_pParentState = CPlayer::STATE_Joker_Execution;
    }

}

void CBody_Player::STATE_Joker_Execution_Method()
{
    m_pModelCom->SetUp_Animation(51, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(51)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;
    }
}

void CBody_Player::STATE_Varg_Execution_Method()
{
    m_pModelCom->SetUp_Animation(50, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterFinalEvent(true);

    if (m_pModelCom->Get_VecAnimation().at(50)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterFinalEvent(false);
        *m_pParentNextStateCan = true;



        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 500;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_URD_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        m_pModelCom->Get_VecAnimation().at(232)->Set_StartOffSetTrackPosition(45.f);
        *m_pParentState = CPlayer::STATE_URD_EXECUTION;
    }
}

void CBody_Player::STATE_URD_EXECUTION_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(232, false);
    m_iRenderState = STATE_NORMAL_RENDER;


#pragma region Effect_Urd_Execution  
    if (m_pModelCom->Get_Current_Animation_Index() == 232)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(232)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분     
                {
                    if (!strcmp(iter.szName, "Effect_Spark"))
                    {
                        const _float4x4* matWeapon = m_pModelCom->Get_BoneMatrix("weapon_l");
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_URD_EXECUTION_SPARK, m_pParentWorldMatrix, matWeapon);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                    }
                }
            }
        }
    }
#pragma endregion
    // 박쥐, 변이된 오두르 칼 

    if (m_pModelCom->Get_VecAnimation().at(232)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;

        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));    

        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 1000;
            _int IncreaseMp_Amount = 150;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 1000); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 1000); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 150); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 150); // MP    
            }


        }
    }

}

void CBody_Player::STATE_STUN_EXECUTE_START_BAT_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        //m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);    

        *m_pParentState = CPlayer::STATE_BAT_EXECUTION;
    }
}

void CBody_Player::STATE_BAT_EXECUTION_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(219, false);
    m_iRenderState = STATE_NORMAL_RENDER;


#pragma region Effect_Bat_Execution

    if (m_pModelCom->Get_Current_Animation_Index() == 219)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(219)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    if (!strcmp(iter.szName, "Effect_Start1"))
                    {
                        const _float4x4* matWeapon_r = m_pModelCom->Get_BoneMatrix("weapon_r");
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_BAT_EXECUTION_BLOOD_3, m_pParentWorldMatrix, matWeapon_r);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                    }
                }
            }
        }
    }
#pragma endregion

    if (m_pModelCom->Get_VecAnimation().at(219)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;


        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 1000;
            _int IncreaseMp_Amount = 150;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                   
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 1000); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 1000); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 150); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 150); // MP    
            }


        }
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_RESEARCHER_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        //m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);    

        *m_pParentState = CPlayer::STATE_RESEARCHER_EXECUTION;
    }

}

void CBody_Player::STATE_RESEARCHER_EXECUTION_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(216, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(216)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;


        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 300;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 300); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 300); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}

void CBody_Player::STATE_MAGICIAN_Execution_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(225, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(225)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;

        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));

        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 500;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Hp가 이미 더 클 때                
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 100); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 100); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_MAGICIAN_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);


        _float4x4 newPosition = {};
        _matrix fixedPosition = {};
        fixedPosition.r[0] = { 0.00249777804f, 0.00000000f, -0.000105359715f, 0.00000000f };
        fixedPosition.r[1] = { 0.0f, 0.00249999994f, 0.0f, 0.0f };
        fixedPosition.r[2] = { 0.000105359715f, 0.0f, 0.00249777804f, 0.0f };
        fixedPosition.r[3] = { -42.f, m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1], -100.46f, 1.0f };
        // 여기서 이제 그러면 플레이어 위치 변경해주기 

        XMStoreFloat4x4(&newPosition, fixedPosition);

        m_pParent->Get_Transfrom()->Set_WorldMatrix(newPosition);


        *m_pParentState = CPlayer::STATE_MAGICIAN_Execution;
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_MUTATION_MAGICIAN_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);


        _float4x4 newPosition = {};
        _matrix fixedPosition = {};
        fixedPosition.r[0] = { 0.00249777804f, 0.00000000f, -0.000105359715f, 0.00000000f };
        fixedPosition.r[1] = { 0.0f, 0.00249999994f, 0.0f, 0.0f };
        fixedPosition.r[2] = { 0.000105359715f, 0.0f, 0.00249777804f, 0.0f };
        fixedPosition.r[3] = { -42.f, m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION).m128_f32[1], -100.46f, 1.0f };
        // 여기서 이제 그러면 플레이어 위치 변경해주기 

        XMStoreFloat4x4(&newPosition, fixedPosition);

        m_pParent->Get_Transfrom()->Set_WorldMatrix(newPosition);

        *m_pParentState = CPlayer::STATE_MAGICIAN_MUTATION_Execution;
    }
}

void CBody_Player::STATE_MAGICIAN_MUTATION_Execution_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(211, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(211)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;

        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 500;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                  
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }
}

void CBody_Player::STATE_STUN_EXECUTE_START_GRACE_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        //m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);    

        *m_pParentState = CPlayer::STATE_GRACE_Execution;
    }

}

void CBody_Player::STATE_GRACE_Execution_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(221, false);
    m_iRenderState = STATE_NORMAL_RENDER;

#pragma region Effect

    if (m_pModelCom->Get_Current_Animation_Index() == 221)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(221)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    if (!strcmp(iter.szName, "Effect_Grace_Execution_Kick"))
                    {
                        _float4x4 matLeftFoot = {};
                        XMStoreFloat4x4(&matLeftFoot, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pModelCom->Get_BoneMatrix("Bip001-L-Foot")) * XMLoadFloat4x4(m_pParentWorldMatrix));
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_GRACE_EXECUTION_KICK, matLeftFoot);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                    }
                }
            }
        }
    }
#pragma endregion

    if (m_pModelCom->Get_VecAnimation().at(221)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;


        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 500;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                  
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 500); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }


        }
    }

}

void CBody_Player::STATE_STUN_EXECUTE_START_PUNCHMAN_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }



    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        *m_pParentState = CPlayer::STATE_PUNCH_MAN_Execution;
    }
}

void CBody_Player::STATE_PUNCH_MAN_Execution_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f); //이종한 0412 추가 <- 아마 Lerp중이여서 안되는문제 해결용
    m_pModelCom->Set_LerpFinished(true); //이종한 0412 추가

    m_pModelCom->SetUp_Animation(224, false);
    m_iRenderState = STATE_NORMAL_RENDER;

#pragma region Effect_Punch_Execution
    if (m_pModelCom->Get_Current_Animation_Index() == 224)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    if (!strcmp(iter.szName, "Effect_Start"))
                    {
                        const _float4x4* matWeapon_r = m_pModelCom->Get_BoneMatrix("weapon_r");
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_EXECUTION, m_pParentWorldMatrix, matWeapon_r);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                    }
                }
            }
        }
    }
#pragma endregion

    if (m_pModelCom->Get_VecAnimation().at(224)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;



        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 200;
            _int IncreaseMp_Amount = 100;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                 
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 200); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 200); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때                  
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 100); // MP    
            }
        }
    }
}

void CBody_Player::STATE_MAGICIAN_CATCH_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(227, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    if (m_pModelCom->Get_VecAnimation().at(227)->isAniMationFinish())
    {

        *m_pParentState = CPlayer::STATE_WEAK_GETUP_F;

        ///* 여기 수정 작업 ㄱㄱ 싱*/
        //*m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        //*m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        //*m_pParentNextStateCan = true;

        //*m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_SPRINT_Method()
{
    m_pModelCom->SetUp_Animation(252, true);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_Current_Animation_Index() == 252)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(252)->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "SPRINT_AFTER_EFFECT_RIGHT"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(RightFootPos, 1.f);

                                //iter.isPlay = true;     
                            }

                            if (!strcmp(iter.szName, "SPRINT_AFTER_EFFECT_LEFT"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Add_RippleInfo(LeftFootPos, 1.f);

                                //iter.isPlay = true; 
                            }
                        }
                        else
                        {
                            if (!strcmp(iter.szName, "SPRINT_AFTER_EFFECT_RIGHT"))
                            {
                                _matrix RightFootMatrix = XMLoadFloat4x4(m_mRightFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 RightFootPos = { RightFootMatrix.r[3].m128_f32[0],  RightFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_R, m_pParentWorldMatrix, m_mRightFootBoneMartix);

                            }

                            else if (!strcmp(iter.szName, "SPRINT_AFTER_EFFECT_LEFT"))
                            {
                                _matrix LeftFootMatrix = XMLoadFloat4x4(m_mLeftFootBoneMartix) * XMLoadFloat4x4(m_pParentWorldMatrix);

                                _float2 LeftFootPos = { LeftFootMatrix.r[3].m128_f32[0],  LeftFootMatrix.r[3].m128_f32[2] };

                                m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_FOOT_SMOKE_L, m_pParentWorldMatrix, m_mLeftFootBoneMartix);
                            }

                        }

                    }
                }
                break;

                case EVENT_SOUND:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_pGameInstance->IsInWater())
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_1.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.1f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_3.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 0.1f);
                                    iter.isPlay = true;
                                    m_iRightFootStepSound = 0;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_2.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.1f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound++;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_Fantasy_Game_Footsteps_Water_4.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 0.1f);
                                    iter.isPlay = true;
                                    m_iLeftFootStepSound = 0;
                                    break;
                                }
                                //iter.isPlay = true; 
                            }
                        }

                        /* Water가 아닐 때 */
                        else
                        {
                            if (!strcmp(iter.szName, "Right_Foot_Sound"))
                            {
                                switch (m_iRightFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                                    m_iRightFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                                    m_iRightFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }

                            }

                            if (!strcmp(iter.szName, "Left_Foot_Sound"))
                            {
                                switch (m_iLeftFootStepSound)
                                {
                                case 0:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 1.f);
                                    m_iLeftFootStepSound++;
                                    iter.isPlay = true;
                                    break;
                                case 1:
                                    m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_2, 1.f);
                                    m_iLeftFootStepSound = 0;
                                    iter.isPlay = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                break;
                }
            }
        }
    }

    /* 3월 8일 추가 */
    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
}


void CBody_Player::STATE_CANE_SWORD_SP02_Method()
{
    m_pModelCom->SetUp_Animation(88, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    //if (m_pModelCom->Get_Current_Animation_Index() == 88)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 88)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(88)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Cane_Sword.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(88)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;
    }
}

void CBody_Player::STATE_GREATSWORD_Method()
{
    m_pModelCom->SetUp_Animation(103, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    //if (m_pModelCom->Get_Current_Animation_Index() == 103)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 103)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(103)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    if (!strcmp(iter.szName, "Attack_Sound_1"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Gread_Sword_1.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                        iter.isPlay = true;
                    }

                    else if (!strcmp(iter.szName, "Attack_Sound_2"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Gread_Sword_2.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                        iter.isPlay = true;
                    }
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(103)->isAniMationFinish())
    {

        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_GREATSWORD
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_GREATSWORD
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }
        }
    }
#pragma endregion 

}

void CBody_Player::STATE_JAVELIN_SWORD_Method()
{
    m_pModelCom->SetUp_Animation(135, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    //if (m_pModelCom->Get_Current_Animation_Index() == 135)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 135)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(135)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Javelin_Throw.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(135)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;    
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_JAVELIN_SWORD
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_JAVELIN_SWORD
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }
        }
    }
#pragma endregion 
}
//void CBody_Player::STATE_VARG_STUN_EXECUTE_START_R_Method()
//{
//    m_pModelCom->SetUp_Animation(292, false);
//    m_iRenderState = STATE_NORMAL_RENDER;
//
//    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 68.f)
//    {
//        m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(68.f);
//        *m_pParentState = CPlayer::STATE_VARG_RUN_EXECUTION;
//    }
//}


void CBody_Player::STATE_STUN_EXECUTE_START_VARG_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 291)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(291)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Stun_Start.wav", CHANNELID::SOUND_PLAYER_ACTION_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 25.f)
    {
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(true);
        m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(45.f);

        *m_pParentState = CPlayer::STATE_VARG_RUN_EXECUTION;
    }
}

void CBody_Player::STATE_HURT_MUTATION_MAGICIAN_CATCH_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);


    m_pModelCom->SetUp_Animation(212, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    if (m_pModelCom->Get_VecAnimation().at(212)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);

        *m_pParentState = CPlayer::STATE::STATE_GET_UP;
        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));
    }
}

void CBody_Player::STATE_HURT_RESEARCHER_CATCHED_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);


    m_pModelCom->SetUp_Animation(218, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    if (m_pModelCom->Get_VecAnimation().at(218)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);
        //m_pModelCom->Set_LerpFinished(false);      

        *m_pParentState = CPlayer::STATE::STATE_GET_UP;
        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));
    }
}



void CBody_Player::STATE_VARG_RUN_EXECUTION_Method()
{
    m_pModelCom->SetUp_Animation(298, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(298)->isAniMationFinish())
    {
        m_pModelCom->Get_VecAnimation().at(50)->Set_StartOffSetTrackPosition(15.f);
        dynamic_cast<CPlayer*>(m_pParent)->Set_MonsterEvent(false);
        *m_pParentState = CPlayer::STATE_Varg_Execution;
    }
}

void CBody_Player::STATE_LIGHT_EXECUTION_R_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);


    m_pModelCom->SetUp_Animation(210, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(210)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;

    }
}

void CBody_Player::STATE_LIGHT_EXECUTION_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);


    m_pModelCom->SetUp_Animation(209, false);

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 209)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(209)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Light_Execute_L.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 20.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(209)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
        *m_pParentNextStateCan = true;



        if (*m_pParentTalent & CPlayer::TALENT_EXECUTION_HP_MP)
        {
            _int IncreaseHp_Amount = 50;
            _int IncreaseMp_Amount = 50;

            _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();
            _int m_iFullHp = static_cast<CPlayer*>(m_pParent)->Get_FullHp();


            if ((*m_pParentHp + IncreaseHp_Amount) > m_iFullHp) // Mp가 이미 더 클 때                  
            {
                if (m_iFullHp > *m_pParentHp)
                    *m_pParentHp += m_iFullHp - *m_pParentHp;

                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 50); // HP    
            }
            else
            {
                *m_pParentHp += IncreaseHp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 12, 50); // HP    
            }

            if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때               
            {
                if (m_iFullMp > *m_pParentMp)
                    *m_pParentMp += m_iFullMp - *m_pParentMp;


                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 50); // MP        

            }
            else
            {
                *m_pParentMp += IncreaseMp_Amount;
                m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, 50); // MP    
            }


        }



    }
}

void CBody_Player::STATE_LADDER_CLIMB_START_Method()
{
    m_pModelCom->SetUp_Animation(47, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(47)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_UP;
        }

        else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_DOWN_END;
        }
    }
}

void CBody_Player::STATE_LADDER_CLIMB_L_DOWN_Method()
{
    m_pModelCom->SetUp_Animation(39, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 39)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(39)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    switch (m_iLadderStepSound)
                    {
                    case 0:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 1:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 2:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 3:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 4:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 5:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound = 0;
                        break;

                    }

                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(39)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_UP;
        }

        else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_DOWN;
        }
    }
}

void CBody_Player::STATE_LADDER_CLIMB_L_DOWN_END_Method()
{
    m_pModelCom->SetUp_Animation(40, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(40)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_LADDER;

        _vector PlayerPos = m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

        m_pParentNavigationCom->Find_Closest_Cell(m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));

        m_pParent->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, XMVectorSetY(PlayerPos, m_pParentNavigationCom->Compute_Height(PlayerPos)));
    }
}

void CBody_Player::STATE_LADDER_CLIMB_L_UP_Method()
{
    m_pModelCom->SetUp_Animation(41, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 41)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(41)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    switch (m_iLadderStepSound)
                    {
                    case 0:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 1:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 2:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 3:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 4:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 5:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound = 0;
                        break;

                    }

                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(41)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_UP;
        }

        else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_DOWN;
        }
    }
}

void CBody_Player::STATE_LADDER_CLIMB_L_UP_END_Method()
{
    m_pModelCom->SetUp_Animation(42, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(42)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_LADDER;

        _vector PlayerPos = m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

        m_pParentNavigationCom->Find_Closest_Cell(m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));

        m_pParent->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, XMVectorSetY(PlayerPos, m_pParentNavigationCom->Compute_Height(PlayerPos)));
    }
}

void CBody_Player::STATE_LADDER_CLIMB_L_IDEL_Method()
{
    m_pModelCom->SetUp_Animation(48, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}

void CBody_Player::STATE_LADDER_CLIMB_R_DOWN_Method()
{
    m_pModelCom->SetUp_Animation(43, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 43)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(43)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    switch (m_iLadderStepSound)
                    {
                    case 0:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 1:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 2:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 3:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 4:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 5:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound = 0;
                        break;

                    }

                    break;
                }
            }
        }
    }



    if (m_pModelCom->Get_VecAnimation().at(43)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_UP;
        }

        else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_DOWN;
        }
    }
}

void CBody_Player::STATE_LADDER_CLIMB_R_DOWN_END_Method()
{
    m_pModelCom->SetUp_Animation(44, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(44)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_LADDER;

        _vector PlayerPos = m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

        m_pParentNavigationCom->Find_Closest_Cell(m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));

        m_pParent->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, XMVectorSetY(PlayerPos, m_pParentNavigationCom->Compute_Height(PlayerPos)));
    }

}

void CBody_Player::STATE_LADDER_CLIMB_R_UP_Method()
{
    m_pModelCom->SetUp_Animation(45, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 45)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(45)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    switch (m_iLadderStepSound)
                    {
                    case 0:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 1:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 2:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_03.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 3:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_04.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 4:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_05.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound++;
                        break;
                    case 5:
                        m_pGameInstance->Play_Sound(L"Player_FootStep_WoodStairA_06.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 3.f);
                        iter.isPlay = true;
                        m_iLadderStepSound = 0;
                        break;

                    }

                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(45)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_UP;
        }

        else if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_DOWN;
        }
    }

}

void CBody_Player::STATE_LADDER_CLIMB_R_UP_END_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(46, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(46)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_LADDER;

        _vector PlayerPos = m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

        m_pParentNavigationCom->Find_Closest_Cell(m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));

        m_pParent->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, XMVectorSetY(PlayerPos, m_pParentNavigationCom->Compute_Height(PlayerPos)));
    }
}

void CBody_Player::STATE_LADDER_CLIMB_R_UP_REVERSE_END_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(46, false, true);  // 역재생 하므로 true를 줌  
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(46)->isAniMationFinish())
    {
        if (m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_L_UP_END;
        }

        if (m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
        {
            *m_pParentState = CPlayer::STATE_LADDER_CLIMB_R_DOWN;
        }
    }
}

void CBody_Player::STATE_LADDER_CLIMB_R_IDEL_Method()
{
    m_pModelCom->SetUp_Animation(49, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}

void CBody_Player::STATE_MAGICIAN_LV1_SEQ_BOSS_FIGHT_START_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(296, true);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(296)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_IDLE;
        *m_pParentNextStateCan = true;
    }
}

void CBody_Player::STATE_ARCHIVE_SIT_START_Method()
{
    m_pModelCom->SetUp_Animation(66, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(66)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_ARCHIVE_SIT_LOOP;
    }

}

void CBody_Player::STATE_ARCHIVE_SIT_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(65, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    m_pModelCom->Get_VecAnimation().at(65)->Set_AnimationSpeed(0.f);

}

void CBody_Player::STATE_ARCHIVE_SIT_GETUP_Method()
{
    m_pModelCom->SetUp_Animation(63, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(63)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_INTERACTION;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
    }
}

void CBody_Player::STATE_ARCHIVE_SIT_LIGHT_UP_Method()
{
    /* 의자 불켜기 관련 */
    m_pModelCom->SetUp_Animation(64, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(64)->isAniMationFinish() && m_pModelCom->Get_LerpFinished()) 
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_INTERACTION;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
    }
}

void CBody_Player::STATE_HEAL_Method()
{
    m_pModelCom->SetUp_Animation(237, false);
    m_iRenderState = STATE_NORMAL_RENDER;

#pragma region Effect
    if (m_pModelCom->Get_Current_Animation_Index() == 237)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_EFFECT:
                    if (iter.isEventActivate == true && iter.isPlay == false)
                    {
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로         

                        if (!strcmp(iter.szName, "Effect_Start"))
                        {
                            _float4x4 matLeftHand = {};
                            XMStoreFloat4x4(&matLeftHand, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pModelCom->Get_BoneMatrix("weapon_l")) * XMLoadFloat4x4(m_pParentWorldMatrix));
                            for (_uint i = 0; i < 2; i++)
                            {
                                m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PLAYER_HEAL, matLeftHand);
                            }
                            _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42 , m_pParentWorldMatrix->_43 , 1.f };
                            m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_HEAL, vPos);
                        }
                    }
                    break;
                case EVENT_SOUND:
                    if (iter.isEventActivate == true && iter.isPlay == false)
                    {
                        m_pGameInstance->Play_Sound(L"Player_Potion.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                        iter.isPlay = true;
                    }
                    break;

                }

            }
        }
    }
#pragma endregion

    if (m_pModelCom->Get_VecAnimation().at(237)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_HEAL;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
    }
}

void CBody_Player::STATE_DEAD_Method()
{
    m_pModelCom->SetUp_Animation(266, false);
    m_iRenderState = STATE_DEAD_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(266)->isAniMationFinish())
    {
    }
}

void CBody_Player::STATE_START_WALK_Method()
{
    //m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);   
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(14, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 14)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(14)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    if (!strcmp(iter.szName, "Right_Foot_Sound"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_01.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                        iter.isPlay = true;
                    }

                    else if (!strcmp(iter.szName, "Left_Foot_Sound"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_FootStep_Leather_02.ogg", CHANNELID::SOUND_PLAYER_ACTION_1, 1.f);
                        iter.isPlay = true;
                    }

                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(14)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_START;
    }
}

void CBody_Player::STATE_CLAW_CHARGE_START_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.1f);

    m_pModelCom->SetUp_Animation(144, false);
    m_iRenderState = STATE_CLAW_RENDER;


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 144)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(144)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Charge_Start_Real.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    //m_pModelCom->Get_VecAnimation().at(2)->SetLerpTime(0.15f);

    if (m_pModelCom->Get_VecAnimation().at(144)->isAniMationFinish() && m_pModelCom->Get_LerpFinished())
    {

        *m_pParentState = CPlayer::STATE::STATE_CLAW_CHARGE_LOOP;

    }
}

void CBody_Player::STATE_CLAW_CHARGE_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(142, true);
    m_iRenderState = STATE_CLAW_RENDER;

    //if(m_pGameInstance->isMouseRelease(DIM_RB))  
    //{   
    //    *m_pParentState = CPlayer::STATE_CLAW_CHARGE_FULL_ATTACK;   
    //
    //    //static_cast<CPlayer*>(m_pParent)->Reset_ChargeTime();   
    //	//m_fChrageTime = 0.f;    
    //}
}

void CBody_Player::STATE_CLAW_CHARGE_FULL_ATTACK_Method()
{
    m_pModelCom->SetUp_Animation(145, false);
    m_iRenderState = STATE_CLAW_RENDER;

    m_pModelCom->Get_VecAnimation().at(145)->Set_StartOffSetTrackPosition(36.f);


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 145)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(145)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_FullCharge_Attack.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 3.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(145)->isAniMationFinish() && m_pModelCom->Get_Current_Animation_Index() == 145)
    {
        m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.2f);
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        //static_cast<CPlayer*>(m_pParent)->Reset_ChargeTime();

    }
}

void CBody_Player::STATE_CLAW_LONG_PLUNDER_ATTACK2_Method()
{

    m_pModelCom->SetUp_Animation(147, false);
    m_iRenderState = STATE_CLAW_RENDER;

    m_pModelCom->Get_VecAnimation().at(147)->Set_StartOffSetTrackPosition(3.f);

    if (m_pModelCom->Get_Current_Animation_Index() == 147)
    {
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    }

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 147)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(147)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Flunder_Attack.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 40.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }



    if (m_pModelCom->Get_VecAnimation().at(147)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
    }
}

void CBody_Player::STATE_HALBERDS_B_Method()
{
    m_pModelCom->SetUp_Animation(107, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    //if (m_pModelCom->Get_Current_Animation_Index() == 107)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}

    if (m_pModelCom->Get_Current_Animation_Index() == 107)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(107)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    if (!strcmp(iter.szName, "Attack_Halberd_Sound_1"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Halberd_Attack_Sound_1.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    else if (!strcmp(iter.szName, "Attack_Halberd_Sound_2"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Halberd_Attack_Sound_2.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    else if (!strcmp(iter.szName, "Attack_Halberd_Sound_3"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Halberd_Attack_Sound_3.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(107)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_HALBERDS_B
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_HALBERDS_B
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }
        }
    }
#pragma endregion 
}

void CBody_Player::STATE_SCYTHE_B_Method()
{
    m_pModelCom->SetUp_Animation(122, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    //if (m_pModelCom->Get_Current_Animation_Index() == 122)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}

    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 122)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(122)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    if (!strcmp(iter.szName, "Attack_Sound_1"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Scythe_Attack_1.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    else if (!strcmp(iter.szName, "Attack_Sound_2"))
                    {
                        m_pGameInstance->Play_Sound(L"Player_Scythe_Attack_2.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                        iter.isPlay = true;
                    }

                    break;
                }
            }
        }
    }

    if (m_pModelCom->Get_VecAnimation().at(122)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;
    }
#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_SCYTHE_B
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_SCYTHE_B
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }
        }
    }
#pragma endregion 

}

void CBody_Player::STATE_AXE_Method()
{
    m_pModelCom->SetUp_Animation(69, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    //if (m_pModelCom->Get_Current_Animation_Index() == 69)
    //{
    //    /* 스킬을 사용 했음을 UI에게 알리는 코드 */
    //    *m_pParentSkillCoolTime = false;
    //}


    /* 플레이어 사운드 관련 */
    if (m_pModelCom->Get_Current_Animation_Index() == 69)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(69)->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.isEventActivate == true)
            {
                switch (iter.eType)
                {
                case EVENT_SOUND:
                    m_pGameInstance->Play_Sound(L"Player_Axe.wav", CHANNELID::SOUND_PLAYER_ATTACK_1, 4.f);
                    iter.isPlay = true;
                    break;
                }
            }
        }
    }


    if (m_pModelCom->Get_VecAnimation().at(69)->isAniMationFinish())
    {
        *m_pParentNextStateCan = true;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
        *m_pParentSkillCoolTime = false;

    }
#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_AXE
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE);
                *m_pParentSkillCoolTime = false;
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_AXE
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 25.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
                *m_pParentSkillCoolTime = false;
            }
        }
    }
#pragma endregion 
}

void CBody_Player::STATE_CATCHED_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);
    m_pModelCom->Set_LerpFinished(true);

    m_pModelCom->SetUp_Animation(233, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(233)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE::STATE_GET_UP;
        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));
    }
}

void CBody_Player::STATE_GET_UP_Method()
{
    m_pModelCom->SetUp_Animation(273, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(273)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = CPlayer::STATE::STATE_IDLE;

    }
}





HRESULT CBody_Player::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corner"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Dissolve_Noise*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pDissolveNoiseTextureCom))))
        return E_FAIL;


    /* ORM 텍스처 관련 */

    /* ORM 머리 */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Head"),
        TEXT("Com_ORM_Head_Texture"), reinterpret_cast<CComponent**>(&m_pORM_HeadTexture))))
        return E_FAIL;

    /* ORM 몸통 */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Body"),
        TEXT("Com_ORM_Body_Texture"), reinterpret_cast<CComponent**>(&m_pORM_BodyTexture))))
        return E_FAIL;

    /* ORM 다리 */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Leg"),
        TEXT("Com_ORM_Leg_Texture"), reinterpret_cast<CComponent**>(&m_pORM_LegTexture))))
        return E_FAIL;

    /* ORM 망토 */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Clock"),
        TEXT("Com_ORM_Clock_Texture"), reinterpret_cast<CComponent**>(&m_pORM_ClockTexture))))
        return E_FAIL;



    return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);

    /*ORM Texture 삭제 */
    Safe_Release(m_pORM_HeadTexture);
    Safe_Release(m_pORM_BodyTexture);
    Safe_Release(m_pORM_LegTexture);
    Safe_Release(m_pORM_ClockTexture);

    Safe_Release(m_pDissolveNoiseTextureCom);



}
