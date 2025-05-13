#include "pch.h" 
#include "Weapon_Scythe.h"  
#include "GameInstance.h"   
#include "Animation.h"  
#include "Camera_Free.h"    

CWeapon_Scythe::CWeapon_Scythe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{

}

CWeapon_Scythe::CWeapon_Scythe(const CWeapon_Scythe& Prototype)
    :CPartObject(Prototype)
{

}

HRESULT CWeapon_Scythe::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CWeapon_Scythe::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_PLAGUE_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentPhaseState = pDesc->pParentPhaseState;
    m_pPreParentState = pDesc->pPreParentState;
    m_pParentSkillBonusDamage = pDesc->pParentSkillBonusDamage;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.2f,0.7f,0.f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

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

    m_fSkillBaseDamage = 200.f;

    return S_OK;

}

void CWeapon_Scythe::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_fTime += fTimeDelta;
    m_fSkill_AttackPower = m_fSkillBaseDamage + *m_pParentSkillBonusDamage;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CWeapon_Scythe::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    CPlayer::STATE curState = (CPlayer::STATE)*m_pParentState;

    if (m_pSet_Scythe_Weapon_States->count(curState))
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
                        m_pGameInstance->Add_Actor_Scene(m_pActor);
                    }

                    else
                    {
                        if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() > iter.fEndTime)
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
                        if (!strcmp(iter.szName, "Dissolve_Weapon"))
                        {
                            m_bDeadOn = true;
                        }

                        if (!strcmp(iter.szName, "Reverse_Dissolve_Weapon"))
                        {
                            m_bAppear = true;
                        }


                        if (!strcmp(iter.szName, "Zoom_In"))
                        {
                            m_pCamera->Set_Camera_ZoomInSpeed(10.f);
                            m_pCamera->ZoomIn();
                        }

                    }

                    else
                    {
                        if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() > iter.fEndTime)
                        {
                            if (!strcmp(iter.szName, "Zoom_In"))
                            {
                                m_pCamera->ResetZoomInCameraPos(10.f);
                            }
                        }
                    }
                }
                break;
                }

                if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로         

#pragma region Effect
                    if (!strcmp(iter.szName, "Effect1_Start"))
                        m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SCYTHE_1, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                    else if (!strcmp(iter.szName, "Effect2_Start"))
                        m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_SCYTHE_2, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                    else if (!strcmp(iter.szName, "Particle1_Start"))
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_1, *m_pParentWorldMatrix);
                    else if (!strcmp(iter.szName, "Particle2_Start"))
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_2, *m_pParentWorldMatrix);
#pragma endregion
                }
            }
        }
    }

    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);

        if (*m_pParentPhaseState != CPlayer::PHASE_EXECUTION
            && !(m_pSet_Body_States->count(curState))
            && !(m_pSet_Claw_Weapon_States->count(curState))
            && !(m_pSet_Axe_Weapon_States->count(curState))
            && !(m_pSet_Right_Weapon_States->count(curState))
            && !(m_pSet_Halberd_Weapon_States->count(curState))
            && !(m_pSet_GreadSword_Weapon_States->count(curState))
            && !(m_pSet_JavelinSword_Weapon_States->count(curState))
            && !(m_pSet_Player_Camera_States->count(curState))
            && !(m_pSet_Cane_Weapon_States->count(curState))
            )
        {
            /* 카메라 관련 */
            m_pCamera->ResetZoomInCameraPos(10.f);
            m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        }
    }

    if (m_bDeadOn)
        m_fFinishTime += fTimeDelta * 7.f;

    if (m_bAppear)
        m_fAppearTimer += fTimeDelta * 8.f;

    if (*m_pParentState != *m_pPreParentState)
    {
        m_bAppear = false;
        m_bDeadOn = false;
        m_fAppearTimer = 0.f;
        m_fDeadTimer = 0.f;
        m_fFinishTime = 0.f;
        m_fHitStopTime = 0.f;
    } // 이전하고 현재 비교 해야함 


    if (m_bHitStopOnOff)
    {
        Hit_Slow();
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f,0.f,1.f });

}

void CWeapon_Scythe::Late_Update(_float fTimeDelta)
{

    if (*m_pParentState == CPlayer::STATE_SCYTHE_B)
    {

        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
    }
}

HRESULT CWeapon_Scythe::Render()
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

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Scythe::Render_Glow()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 9)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveNoiseTexture", 7)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTime, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_ReverseDissolveTime", &m_fAppearTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Dead", &m_bDeadOn, sizeof(_bool))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Appear", &m_bAppear, sizeof(_bool))))
            return E_FAIL;

        m_pShaderCom->Begin(14);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Scythe::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Scythe"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CWeapon_Scythe::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Scythe::Hit_Slow()
{
    if (m_fHitStopTime < 0.20f)
    {
        m_pCamera->ShakeOn(500.f, 500.f, 7.f, 7.f);
        m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());
        m_pGameInstance->Set_ZoomBlur_Option(true, m_fHitStopTime * 1.5f);
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fHitStopTime);
    }
    else
    {
        m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        m_bHitStopOnOff = false;
    }

    m_fHitStopTime += m_fTimeDelta;//1.f / 80.f; //         

    return S_OK;
}

void CWeapon_Scythe::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    m_fHitStopTime = 0.f;
    m_bHitStopOnOff = true;
}

void CWeapon_Scythe::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CWeapon_Scythe::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CWeapon_Scythe* CWeapon_Scythe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Scythe* pInstance = new CWeapon_Scythe(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Scythe");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Scythe::Clone(void* pArg)
{
    CWeapon_Scythe* pInstance = new CWeapon_Scythe(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Scythe");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Scythe::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
