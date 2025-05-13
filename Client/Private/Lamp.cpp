#include "pch.h"
#include "Lamp.h"
#include "GameInstance.h"

CLamp::CLamp(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CPartObject{ _pDevice, _pContext }
{
}

CLamp::CLamp(const CLamp& _Prototype)
    :CPartObject(_Prototype)
{
}

HRESULT CLamp::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLamp::Initialize(void* _pArg)
{
    LAMP_DESC* pDesc = static_cast<LAMP_DESC*>(_pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CLamp::Priority_Update(_float _fTimeDelta)
{
    _matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
    _matrix OffsetMatrix = XMMatrixIdentity();
    OffsetMatrix *= XMMatrixRotationZ(XMConvertToRadians(200.0f));
    SocketMatrix = XMMatrixMultiply(OffsetMatrix, SocketMatrix);
    _vector vOffsetPos = XMVectorSet(0.0f, -150.0f, 0.0f, 0.0f);
    SocketMatrix.r[3] += vOffsetPos;

    for (size_t i = 0; i < 3; ++i)
    {
        SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);
    }

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * SocketMatrix * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CLamp::Update(_float _fTimeDelta)
{
    //if (m_bActivate)
    if (m_bLightOn)
        m_fAlphaValue += _fTimeDelta;
}

void CLamp::Late_Update(_float _fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

    if (m_bLightOn)
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
}

HRESULT CLamp::Render()
{
    if (!m_bLightOn)
        return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, (_uint)i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, (_uint)i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CLamp::Render_Glow()
{
    if (!m_bLightOn)
        return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pShaderCom->Bind_RawValue("g_fObjectAlpha", &m_fAlphaValue, sizeof(_float))))
            return E_FAIL;

        m_pShaderCom->Begin(13);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

HRESULT CLamp::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Shotgun */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Lamp"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLamp::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CLamp* CLamp::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CLamp* pInstance = new CLamp(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CLamp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLamp::Clone(void* _pArg)
{
    CLamp* pInstance = new CLamp(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CLamp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLamp::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
