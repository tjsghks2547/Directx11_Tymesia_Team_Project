#include "pch.h" 
#include "ClawWeapon.h"     
#include "GameInstance.h"   
#include "Animation.h"  
#include "Camera_Free.h"    

CClawWeapon::CClawWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CClawWeapon::CClawWeapon(const CClawWeapon& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CClawWeapon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CClawWeapon::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_PLAGUE_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentPhsaeState = pDesc->pParentPhaseState;
    m_pParentHp = pDesc->pParentHp;
    m_pParentMp = pDesc->pParentMp;
    m_pParentSkillBonusDamage = pDesc->pParentSkillBonusDamage;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 0.3f,0.3f,0.3f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 30.f,0.f,0.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::MONSTER | GROUP_TYPE::DESTRUCT;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER_WEAPON, settingColliderGroup);

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



    m_pfClawParentTime = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawTimePtr();
    m_pfClawFinishTime = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawFinishTimePtr();
    m_pfClawAppearTimer = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawAppearTimerPtr();
    m_pbClawDeadOn = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawDeadOnPtr();
    m_pbClawAppear = dynamic_cast<CPlayer*>(m_pParent)->Get_ClawAppearPtr();

    m_fSkillBaseDamage = 100.f;

    return S_OK;
}

void CClawWeapon::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_fSkill_AttackPower = m_fSkillBaseDamage + *m_pParentSkillBonusDamage;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CClawWeapon::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    CPlayer::STATE curState = (CPlayer::STATE)*m_pParentState;

#pragma region 이벤트 관련 작업

    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (m_pSet_Claw_Weapon_States->count(curState))
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                switch (iter.eType)
                {
                case EVENT_COLLIDER:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (m_bCollisionOn)
                            m_pGameInstance->Add_Actor_Scene(m_pActor);

                        else
                            m_pGameInstance->Sub_Actor_Scene(m_pActor);
                    }

                    else
                    {
                        if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                        {
                            m_pGameInstance->Sub_Actor_Scene(m_pActor);
                            iter.isPlay = true;
                        }
                    }
                }
                break;

                case EVENT_STATE:
                {
                    if (iter.isEventActivate == true)
                    {
                        //if (!strcmp(iter.szName, "Camera_Zoom_Out"))
                        //{
                        //    // 카메라 포인터 가져오고 싶다.
                        //    m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());
                        //    m_pGameInstance->Set_ZoomBlur_Option(true, 1.5f * m_fAccTimeDelta);

                        //    m_fAccTimeDelta += fTimeDelta;
                        //    m_pCamera->Set_Camera_ZoomOutSpeed(5.f);
                        //    m_pCamera->ZoomOut();
                        //}

                        //if (!strcmp(iter.szName, "Camera_Zoom_In"))
                        //{
                        //    m_pCamera->Set_Camera_ZoomInSpeed(1.5f);
                        //    m_pCamera->ZoomIn();
                        //}


                        if (!strcmp(iter.szName, "Dissolve_Weapon"))
                        {
                            *m_pbClawDeadOn = true;
                        }

                        if (!strcmp(iter.szName, "Reverse_Dissolve_Weapon"))
                        {
                            *m_pbClawAppear = true;
                        }

                    }


                    /*else
                    {
                        if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                        {
                            if (!strcmp(iter.szName, "Camera_Zoom_Out"))
                            {
                                m_pCamera->ResetZoomOutCameraPos(7.5f);

                                m_fAccTimeDelta -= fTimeDelta;

                                if (m_fAccTimeDelta <= 0.f)
                                {
                                    m_fAccTimeDelta = 0.f;
                                    m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
                                }
                                else
                                    m_pGameInstance->Set_ZoomBlur_Option(true, 0.3f * m_fAccTimeDelta);
                            }

                        }
                    }*/
                }
                break;
                }

#pragma region Effect 0316  
                if ((iter.eType == EVENT_EFFECT)
                    && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분      
                {
                    if (!strcmp(iter.szName, "Claw1_Start"))
                    {

                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_PLAYER_CLAW1, m_pParentWorldMatrix);
                        iter.isPlay = true;

                    }
                    else if (!strcmp(iter.szName, "Claw2_Start"))
                    {

                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_PLAYER_CLAW2, m_pParentWorldMatrix);
                        iter.isPlay = true;

                    }
                    else if (!strcmp(iter.szName, "Claw1_Effect"))
                    {
                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_CLAW_1, &m_CombinedWorldMatrix);
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_CLAW, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;

                    }
                    else if (!strcmp(iter.szName, "Claw2_Effect"))
                    {

                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_CLAW_2, &m_CombinedWorldMatrix);
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_CLAW, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Effect_Start_Fail"))
                    {

                        m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_PLAYER_CHARGECLAW, m_pParentWorldMatrix);
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Particle_Start_Fail"))
                    {

                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW, m_pParentWorldMatrix, m_pSocketMatrix);
                        m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Effect_Start_Success"))
                    {

                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT, m_pParentWorldMatrix, m_pSocketMatrix);
                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Particle_Start_Success"))
                    {
                        m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP);
                        //m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Particle2_Start_Success"))
                    {

                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL_COMPLETE, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Effect_Start_Ready"))
                    {

                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_READY, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                    else if (!strcmp(iter.szName, "Effect_Start_Loop"))
                    {

                        m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP, m_pParentWorldMatrix, m_pSocketMatrix);
                        iter.isPlay = true;      // 한 번만 재생 되어야 하므로
                    }
                }
            }
            else if ((iter.eType == EVENT_EFFECT) && iter.isEventActivate == false && iter.isPlay == true)
            {
                if (!strcmp(iter.szName, "Claw1_Effect"))
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_CLAW_1);
                    iter.isPlay = false;      // 약간 Logic을 뒤틀어서 꺼져야할때를 구분함
                }
                else if (!strcmp(iter.szName, "Claw2_Effect"))
                {
                    m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_CLAW_2);
                    iter.isPlay = false;
                }
            }
