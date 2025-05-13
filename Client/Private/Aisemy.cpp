#include "pch.h"
#include "Aisemy.h"
#include "GameInstance.h"
#include "Body_Aisemy.h"

CAisemy::CAisemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CAisemy::CAisemy(const CAisemy& Prototype)
    :CContainerObject(Prototype)
{
}

HRESULT CAisemy::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAisemy::Initialize(void* pArg)
{
    strcpy_s(m_szName, "NPC");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pNavigationCom->Set_CurrentNaviIndex(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

    m_pState_Manager = CState_Machine<CAisemy>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_HILL, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CAisemy::Priority_Update(_float fTimeDelta)
{
    Culling();
    if (m_bCulling)
        return;
    m_fTimeDelta = fTimeDelta;
    CalCulate_Distance();

    if (m_fDistance <= 20.f && !m_bActive)
    {
        m_bActive = true;
        m_pState_Manager->ChangeState(new CAisemy::Intro_State(), this);
    }
    else if (m_fDistance > 20.f)
        m_bActive = false;

    __super::Priority_Update(fTimeDelta);
}

void CAisemy::Update(_float fTimeDelta)
{
    if (m_bCulling)
        return;

    m_pState_Manager->State_Update(fTimeDelta, this);

    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ -150.f, 250.f,200.f,1.f });

    __super::Update(fTimeDelta);
}

void CAisemy::Late_Update(_float fTimeDelta)
{
    if (m_bCulling)
        return;

    if (m_bNeed_Rotation)
        Rotation_To_Player();

    __super::Late_Update(fTimeDelta);
}

HRESULT CAisemy::Render()
{
    return S_OK;
}

HRESULT CAisemy::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CAisemy::Ready_PartObjects()
{
    CBody_Aisemy::BODY_Aisemy_DESC BodyDesc{};

    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Aisemy"), LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy_Body"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

void CAisemy::CalCulate_Distance()
{
    XMStoreFloat4(&m_vPlayerPos, m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_vPlayerPos) - pPosition));
}

void CAisemy::Culling()
{
    //절두체 안에있을때
    if (!m_bActive)
    {
        if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER))
        {
            m_bCulling = false;
        }
        //절두체 안에 없을때
        else
        {
            m_bCulling = true;
        }
    }
}

void CAisemy::RootAnimation()
{
    //루트애니메이션이 1도없음 NPC는 

    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    /* 루트 모션 애니메션 코드 */
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");

    if ((!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished() && m_bActive))
    {
        if ((m_pNavigationCom->isMove(vCurPosition)))
            m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

        /* 2월 19일 추가 코드 */
        if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
        {
            _float4x4 test = {};
            XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
            const _float4x4* test2 = const_cast<_float4x4*>(&test);
            m_pTransformCom->Set_MulWorldMatrix(test2);
        }
    }
}

void CAisemy::RotateDegree_To_Player()
{
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vLook2 = XMLoadFloat4(&m_vPlayerPos) - vPos;

    vLook = XMVector3Normalize(vLook);
    vLook2 = XMVector3Normalize(vLook2);

    //회전해야 하는 각도
    _float fAngle = acos(XMVectorGetX(XMVector3Dot(vLook, vLook2)));
    fAngle = XMConvertToDegrees(fAngle);
    m_fRotateDegree = fAngle;
    if (m_fRotateDegree > 5.f)
    {
        m_bNeed_Rotation = true;
    }
    _vector fCrossResult = XMVector3Cross(vLook, vLook2);
    if (XMVectorGetY(fCrossResult) < 0)
    {
        m_fRotateDegree *= -1;
    }
}

void CAisemy::Rotation_To_Player()
{
    _float fRadians = 3.f;
    if (m_fRotateDegree < 0.f)
    {
        fRadians *= -1;
        m_fAngle -= 3.f;
    }
    else
    {
        m_fAngle += 3.f;
    }

    m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRadians));
    if (fabs(m_fAngle) >= fabs(m_fRotateDegree))
    {
        m_fAngle = 0.f;
        m_fRotateDegree = 0.f;
        m_bNeed_Rotation = false;
    }
}

void CAisemy::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vAisemyPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vAisemyPos = XMVectorSetY(vAisemyPos, XMVectorGetY(vAisemyPos) + 1.0f);

    _float4 vAisemyPosition;
    XMStoreFloat4(&vAisemyPosition, vAisemyPos);

    m_pButton->Set_WorldPosition(vAisemyPosition);
    m_pButton->Set_ButtonText(TEXT("E"), TEXT("대화"));
    m_pButton->Activate_Button(true);
}

void CAisemy::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    _uint iRandVoice = {};

    if (m_pGameInstance->isKeyEnter(DIK_E))
    {
        if (!m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemy"))
            && !m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemyPop")))
        {
            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true); // 마우스 이미지 켜기

            m_pButton->Activate_Button(false);

            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_Dialogue"), true);
            m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemy")), true);
        }

        //iRandVoice = rand() % 5 + 1;
        iRandVoice = 6;
        switch (iRandVoice)
        {
        case 1:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk09.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        case 2:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk08.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        case 3:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk06.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        case 4:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk04.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        case 5:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk01.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        case 6:
            m_pGameInstance->Play_Sound(TEXT("Aisemy_Talk08.ogg"), CHANNELID::SOUND_NPC, 0.3f);
            break;
        }

    }
}

void CAisemy::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
}

CAisemy* CAisemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAisemy* pInstance = new CAisemy(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAisemy::Clone(void* pArg)
{
    CAisemy* pInstance = new CAisemy(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAisemy::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}

void CAisemy::Idle_State::State_Enter(CAisemy* pObject)
{
    m_iIndex = 0;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CAisemy::Idle_State::State_Update(_float fTimeDelta, CAisemy* pObject)
{
    if (m_iIndex == 0 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 1;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    if (m_iIndex != 0)
        pObject->RotateDegree_To_Player();

    if (pObject->m_fRotateDegree > 5.f)
    {
        m_iIndex = 2;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    else if (pObject->m_fRotateDegree < -5.f)
    {
        m_iIndex = 3;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    else
    {
        m_iIndex = 1;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

}

void CAisemy::Idle_State::State_Exit(CAisemy* pObject)
{
}

void CAisemy::Intro_State::State_Enter(CAisemy* pObject)
{
    //m_iIndex = 0;
    //pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CAisemy::Intro_State::State_Update(_float fTimeDelta, CAisemy* pObject)
{
    /* if (pObject->m_pModelCom->GetAniFinish())
     {

     }*/

}

void CAisemy::Intro_State::State_Exit(CAisemy* pObject)
{
}
