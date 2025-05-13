#include "pch.h"
#include "ChairLamp.h"
#include "GameInstance.h"

CChairLamp::CChairLamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSpecificObject{ pDevice, pContext }
{
}

CChairLamp::CChairLamp(const CChairLamp& Prototype)
    :CSpecificObject(Prototype)
{
}

HRESULT CChairLamp::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CChairLamp::Initialize(void* pArg)
{
    CHAIRLAMP_DESC* pDesc = static_cast<CHAIRLAMP_DESC*>(pArg);

    string LampName = "NPCLamp_" + to_string(pDesc->iPairNum);

    m_iCurrentLevel = pDesc->iCurLevel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.5f, 0.5f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);
    m_bColliderOn = true;

    string ChairName = "P_Archive_Chair01_" + to_string(pDesc->iPairNum);

    m_pChair = dynamic_cast<CChair*>(m_pGameInstance->Get_GameObject_To_Layer(pDesc->iCurLevel, TEXT("Layer_SpecificObject"), const_cast<_char*>(ChairName.c_str())));



    return S_OK;
}

void CChairLamp::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CChairLamp::Update(_float fTimeDelta)
{
    if (m_bFirstTouch)
    {
        m_fDissolveValue += fTimeDelta * 0.5f;

        if (m_fDissolveValue > 1.0f)
        {
            m_fDissolveValue = 1.0f;
            m_bActivateChairLamp = true;
        }
    }

    __super::Update(fTimeDelta);
}

void CChairLamp::Late_Update(_float fTimeDelta)
{
    if (false == m_bEffect)
    {
        _float4 vPos{}, vLook{};
        XMStoreFloat4(&vPos, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        XMStoreFloat4(&vLook, m_pTransformCom->Get_State(CTransform::STATE_LOOK));

        vPos.x += vLook.x;
        vPos.y += vLook.y;
        vPos.y -= 0.3f;
        vPos.z += vLook.z;

        m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_CHAIR, XMLoadFloat4(&vPos));
        m_bEffect = true;
    }


    __super::Late_Update(fTimeDelta);
}

HRESULT CChairLamp::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (m_bActivateChairLamp)
        return S_OK;

    if (FAILED(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveValue", &m_fDissolveValue, sizeof(_float))))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(26);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CChairLamp::Render_Glow()
{
    //if (m_bFirstTouch)
    if (m_bActivateChairLamp)
        return S_OK;

    if (FAILED(__super::Render_Glow()))
        return E_FAIL;

    return S_OK;
}

void CChairLamp::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    //if (!strcmp(m_szName, ("NPCLamp")))
    if (!strncmp(m_szName, "NPCLamp", 7))
    {
        _vector vLampPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
        vLampPos = XMVectorSetY(vLampPos, XMVectorGetY(vLampPos) + 1.0f);

        _float4 vLampPosition;
        XMStoreFloat4(&vLampPosition, vLampPos);

        m_pButton->Set_WorldPosition(vLampPosition);
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("비콘 활성화"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;
        m_bFadingIn = true;
        m_bFadingOut = false;
    }
}

void CChairLamp::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (m_pGameInstance->isKeyEnter(DIK_E) && !strncmp(m_szName, "NPCLamp", 7))
    {
        m_pButton->Activate_Button(false);
        m_pGameInstance->Play_Effect(EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_GREEN, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
        if (!m_bFirstTouch)
        {
            /* 신호기 발견 알림*/
            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_Landing"), true);
            m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_2Beacon")), true);
            m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_2Beacon")), true);
            m_bFirstTouch = true;


            m_bColliderOn = false;
            m_bFadingOut = true;

            m_pGosemy = dynamic_cast<CGhostAisemy*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_NPC"), "GHOSEMY"));
            m_pLamp = m_pGosemy->Get_SemyLamp();


            if (nullptr != m_pChair)
            {
                m_pChair->Set_ColliderRender(true);
            }

            m_pLamp->LightUp_Lamp(true);
        }
        m_pGameInstance->Play_Sound(TEXT("TouchLamp.ogg"), CHANNELID::SOUND_STRUCT, 0.4f);
    }
}

void CChairLamp::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
    m_bInteractOn = false;
    m_bFadingIn = false;
    m_bFadingOut = true;
}

HRESULT CChairLamp::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChairLamp"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    return S_OK;
}

HRESULT CChairLamp::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

CChairLamp* CChairLamp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CChairLamp* pInstance = new CChairLamp(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CChairLamp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CChairLamp::Clone(void* pArg)
{
    CChairLamp* pInstance = new CChairLamp(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Created : CChairLamp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChairLamp::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);

    Safe_Release(m_pNoiseTextureCom);
}