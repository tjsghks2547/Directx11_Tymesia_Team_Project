#include "pch.h"
#include "LadderObject.h"

CLadderObject::CLadderObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CSpecificObject{ _pDevice , _pContext }
{
}

CLadderObject::CLadderObject(const CLadderObject& _Prototype)
    :CSpecificObject(_Prototype)
{
}

HRESULT CLadderObject::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLadderObject::Initialize(void* _pArg)
{
    LADDER_DESC* pDesc = static_cast<LADDER_DESC*>(_pArg);


    //if (pDesc->ObjectName == "P_Ladder02_Down")
    if (pDesc->ObjectName == "P_Ladder02_Down" + to_string(pDesc->iObjectNumber))
    {
        string strLadderDownName = "P_Ladder02_Down" + to_string(pDesc->iObjectNumber);
        //strcpy_s(m_szName, strLadderDownName.c_str());
        strcpy_s(m_szName, pDesc->ObjectName.c_str());
    }
    //else if (pDesc->ObjectName == "P_Ladder02_Up")
    else if (pDesc->ObjectName == "P_Ladder02_Up" + to_string(pDesc->iObjectNumber))
    {
        string strLadderUpName = "P_Ladder02_Up" + to_string(pDesc->iObjectNumber);
        //strcpy_s(m_szName, strLadderUpName.c_str());
        strcpy_s(m_szName, pDesc->ObjectName.c_str());
    }

    m_iCurrentLevel = pDesc->iCurLevel;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.7f, 0.7f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);

    m_bColliderOn = true;

    return S_OK;
}

void CLadderObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLadderObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

}

void CLadderObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

}
HRESULT CLadderObject::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLadderObject::Render_Glow()
{
    return S_OK;
}

void CLadderObject::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vLadderUpPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vLadderUpPos = XMVectorSetY(vLadderUpPos, XMVectorGetY(vLadderUpPos) + 1.0f);

    _float4 vLadderUpPosition;
    XMStoreFloat4(&vLadderUpPosition, vLadderUpPos);

    m_pButton->Set_WorldPosition(vLadderUpPosition);

    if (!strncmp(m_szName, "P_Ladder02_Up", 13))
    {
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("내려가기"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;
        m_bFadingIn = true;
        m_bFadingOut = false;
    }
    else if (!strncmp(m_szName, "P_Ladder02_Down", 15))
    {
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("올라가기"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;
        m_bFadingIn = true;
        m_bFadingOut = false;
    }
}

void CLadderObject::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CLadderObject::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
    m_bInteractOn = false;
    m_bFadingIn = false;
    m_bFadingOut = true;
}

HRESULT CLadderObject::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLadderObject::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

CLadderObject* CLadderObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CLadderObject* pInstance = new CLadderObject(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CLadderObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLadderObject::Clone(void* _pArg)
{
    CLadderObject* pInstance = new CLadderObject(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CStatCLadderObjecticObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLadderObject::Free()
{
    __super::Free();
}
