#include "pch.h"
#include "DoorManager.h"

#include "DoorObject.h"

CDoorManager::CDoorManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CSpecificObject{ _pDevice , _pContext }
{
}

CDoorManager::CDoorManager(const CDoorManager& _Prototype)
    :CSpecificObject(_Prototype)
{
}

HRESULT CDoorManager::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDoorManager::Initialize(void* _pArg)
{
    DOOR_MANAGER_DESC* pDesc = static_cast<DOOR_MANAGER_DESC*>(_pArg);

    strcpy_s(m_szName, pDesc->ObjectName.c_str());
    m_iCurrentLevel = pDesc->iCurLevel;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 1.f, 1.f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    m_bColliderOn = true;

    m_pDoorLeft = pDesc->pObjectLeft;
    m_pDoorRight = pDesc->pObjectRight;

    Safe_AddRef(m_pDoorLeft);
    Safe_AddRef(m_pDoorRight);

    return S_OK;
}

void CDoorManager::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CDoorManager::Update(_float fTimeDelta)
{
    if (m_bOpened && (m_fOpenTime > 0.f && m_fOpenTime < 2.f))
    {
        m_fOpenTime += fTimeDelta;

        if (m_fOpenTime >= 2.f)
        {
            m_pDoorLeft->Set_Open();
            m_pDoorRight->Set_Open();


            m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Metal_Latch_Door_Hit_1.ogg"), CHANNELID::SOUND_STRUCT, 0.3f);

            m_fOpenTime = 2.f;
        }
    }/*
    else if (!m_bOpened && (m_fOpenTime > 0.f && m_fOpenTime < 3.f))
    {
        m_fOpenTime -= fTimeDelta;

        if (m_fOpenTime <= 0.f)
        {
            m_pDoorLeft->Set_Close();
            m_pDoorRight->Set_Close();

            m_fOpenTime = 0.f;
        }
    }*/



    if (m_bColliderOn)
    {
        m_pGameInstance->Add_Actor_Scene(m_pActor);

        if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
            m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 100.f,0.f,1.f });
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }
}

void CDoorManager::Late_Update(_float fTimeDelta)
{
}
HRESULT CDoorManager::Render()
{
    return S_OK;
}

HRESULT CDoorManager::Render_Glow()
{
    return S_OK;
}

void CDoorManager::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vDoorPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vDoorPos = XMVectorSetY(vDoorPos, XMVectorGetY(vDoorPos) + 1.0f);

    _float4 vDoorUpPosition;
    XMStoreFloat4(&vDoorUpPosition, vDoorPos);

    m_pButton->Set_WorldPosition(vDoorUpPosition);

    if (!m_bOpened)
    {
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("¿­±â"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;/*
        m_bFadingIn = true;
        m_bFadingOut = false;*/
    }
    //else if(m_bOpened)
    //{
    //    m_pButton->Set_ButtonText(TEXT("E"), TEXT("´Ý±â"));
    //    m_pButton->Activate_Button(true);
    //    m_bInteractOn = true;/*
    //    m_bFadingIn = true;
    //    m_bFadingOut = false;*/
    //    //m_pDoorLeft->Set_Open();

    //   // m_bOpened = false;
    //}
}

void CDoorManager::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (m_pGameInstance->isKeyEnter(DIK_E) && !m_bOpened && !(m_fOpenTime > 0.f && m_fOpenTime < 3.f) && m_pGameInstance->Get_Item_Count(ITEM_TYPE::ITEM_KEY2))
    {
        m_pGameInstance->Use_Item(ITEM_TYPE::ITEM_KEY2, 1);

        m_pDoorLeft->Set_Open();
        m_pDoorRight->Set_Open();

        m_fOpenTime += 0.001f;

        m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Large_Gate_Close02.ogg"), CHANNELID::SOUND_NATURAL, 0.8f);

        m_bOpened = true;
    }/*
    else if (m_pGameInstance->isKeyEnter(DIK_E) && m_bOpened && !(m_fOpenTime > 0.f && m_fOpenTime < 3.f))
    {
        m_pDoorLeft->Set_Close();
        m_pDoorRight->Set_Close();

        m_fOpenTime -= 0.001f;

        m_bOpened = false;
    }*/
}

void CDoorManager::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
    m_bInteractOn = false;/*
    m_bFadingIn = false;
    m_bFadingOut = true;*/
}

HRESULT CDoorManager::Ready_Components()
{
    return S_OK;
}

HRESULT CDoorManager::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

CDoorManager* CDoorManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CDoorManager* pInstance = new CDoorManager(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CDoorManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDoorManager::Clone(void* _pArg)
{
    CDoorManager* pInstance = new CDoorManager(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CDoorManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDoorManager::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);
    Safe_Release(m_pDoorLeft);
    Safe_Release(m_pDoorRight);
}