#pragma endregion
        }
    }

    else if (!(m_pSet_Body_States->count(curState))
        && *m_pParentPhsaeState != CPlayer::PHASE_EXECUTION
        && !(m_pSet_Axe_Weapon_States->count(curState))
        && !(m_pSet_Scythe_Weapon_States->count(curState))
        && !(m_pSet_Halberd_Weapon_States->count(curState))
        && !(m_pSet_GreadSword_Weapon_States->count(curState))
        && !(m_pSet_JavelinSword_Weapon_States->count(curState))
        && !(m_pSet_Cane_Weapon_States->count(curState))
        && !(m_pSet_Player_Camera_States->count(curState))
        && !(m_pSet_Right_Weapon_States->count(curState)))
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
        m_pCamera->ResetZoomOutCameraPos(3.f);
        m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
    }
#pragma endregion  


    if (m_iPreParentState != *m_pParentState)
    {
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
        m_fAccTimeDelta = 0.f;
        m_bCollisionOn = true;

        if (!m_pSet_Claw_Weapon_States->count(curState))
        {
            *m_pbClawAppear = false;
            *m_pbClawDeadOn = false;
            *m_pfClawFinishTime = 0.f;
            *m_pfClawAppearTimer = 0.f;
        }

        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }

    if (m_bHitStopOnOff)
    {
        Hit_Slow();
    }



    if (*m_pbClawDeadOn)
        *m_pfClawFinishTime += fTimeDelta * 0.75f;

    if (*m_pbClawAppear)
        *m_pfClawAppearTimer += fTimeDelta * 1.25f;


    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
    {
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 0.f, 0.f,0.f,1.f });
    }
}

void CClawWeapon::Late_Update(_float fTimeDelta)
{
    m_iPreParentState = *m_pParentState;
}

HRESULT CClawWeapon::Render()
{

    return S_OK;
}

HRESULT CClawWeapon::Ready_Components()
{

    return S_OK;
}

HRESULT CClawWeapon::Bind_ShaderResources()
{

    return S_OK;
}

HRESULT CClawWeapon::Hit_Slow()
{

    if (m_fHitStopTime < 0.2f)
    {
        m_pCamera->ShakeOn(500.f, 500.f, 4.f, 4.f);
    }

    else
    {
        m_bHitStopOnOff = false;
    }

    m_fHitStopTime += m_fTimeDelta;

    return S_OK;
}

void CClawWeapon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    m_fHitStopTime = 0.f;
    m_bHitStopOnOff = true;
    m_bCollisionOn = false;

#pragma region Effect
    if (!strcmp("MONSTER", _pOther->Get_Name()))
    {
        _vector vPlayerLook = { (*m_pParentWorldMatrix)._31, (*m_pParentWorldMatrix)._32, (*m_pParentWorldMatrix)._33, 0.f };
        _vector vPosition = { m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f };
        if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_1_HOLDING, vPosition, vPlayerLook);
        if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_2_HOLDING, vPosition, vPlayerLook);
    }
#pragma endregion

    if (*m_pParentState == CPlayer::STATE_CLAW_CHARGE_FULL_ATTACK)
    {
        *m_pParentState = CPlayer::STATE_CLAW_LONG_PLUNDER_ATTACK2;

        if (dynamic_cast<CMonster*>(_pOther)->Get_MonsterSkill() != 0)
            dynamic_cast<CPlayer*>(m_pParent)->Set_Player_Take_Away_Skill(dynamic_cast<CMonster*>(_pOther)->Get_MonsterSkill());
    }

    m_pGameInstance->Sub_Actor_Scene(m_pActor);


#pragma region 발톱 관련 UI

    _int IncreaseMp_Amount = *static_cast<CPlayer*>(m_pParent)->Get_Bonus_Claw_Attack_Mp() + 5;
    _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();

    if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때              
    {
        if (m_iFullMp > *m_pParentMp)
            *m_pParentMp += m_iFullMp - *m_pParentMp;
    }
    else
    {
        *m_pParentMp += IncreaseMp_Amount;
    }
#pragma endregion 
}

void CClawWeapon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CClawWeapon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CClawWeapon* CClawWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CClawWeapon* pInstance = new CClawWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CClawWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CClawWeapon::Clone(void* pArg)
{
    CClawWeapon* pInstance = new CClawWeapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CClawWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CClawWeapon::Free()
{
    __super::Free();
}
