#include "pch.h"
#include "Body_Punch_Man.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Elite_Punch_Man.h"
#include "Player.h"

CBody_Punch_Man::CBody_Punch_Man(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CBody_Punch_Man::CBody_Punch_Man(const CBody_Punch_Man& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBody_Punch_Man::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Punch_Man::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    CBody_Punch_Man::BODY_PUNCH_MAN_DESC* pDesc = static_cast<CBody_Punch_Man::BODY_PUNCH_MAN_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_iMonster_Attack = pDesc->iAttack;
    m_bDead = pDesc->bDead;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pSocketMatrix = m_pModelCom->Get_BoneMatrix("ik_hand_l");

    m_pModelCom->SetUp_Animation(9, true);

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,90.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CBody_Punch_Man::Priority_Update(_float fTimeDelta)
{
    if (*m_pParentState == STATE_DEAD)
    {
        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;
        if (m_fDeadTimer >= 1.5)
        {
            *m_bDead = true;
        }
    }
}

void CBody_Punch_Man::Update(_float fTimeDelta)
{
    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true)
                {
                    m_pGameInstance->Add_Actor_Scene(m_pActor);
                    iter.isPlay = true;
                }
            }
            else
            {
                if ((iter.eType == EVENT_COLLIDER && iter.isEventActivate == false) || m_bColliderOff == true)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor);

                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                        iter.isPlay = false;
                }
            }
        }
    }
    else
        m_pGameInstance->Sub_Actor_Scene(m_pActor);


    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pSocketMatrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });


}

void CBody_Punch_Man::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CBody_Punch_Man::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        if (*m_pParentState == STATE_DEAD)
        {
            m_iPassNum = 5;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 6)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
                return E_FAIL;
        }
        else
            m_iPassNum = 0;

        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_Punch_Man::Render_Shadow()
{
    return S_OK;
}

HRESULT CBody_Punch_Man::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Punch_Man_Body"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Punch_Man::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CBody_Punch_Man::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;

        if (m_pModelCom->Get_Current_Animation_Index() == 0 || m_pModelCom->Get_Current_Animation_Index() == 1)
        {
            //PunchMan 은 휘두르기 공격일때만 플레이어가 맞는 소리가 나기에 이런 조건을 추가해둠

            if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3 + 1;

                switch (iRandSoundFileNum)
                {
                case 1:
                    m_pGameInstance->Play_Sound(L"Punchman_ToPlayer_Damage.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                    break;
                case 2:
                    m_pGameInstance->Play_Sound(L"Punchman_ToPlayer_Damage_2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                    break;
                }
            }
        }
        else
        {
            if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
            {
                _uint iRandSoundFileNum = {};
                iRandSoundFileNum = rand() % 3 + 1;

                switch (iRandSoundFileNum)
                {
                case 1:
                    m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.5f);
                    break;
                case 2:
                    m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.5f);
                    break;
                case 3:
                    m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.5f);
                    break;
                }
            }
        }
    }
}

void CBody_Punch_Man::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBody_Punch_Man::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBody_Punch_Man* CBody_Punch_Man::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Punch_Man* pInstance = new CBody_Punch_Man(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Punch_Man");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Punch_Man::Clone(void* pArg)
{
    CBody_Punch_Man* pInstance = new CBody_Punch_Man(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Punch_Man");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Punch_Man::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
