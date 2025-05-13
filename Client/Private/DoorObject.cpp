#include "pch.h"
#include "DoorObject.h"

CDoorObject::CDoorObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CSpecificObject{ _pDevice , _pContext }
{
}

CDoorObject::CDoorObject(const CDoorObject& _Prototype)
    :CSpecificObject(_Prototype)
{
}

HRESULT CDoorObject::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDoorObject::Initialize(void* _pArg)
{
    DOOR_DESC* pDesc = static_cast<DOOR_DESC*>(_pArg);

    m_fRotationPerSec = pDesc->fRotationPerSec;
    
    strcpy_s(m_szName, pDesc->ObjectName.c_str());

    m_iCurrentLevel = pDesc->iCurLevel;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 2.0f, 1.0f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);

    m_bColliderOn = true;

    pDesc->pDoorObject = this;

    return S_OK;
}

void CDoorObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CDoorObject::Update(_float fTimeDelta)
{
    if (strncmp("SM_Door_Left", m_szName, 11))
    {
        if (m_bOpen)
        {
            m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fRotationPerSec * fTimeDelta));
        }
        if (m_bClose)
        {
            m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -XMConvertToRadians(m_fRotationPerSec * fTimeDelta));
        }
    }
    else
    {
        if (m_bOpen)
        {
            m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -XMConvertToRadians(m_fRotationPerSec * fTimeDelta));
        }
        if (m_bClose)
        {
            m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_fRotationPerSec * fTimeDelta));
        }
    }

    if (m_bColliderOn)
    {
        m_pGameInstance->Add_Actor_Scene(m_pActor);

        if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
            m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ -50.f, 50.f,0.f,1.f });
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }
}

void CDoorObject::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

}
HRESULT CDoorObject::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDoorObject::Render_Glow()
{
    if (FAILED(__super::Render_Glow()))
        return E_FAIL;  

    return S_OK;
}

void CDoorObject::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    m_bFadingIn = true;
}

void CDoorObject::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CDoorObject::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_bFadingIn = false;
}

HRESULT CDoorObject::Ready_Components()
{

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Specific_Door"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CDoorObject::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

CDoorObject* CDoorObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CDoorObject* pInstance = new CDoorObject(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CDoorObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDoorObject::Clone(void* _pArg)
{
    CDoorObject* pInstance = new CDoorObject(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CDoorObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDoorObject::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);
}
