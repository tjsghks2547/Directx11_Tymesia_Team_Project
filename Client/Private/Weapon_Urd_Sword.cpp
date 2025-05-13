#include "pch.h"
#include "Weapon_Urd_Sword.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Boss_Urd.h"
#include "Camera_Free.h"
#include "Player.h"

CWeapon_Urd_Sword::CWeapon_Urd_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CWeapon_Urd_Sword::CWeapon_Urd_Sword(const CWeapon_Urd_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CWeapon_Urd_Sword::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Urd_Sword::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    URD_SWORD_DESC* pDesc = static_cast<URD_SWORD_DESC*>(pArg);

    m_pIntroSocketMatrix = pDesc->pIntroSocketMatrix;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_iMonster_Attack = pDesc->iAttack;
    m_bChange_Socket = pDesc->bChange_Socket;
    m_bCollider_Change = pDesc->bCollider_Change;

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

void CWeapon_Urd_Sword::Priority_Update(_float fTimeDelta)
{
    _vector vWorldPos = { m_CombinedWorldMatrix._41,m_CombinedWorldMatrix._42,m_CombinedWorldMatrix._43,1.f };


    _vector vViewPos = XMVector4Transform(vWorldPos, m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE::D3DTS_VIEW));
    _vector vClipPos = XMVector4Transform(vViewPos, m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE::D3DTS_PROJ));


    // 클립 공간 → NDC (정규화 디바이스 좌표)
    vClipPos /= vClipPos.m128_f32[3];

    //NDC → 화면 좌표(0~1 UV)
    m_fObject_UV_Pos.x = vClipPos.m128_f32[0] * 0.5f + 0.5f;
    m_fObject_UV_Pos.y = -vClipPos.m128_f32[1] * 0.5f + 0.5f + 0.05f;


    if (m_pCamera == nullptr)
        m_pCamera = static_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(LEVEL_OCEAN, TEXT("Layer_Camera"), "Camera_Free"));

}

void CWeapon_Urd_Sword::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = {};


    if (!*m_bChange_Socket)
        SocketMatrix = XMLoadFloat4x4(m_pIntroSocketMatrix);
    else
        SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {

                if (iter.eType == EVENT_STATE && iter.isEventActivate)
                {
                    if (!strcmp(iter.szName, "Reverse_Screen"))
                    {
                        m_pGameInstance->Set_ReverseScreenEffect(true);
                        m_fReverseRadius += fTimeDelta * 0.1f;
                        m_pGameInstance->Set_ReverseScreenRadius(m_fReverseRadius * 0.15f);

                        m_fZoomBlurStrength += fTimeDelta;

                        if (m_fReverseRadius > 0.1f)
                        {
                            m_fReverseRadius += fTimeDelta * 30.f;
                        }

                        m_pGameInstance->Set_Zoom_Blur_Center(m_fObject_UV_Pos);


                        if (m_fZoomBlurStrength >= 2.f)
                            m_fZoomBlurStrength = 2.f;

                        m_pCamera->ShakeOn(800.f, 800.f, 6.f, 6.f);

                        m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurStrength * 0.15f);

                    }


                }

                if (iter.eType == EVENT_STATE && !iter.isEventActivate)
                {
                    if (!strcmp(iter.szName, "Reverse_Screen"))
                    {
                        if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                        {
                            //m_pGameInstance->Set_ReverseScreenEffect(false);

                            m_pGameInstance->Set_ReverseEnd(true, m_fReverseEndTime);

                            m_fReverseEndTime -= fTimeDelta * 0.5f;

                            m_fZoomBlurStrength -= fTimeDelta;
                            m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurStrength * 0.075f);

                            if (m_fZoomBlurStrength <= 0.f)
                            {
                                m_pGameInstance->Set_ZoomBlur_Option(false, 0.0f);
                                m_fReverseRadius = 0.f;

                            }


                            if (m_fReverseEndTime <= 0.f)
                            {
                                m_pGameInstance->Set_ReverseEnd(false, 0.f);
                                m_pGameInstance->Set_ReverseScreenEffect(false);
                                m_fReverseEndTime = 1.f;
                                iter.isPlay = true;
                            }


                        }
                    }


                }
                //내가 넣은 콜라이더 시간에 진입했을때
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate && !*m_bCollider_Change)
                {
                    m_pGameInstance->Add_Actor_Scene(m_pActor);
                    iter.isPlay = true;
                }
            }
            else
            {
                //내가 넣은 콜라이더 시간이 끝났을때나 플레이어한테 닿아서 데미지를 입혔을경우. 콜라이더를 꺼라.
                if ((iter.eType == EVENT_COLLIDER && !iter.isEventActivate && !*m_bCollider_Change) || m_bColliderOff)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor);
                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                    {
                        iter.isPlay = false;
                    }
                }
            }
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
    {
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f, 0.f,1.f });
    }

}

void CWeapon_Urd_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Urd_Sword::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


        m_pShaderCom->Begin(17);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Urd_Sword::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Urd_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Urd_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Urd_Sword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
                m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 1:
                m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 2:
                m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            case 3:
                m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
                break;
            }
        }
    }
}

void CWeapon_Urd_Sword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Urd_Sword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Urd_Sword* CWeapon_Urd_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Urd_Sword* pInstance = new CWeapon_Urd_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Urd_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Urd_Sword::Clone(void* pArg)
{
    CWeapon_Urd_Sword* pInstance = new CWeapon_Urd_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Urd_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Urd_Sword::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
