#include "pch.h"
#include "GameInstance.h"
#include "Effect_Sword.h"

CEffect_Sword::CEffect_Sword(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
    : CEffect{ _pDevice, _pDeviceContext }
{
}

CEffect_Sword::CEffect_Sword(const CEffect_Sword& _pPrototype)
    : CEffect(_pPrototype)
{
}

HRESULT CEffect_Sword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect_Sword::Initialize(void* _Arg)
{
    EFFECT_SWORD_DESC* pDesc = static_cast<EFFECT_SWORD_DESC*>(_Arg);
    m_fMaxTimer = pDesc->fMaxTimer;
    m_iDiffuse = pDesc->iDiffuse;
    m_iShaderPass = pDesc->iShaderPass;
    m_fLength_Up = pDesc->fLength_Up;
    m_fLength_Right = pDesc->fLength_Right;
    m_fParent_Look = pDesc->fParent_Look;
    m_vRGB = pDesc->vRGB;
    m_iSword_XYZ = pDesc->iSword_XYZ;

    if (FAILED(__super::Initialize(_Arg)))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    

    m_pTransformCom->Scaling(pDesc->vScale);
    m_pTransformCom->Rotation(XMConvertToRadians(pDesc->vRot.x), XMConvertToRadians(pDesc->vRot.y), XMConvertToRadians(pDesc->vRot.z * -1.f)); //이부분은 Tool이랑 뭔가 이상함
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vTranslation), 1.f));

    return S_OK;
}

void CEffect_Sword::Priority_Update(_float _fTimeDelta)
{
}

void CEffect_Sword::Update(_float _fTimeDelta)
{
    if (nullptr != m_pSettingMatrix && nullptr == m_pSocketMatrix) //소켓이 아니다
    {
        XMStoreFloat4x4(&m_matCombined, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pSettingMatrix));
    }
	else if (nullptr != m_pSocketMatrix && nullptr != m_pSettingMatrix) //소켓이다)
    {
        _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

        XMStoreFloat4x4(&m_matCombined, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * SocketMatrix * XMLoadFloat4x4(m_pSettingMatrix));
    }
    if (true == m_bIsPlaying)
    {
        Calculate_Trail(_fTimeDelta);

        _uint iCount_Trail = static_cast<_uint>(m_dequeCenterPos.size());
        _float3 vUp{}, vRight{};
        XMStoreFloat3(&vUp, m_fLength_Up * XMVector3Normalize(XMVector3Cross(_vector{ 0.f, 1.f, 0.f, 0.f }, _vector{ m_matCombined._41, m_matCombined._42, m_matCombined._43, 0.f } - XMLoadFloat4(&m_pGameInstance->Get_CamPosition()))));
        XMStoreFloat3(&vRight, m_fLength_Right * XMVector3Normalize(XMVector3Cross(_vector{ 1.f, 0.f, 0.f, 0.f }, _vector{ m_matCombined._41, m_matCombined._42, m_matCombined._43, 0.f } - XMLoadFloat4(&m_pGameInstance->Get_CamPosition()))));
        if (iCount_Trail > 4)
        {
            m_pBufferCom->Set_Trail_Local(m_dequeCenterPos, iCount_Trail, vUp, vRight);
        }
    }
}

void CEffect_Sword::Late_Update(_float _fTimeDelta)
{
    switch (m_iShaderPass)
    {
    case 0:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
        break;
    case 1:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_DISTORTION, this);
        break;
    case 2:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_WEIGHTBLEND, this);
        break;
    }
}

HRESULT CEffect_Sword::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(m_iShaderPass);

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

HRESULT CEffect_Sword::Render_Distortion()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(m_iShaderPass);

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

HRESULT CEffect_Sword::Render_WeightBlend()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(m_iShaderPass);

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

void CEffect_Sword::Set_IsPlaying(_bool _bIsPlaying)
{
    if (true == _bIsPlaying) //시작해라 느낌
    {
        m_fTimerY = 0.f;
        m_fDissolve = 0.f;
        m_fTimer_Timelag = 0.f;
        m_bIsPlaying = _bIsPlaying;
        m_pBufferCom->Set_Trail_Reset();

        if (nullptr != m_pSettingMatrix)
            m_pSettingMatrix = nullptr;
        if (nullptr != m_pSocketMatrix)
            m_pSocketMatrix = nullptr;
    }
    m_bisCalculate = _bIsPlaying;
}

void CEffect_Sword::Calculate_Trail(_float _fTimeDelta)
{
    m_fTimerX += _fTimeDelta;

    _vector vScale{}, vRotation{}, vTranslation{};

    XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&m_matCombined));

    _float3 vPos{}, vLook_Sword{};

    XMStoreFloat3(&vPos, vTranslation);

    switch (m_iSword_XYZ)
    {
    case 0:
        vLook_Sword = { vPos.x + m_matCombined._11 * m_fParent_Look, vPos.y + m_matCombined._12 * m_fParent_Look, vPos.z + m_matCombined._13 * m_fParent_Look };
        break;
    case 1:
        vLook_Sword = { vPos.x + m_matCombined._21 * m_fParent_Look, vPos.y + m_matCombined._22 * m_fParent_Look, vPos.z + m_matCombined._23 * m_fParent_Look };
        break;
    case 2:
        vLook_Sword = { vPos.x + m_matCombined._31 * m_fParent_Look, vPos.y + m_matCombined._32 * m_fParent_Look, vPos.z + m_matCombined._33 * m_fParent_Look };
        break;
    }

    if (false == m_bisCalculate) //크기가 적다 즉 시작했을때나 끝났을때이다
    {
        Delete_Trail();
    }
    else if (m_fTimerX < 1.25f && m_dequeCenterPos.size() < 64)
    {
        m_dequeCenterPos.push_front(vLook_Sword);
        
    }
    else // 크기가 꽉찼다 맨앞꺼를 밀어버려라 || 시간이 지났다
    {
        Delete_Trail();
        m_dequeCenterPos.push_front(vLook_Sword);
        //m_bisCalculate = false;
    }
}

void CEffect_Sword::Delete_Trail()
{
    if (0 < m_dequeCenterPos.size())
        m_dequeCenterPos.pop_back();
    else// if (1 > m_dequeCenterPos.size())
    {
        m_bIsPlaying = false;
        m_fTimerX = 0.f;
        m_pBufferCom->Set_Trail_Reset();
    }
}

HRESULT CEffect_Sword::Ready_Component()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Sword"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sword_Image"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sword"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    //if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matCombined)))
    //    return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iDiffuse)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vRGB", &m_vRGB, sizeof(_float3))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimer", &m_fTimerX, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CEffect_Sword* CEffect_Sword::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
    CEffect_Sword* pInstance = new CEffect_Sword(_pDevice, _pDeviceContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed Create : Effect_Sword_Prototype");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_Sword::Clone(void* _pArg)
{
    CEffect_Sword* pInstance = new CEffect_Sword(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed Clone : Effect_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_Sword::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pBufferCom);
}
