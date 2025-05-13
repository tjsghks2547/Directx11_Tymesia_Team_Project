#include "pch.h" 
#include "RightWeapon.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Camera_Free.h"

CRightWeapon::CRightWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CRightWeapon::CRightWeapon(const CRightWeapon& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CRightWeapon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CRightWeapon::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentPhaseState = pDesc->pParentPhaseState;
    m_pParentHp = pDesc->pParentHp;
    m_pParentMp = pDesc->pParentMp;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.06f,0.6f,0.f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 2.f,0.f,0.f,1.f });

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

    return S_OK;

}

void CRightWeapon::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CRightWeapon::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    //for (size_t i = 0; i < 3; i++)  
    //    SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);  

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    CPlayer::STATE curState = (CPlayer::STATE)*m_pParentState;

#pragma region 이벤트 관련 작업
    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (m_pSet_Right_Weapon_States->count(curState))
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
                            m_pGameInstance->Add_Actor_Scene(m_pActor);  // 4타 때가 문제.   

                        else
                            m_pGameInstance->Sub_Actor_Scene(m_pActor);
                    }

                    else if (iter.isEventActivate == false && m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                    {
                        if (*m_pParentState == CPlayer::STATE_ATTACK_L4)
                            m_bCollisionOn = true;

                        m_pGameInstance->Sub_Actor_Scene(m_pActor);
                        iter.isPlay = true;
                        m_fHitStopTime = 0.f;
                    }

                    break;
                }
                case EVENT_STATE:
                {
                    if (iter.isEventActivate == true)
                    {
                        if (!strcmp(iter.szName, "Zoom_Blur"))
                        {
                            m_fZoomBlurTime += fTimeDelta;
                            m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());
                            m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurTime * 0.15f);
                            m_pCamera->ShakeOn(400.f, 400.f, 3.f, 3.f);
                        }

                        if (!strcmp(iter.szName, "Camera_Zoom_In"))
                        {
                            m_pCamera->Set_Camera_ZoomInSpeed(10.f);
                            m_pCamera->ZoomIn();
                        }

                        if (!strcmp(iter.szName, "Camera_Zoom_Out"))
                        {
                            m_pCamera->Set_Camera_ZoomOutSpeed(5.f);
                            m_pCamera->ZoomOut();
                        }

                        if (!strcmp(iter.szName, "Camera_Zoom_Out_2"))
                        {
                            m_pCamera->Set_Camera_ZoomOutSpeed(5.f);
                            m_pCamera->ZoomOut();
                        }

                        if (!strcmp(iter.szName, "Camera_Shake"))
                        {
                            m_pCamera->ShakeOn(800.f, 800.f, 10.f, 10.f);
                        }


                        else if (iter.isEventActivate == false && m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                        {
                            if (!strcmp(iter.szName, "Zoom_Blur"))
                            {
                                if (m_fZoomBlurTime <= 0.f)
                                {
                                    m_fZoomBlurTime = 0.f;
                                    m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
                                }
                                else
                                {
                                    m_fZoomBlurTime -= fTimeDelta * 1.5f;
                                    m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurTime * 0.15f);
                                }
                            }


                        }


                    }
                    break;
                }
                case EVENT_EFFECT:
                {
                    if (iter.isEventActivate == true)
                    {
#pragma region Effect0412수정
                        if (!strcmp(iter.szName, "LAttack1_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD1, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
                        }
                        else if (!strcmp(iter.szName, "LAttack2_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD2, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "LAttack3_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD3, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "LAttack4_1_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD4_1, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "LAttack4_2_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD4_2, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "LAttack5_Start"))
                        {
                            m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SWORD5, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "LAttack5_Dust"))
                        {
                            _vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
                            _vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
                            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PLAYERATTACK_5_DUST_EXPLOSION, vPos, vDir);
                            iter.isPlay = true;
                        }
                        else if (!strcmp(iter.szName, "Effect_Punch_Execution"))
                        {
                            m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_PUNCH_EXECUTION, m_pParentWorldMatrix, m_pSocketMatrix);
                            iter.isPlay = true;
                        }
                    }
#pragma endregion
                    break;
                }
                }
            }
        }
    }

    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);

        if (!(m_pSet_Body_States->count(curState))
            && *m_pParentPhaseState != CPlayer::PHASE_EXECUTION
            && !(m_pSet_Axe_Weapon_States->count(curState))
            && !(m_pSet_Scythe_Weapon_States->count(curState))
            && !(m_pSet_Halberd_Weapon_States->count(curState))
            && !(m_pSet_GreadSword_Weapon_States->count(curState))
            && !(m_pSet_JavelinSword_Weapon_States->count(curState))
            && !(m_pSet_Claw_Weapon_States->count(curState))
            && !(m_pSet_Player_Camera_States->count(curState))
            && !(m_pSet_Cane_Weapon_States->count(curState))
            )
        {
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        }
    }
#pragma endregion  

    /*m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);   */

    if (m_iPreParentState != *m_pParentState)
    {
        m_fHitStopTime = 0.f;
        m_bCollisionOn = true;
    }


    if (m_bHitStopOnOff)
    {
        Hit_Slow();
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 50.f, 0.f,0.f,1.f });

}

