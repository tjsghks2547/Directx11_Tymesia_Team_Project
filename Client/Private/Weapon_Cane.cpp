#include "pch.h"
#include "Weapon_Cane.h"
#include "Animation.h"
#include "GameInstance.h"
#include "Boss_Magician.h"
#include "Player.h"

CWeapon_Cane::CWeapon_Cane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CWeapon_Cane::CWeapon_Cane(const CWeapon_Cane& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CWeapon_Cane::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Cane::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    MAGICIAN_CANE_DESC* pDesc = static_cast<MAGICIAN_CANE_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_IsDissolveOn = pDesc->IsDissolveOn;
    m_IsDissolveOff = pDesc->IsDissolveOff;
    m_bRender = pDesc->bRender;
    m_bCane_Collider_On = pDesc->bCane_Collider_On;
    m_iMonster_Attack = pDesc->iAttack;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.8f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CWeapon_Cane::Priority_Update(_float fTimeDelta)
{
    if (*m_IsDissolveOn)
    {
        m_eDissolveState = DISSOLVE_ON;
    }
    else if (*m_IsDissolveOff)
    {
        m_eDissolveState = DISSOLVE_OFF;
    }

    switch (m_eDissolveState)
    {
    case eDissolveState::DISSOLVE_ON:
        m_fDissolveTimer += fTimeDelta * 2.f;
        m_fDissolveTimer = min(m_fDissolveTimer, 1.5f);
        break;

    case eDissolveState::DISSOLVE_OFF:
        m_fDissolveTimer -= fTimeDelta * 2.f;
        m_fDissolveTimer = max(m_fDissolveTimer, 0.f);

        if (m_fDissolveTimer <= 0.f)
        {
            m_eDissolveState = eDissolveState::DISSOLVE_NONE;
        }
        break;

    case eDissolveState::DISSOLVE_NONE:
        break;
    }
}

void CWeapon_Cane::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD && *m_bCane_Collider_On)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                //내가 넣은 콜라이더 시간에 진입했을때
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate)
                {
                    m_pGameInstance->Add_Actor_Scene(m_pActor);
                    iter.isPlay = true;
                }
            }
            else
            {
                //내가 넣은 콜라이더 시간이 끝났을때나 플레이어한테 닿아서 데미지를 입혔을경우. 콜라이더를 꺼라.
                if ((iter.eType == EVENT_COLLIDER && !iter.isEventActivate) || m_bColliderOff)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor);
                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                    {
                        iter.isPlay = false;
                    }
                }
            }
            //문제 iter가 2개니까 첫 iter는 add를 했는데 다음 iter가 자기 프레임 타이밍이 아니니 Sub를 해버린다.
            //다단히트 해결하기 = 충돌나면 bool 값이 ColliderOff-> true가 되는데 이때 sub하면되는디 다시 추가되는느낌?
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }

    for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT
        {
            if (!strncmp(iter.szName, "Weapon_Trail_Cane", strlen("Weapon_Trail_Cane"))) //Trail 시작해야하는 부분
            {
                if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
                {
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
                    m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_CANE, &m_CombinedWorldMatrix);
                }
            }
        }
        else if (iter.eType == EVENT_EFFECT && iter.isEventActivate == false && true == iter.isPlay)
        {
            if (!strncmp(iter.szName, "Weapon_Trail_Cane", strlen("Weapon_Trail_Cane"))) //Trail이 꺼져야 하는 부분
            {
                m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_MAGICIAN_CANE);
                iter.isPlay = false;
            }
        }
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
    {
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f, 0.f,1.f });
    }


}

void CWeapon_Cane::Late_Update(_float fTimeDelta)
{
    if (*m_bRender)
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Cane::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (m_eDissolveState != eDissolveState::DISSOLVE_NONE)
        {
            m_iPassNum = 9;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 13)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveTimer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveTimer, sizeof(_float))))
                return E_FAIL;
        }
        else
            m_iPassNum = 17;//; 


        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Cane::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Cane"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Cane::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Cane::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;

        if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
        {
            _uint iRandSoundFileNum = {};
            iRandSoundFileNum = rand() % 4;

            switch (iRandSoundFileNum)
            {
            case 0:
                m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
                break;
            case 2:
                m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
                break;
            case 3:
                m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
                break;
            }
        }
    }
}

void CWeapon_Cane::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Cane::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Cane* CWeapon_Cane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Cane* pInstance = new CWeapon_Cane(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Cane");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Cane::Clone(void* pArg)
{
    CWeapon_Cane* pInstance = new CWeapon_Cane(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Cane");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Cane::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
