#include "pch.h" 
#include "Boss_Mutation_Magician_Camera.h"  
#include "GameInstance.h"
#include "Player.h"
#include "Animation.h"
#include "Camera_Free.h"
#include "Player.h"


CBoss_Mutation_Magician_Camera::CBoss_Mutation_Magician_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CBoss_Mutation_Magician_Camera::CBoss_Mutation_Magician_Camera(const CBoss_Mutation_Magician_Camera& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CBoss_Mutation_Magician_Camera::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CBoss_Mutation_Magician_Camera::Initialize(void* pArg)
{

    strcpy_s(m_szName, "CBoss_Mutation_Magician_Camera");

    CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    //m_pParentActor = pDesc->pParentActor;     
    m_pPlayer = pDesc->pPlayer;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

    m_iCurrentLevel = static_cast<LEVELID>(pDesc->iCurLevel); //���� �߰� Level ��ȯ������

    return S_OK;
}

void CBoss_Mutation_Magician_Camera::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CBoss_Mutation_Magician_Camera::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    _vector vScale, vRotation, vTranslation;
    XMMatrixDecompose(&vScale, &vRotation, &vTranslation, SocketMatrix);

    _vector vParentScale, vParentRotation, vParentTranslation;
    XMMatrixDecompose(&vParentScale, &vParentRotation, &vParentTranslation, XMLoadFloat4x4(m_pParentWorldMatrix));

    _vector qResult = XMQuaternionMultiply(vRotation, vParentRotation);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* ���� ���� */
        SocketMatrix *  /* ���� �����̽� ���� */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* ���� ���� */
    );


#pragma region �̺�Ʈ ���� �۾�
    /* 3�� 6�� �߰� �۾� ��  �� �������� ���̵�� ������ */
    if (*m_pParentState == STATE_INTRO)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false && iter.eType == EVENT_STATE)
            {
                if (iter.isEventActivate == true) // EVENT_STATE �κ�           
                {
                    _vector pos = { m_CombinedWorldMatrix._41,m_CombinedWorldMatrix._42,m_CombinedWorldMatrix._43,1.f };
                    _vector vRight = { m_CombinedWorldMatrix._11,m_CombinedWorldMatrix._12,m_CombinedWorldMatrix._13,0.f };
                    _vector vUp = { m_CombinedWorldMatrix._21,m_CombinedWorldMatrix._22,m_CombinedWorldMatrix._23,0.f };
                    _vector vLook = { m_CombinedWorldMatrix._31,m_CombinedWorldMatrix._32,m_CombinedWorldMatrix._33,0.f };


                    _matrix Rotation = XMMatrixRotationAxis(XMVectorSetW(XMVector3Normalize(vLook), 0.f), XMConvertToRadians(-90.f));
                    vLook = XMVector3TransformNormal(vLook, Rotation);
                    vLook = XMVectorSetW(XMVector3Normalize(vLook), 0.f);

                    vRight = XMVector3TransformNormal(vRight, Rotation);
                    vRight = XMVectorSetW(XMVector3Normalize(vRight), 0.f); // ���⼭ LOOK ������ 90�� ȸ��  ( 0.f, 0.f,1.f,0.f)     

                    vUp = XMVector3TransformNormal(vUp, Rotation);
                    vUp = XMVectorSetW(XMVector3Normalize(vUp), 0.f);    // ���⼭ LOOK ������ 90�� ȸ��  ( 0.f, 0.f,1.f,0.f)  

                    m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_LOOK, vLook);
                    m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_RIGHT, vRight);
                    m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_UP, vUp);

                    m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, pos);


                    m_pCamera->Set_Camera_Cut_Scene_OnOff(true);
                    m_pCamera->Set_Camera_GetBackCamPos(true);

                    //m_pGameInstance->Sub_Actor_Scene(m_pParentActor);

                    //����ٰ� �÷��̾�� �������̰� �����ؾ���
                    m_pPlayer->Set_ParentPhaseState(CPlayer::PHASE_BOSS_INTRO);


                    if (m_bFirst)
                    {

                        _vector PlayerLook = XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
                        _vector MonsterDir = XMVector3Normalize(m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_LOOK)) * -1.f;


                        float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, MonsterDir));
                        dotResult = max(-1.0f, min(dotResult, 1.0f));
                        float Radian = acosf(dotResult);

                        _vector crossResult = XMVector3Cross(PlayerLook, MonsterDir);
                        float crossY = XMVectorGetY(crossResult);
                        if (crossY < 0.0f)
                        {
                            Radian = -Radian;
                        }

                        m_pPlayer->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
                        m_pPlayer->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, _fvector{ -42.468f, 100.59f, -120.896f, 1.f });

                        m_pPlayer->Set_PlayerState(CPlayer::STATE_IDLE);

                        m_bFirst = false;
                    }
                }

                else
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                    {
                        //if (m_pCamera->Get_Execute_CamereScene() == 0 
                        m_pCamera->Set_Camera_Cut_Scene_OnOff(false);    // ���Ⱑ ��������    
                        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 1.5f);
                        //����ٰ� �÷��̾�� ī�޶� ��ġ �������Ѿ��ҵ�.
                        _vector PlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
                        _vector PlayerLook = XMVectorSetW(XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK)), 0.f);
                        _vector PlayerBackLook = PlayerLook * -1.f;
                        _vector PlayerRight = XMVectorSetW(XMVector3Normalize(m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT)), 0.f);

                        _matrix RotationMatrix = XMMatrixRotationAxis(PlayerRight, XMConvertToRadians(30.f));

                        _vector FinalDir = XMVector3TransformNormal(PlayerBackLook, RotationMatrix);
                        _vector FinalCamPos = PlayerPos + FinalDir * 4.f;

                        m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, FinalCamPos);

                        m_pPlayer->Sub_PhaseState(CPlayer::PHASE_BOSS_INTRO);
                    }
                }
            }
        }
    }

    else
    {
        //m_pCamera->ResetZoomInCameraPos();    
    }

}

void CBoss_Mutation_Magician_Camera::Late_Update(_float fTimeDelta)
{
    m_iPreParentState = *m_pParentState;
}

HRESULT CBoss_Mutation_Magician_Camera::Render()
{


    return S_OK;
}

HRESULT CBoss_Mutation_Magician_Camera::Ready_Components()
{


    return S_OK;
}

HRESULT CBoss_Mutation_Magician_Camera::Bind_ShaderResources()
{

    return S_OK;
}

void CBoss_Mutation_Magician_Camera::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{

}

void CBoss_Mutation_Magician_Camera::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CBoss_Mutation_Magician_Camera::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CBoss_Mutation_Magician_Camera* CBoss_Mutation_Magician_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoss_Mutation_Magician_Camera* pInstance = new CBoss_Mutation_Magician_Camera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CPlayer_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBoss_Mutation_Magician_Camera::Clone(void* pArg)
{
    CBoss_Mutation_Magician_Camera* pInstance = new CBoss_Mutation_Magician_Camera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBoss_Mutation_Magician_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBoss_Mutation_Magician_Camera::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