void CRightWeapon::Late_Update(_float fTimeDelta)
{
    CPlayer::STATE curState = (CPlayer::STATE)*m_pParentState;

    if (!m_pSet_Claw_Weapon_States->count(curState)
        && !m_pSet_Halberd_Weapon_States->count(curState)
        && !m_pSet_Scythe_Weapon_States->count(curState)
        && !m_pSet_Axe_Weapon_States->count(curState)
        && !(*m_pParentPhaseState & CPlayer::PHASE_INTERACTION)
        && !(*m_pParentPhaseState & CPlayer::PHASE_DEAD)
        && !(*m_pParentPhaseState & CPlayer::PHASE_LADDER)
        && *m_pParentState != CPlayer::STATE_CANE_SWORD_SP02
        && *m_pParentState != CPlayer::STATE_GREATSWORD
        && *m_pParentState != CPlayer::STATE_JAVELIN_SWORD
        && *m_pParentState != CPlayer::STATE_LOBBY_IDLE_01
        && *m_pParentState != CPlayer::STATE_LOBBY_IDLE_01_END)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }

    m_iPreParentState = *m_pParentState;
}

HRESULT CRightWeapon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(17);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CRightWeapon::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CRightWeapon::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CRightWeapon::Hit_Slow()
{
    if (m_fHitStopTime < 0.125f)
    {
        //m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
        m_pCamera->ShakeOn(400.f, 400.f, 4.f, 4.f);

        m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());

        switch (*m_pParentState)
        {
        case CPlayer::STATE_ATTACK_L1:
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.0f);
            break;
        case CPlayer::STATE_ATTACK_L2:
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.0f);
            break;
        case CPlayer::STATE_ATTACK_L3:
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.9f);
            break;
        case CPlayer::STATE_ATTACK_L4:
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta * 1.5f);   
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.1f);
            break;
        case CPlayer::STATE_ATTACK_L5:
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.2f);
            break;
        case CPlayer::STATE_SPRINT_ATTACK_L1:
            m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.0f);
            break;
        }
    }
    else
    {
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        m_bHitStopOnOff = false;

        m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
        m_fHitStopTime = 0.f;

    }
    m_fHitStopTime += m_fTimeDelta;//1.f / 80.f; //         


    return S_OK;
}

void CRightWeapon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    m_fHitStopTime = 0.f;
    m_bHitStopOnOff = true;
    m_bCollisionOn = false;

#pragma region Effect0319
    if (!strcmp("MONSTER", _pOther->Get_Name()))
    {
        _vector vPlayerLook = { (*m_pParentWorldMatrix)._31, (*m_pParentWorldMatrix)._32, (*m_pParentWorldMatrix)._33, 0.f };
        _vector vPosition = { m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f };
        if (*m_pParentState == CPlayer::STATE_ATTACK_L1)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_1_HOLDING, vPosition, vPlayerLook);
        if (*m_pParentState == CPlayer::STATE_ATTACK_L2)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_2_HOLDING, vPosition, vPlayerLook);
        if (*m_pParentState == CPlayer::STATE_ATTACK_L3)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_3_HOLDING, vPosition, vPlayerLook);
        if (*m_pParentState == CPlayer::STATE_ATTACK_L4)
        {
            if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= 24.f)
                m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_2_HOLDING, vPosition, vPlayerLook);
            else
                m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_1_HOLDING, vPosition, vPlayerLook);
        }
        if (*m_pParentState == CPlayer::STATE_ATTACK_L5)
            m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_BLOOD_5_HOLDING, vPosition, vPlayerLook);


        PxContactPairPoint contactPoints[1]; // 최대 10개까지 저장		
        _information.extractContacts(contactPoints, 1);
        PxVec3 position = contactPoints[0].position;
        PxVec3 dir = contactPoints[0].normal;
        _vector vHitPosition = { position.x, position.y, position.z, 1.f };
        _vector vHitDir = { dir.x, dir.y, dir.z, 1.f };
        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_EXPLOSION, vHitPosition, vHitDir);
        m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_PLAYER_MANA, vHitPosition, vHitDir);

    }
#pragma endregion

#pragma region UI 연동 작업 


    _int IncreaseMp_Amount = *static_cast<CPlayer*>(m_pParent)->Get_Bonus_Sword_Attack_Mp() + 6;
    _int m_iFullMp = static_cast<CPlayer*>(m_pParent)->Get_FullMp();


    if ((*m_pParentMp + IncreaseMp_Amount) > m_iFullMp) // Mp가 이미 더 클 때        
    {
        if (m_iFullMp > *m_pParentMp)
            *m_pParentMp += m_iFullMp - *m_pParentMp;
    }

    else
    {
        m_pGameInstance->Find_TextBox_PlayerScreen(m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen"), 22, IncreaseMp_Amount);
        *m_pParentMp += IncreaseMp_Amount;
    }



#pragma endregion 
}

void CRightWeapon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CRightWeapon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CRightWeapon* CRightWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRightWeapon* pInstance = new CRightWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRightWeapon::Clone(void* pArg)
{
    CRightWeapon* pInstance = new CRightWeapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CRightWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRightWeapon::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
