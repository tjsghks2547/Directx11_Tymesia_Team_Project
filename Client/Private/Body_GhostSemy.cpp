#include "pch.h"
#include "Body_GhostSemy.h"
#include "Animation.h"
#include "GameInstance.h"

CBody_GhostSemy::CBody_GhostSemy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CPartObject{ _pDevice , _pContext }
{
}

CBody_GhostSemy::CBody_GhostSemy(const CBody_GhostSemy& _Prototype)
    :CPartObject(_Prototype)
{
}

HRESULT CBody_GhostSemy::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_GhostSemy::Initialize(void* _pArg)
{
    CBody_GhostSemy::BODY_Ghost_Aisemy_DESC* pDesc = static_cast<CBody_GhostSemy::BODY_Ghost_Aisemy_DESC*>(_pArg);

    m_pParentState = pDesc->pParentState;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBody_GhostSemy::Priority_Update(_float _fTimeDelta)
{
    if (m_bActivate)
        m_pModelCom->Play_Animation(_fTimeDelta);

    if (*m_pParentState != m_iPreAnimationState)
    {
        if (*m_pParentState == 1)
        {
            m_pModelCom->SetUp_Animation(0, false);
            m_pModelCom->Get_VecAnimation().at(0)->Set_AnimationSpeed(2.5f);

        }
        if (*m_pParentState == 2)
        {
            m_pModelCom->SetUp_Animation(1, false);
            m_pModelCom->Get_VecAnimation().at(1)->Set_AnimationSpeed(2.5f);
        }
        if (*m_pParentState == 3)
        {
            m_pModelCom->SetUp_Animation(2, false);
            m_pModelCom->Get_VecAnimation().at(2)->Set_AnimationSpeed(3.f);
        }
        if (*m_pParentState == 4)
        {
            m_pModelCom->SetUp_Animation(3, true);
            m_pModelCom->Get_VecAnimation().at(3)->Set_AnimationSpeed(2.5f);
        }

        m_iPreAnimationState = *m_pParentState;
    }

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CBody_GhostSemy::Update(_float _fTimeDelta)
{
    if (!m_bActivate)
        return;

    m_fDissolveAmount += m_bReverse ? _fTimeDelta * 0.9f : -_fTimeDelta;

    if (m_fDissolveAmount >= 1.0f)
    {
        m_fDissolveAmount = 1.0f;

        if (*m_pParentState == 4)
            m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTCLE_GHOSEMY_DUST, m_pParentWorldMatrix, m_pModelCom->Get_BoneMatrix("Bone_Skirts19"));
    }
    else if (m_fDissolveAmount <= 0.0f)
    {
        m_fDissolveAmount = 0.0f;
    }
}

void CBody_GhostSemy::Late_Update(_float _fTimeDelta)
{
    if (!m_bActivate)
        return;

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
}

HRESULT CBody_GhostSemy::Render()
{
    if (!m_bActivate)
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_GhostColor", &m_vGhostColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_GhostNoiseTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveValue", &m_fDissolveAmount, sizeof(_float))))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(7);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CBody_GhostSemy::Render_Glow()
{
    if (!m_bActivate)
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_GhostColor", &m_vGhostColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pNoiseTextureCom->Bind_ShaderResource(m_pShaderCom, "g_GhostNoiseTexture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveValue", &m_fDissolveAmount, sizeof(_float))))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(7);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

void CBody_GhostSemy::Activate_SemyBody(_bool _bActivate, _uint _iType)
{
    if (_iType == 0)
    {
        m_fDissolveAmount = 0.0f;
        m_bActivate = _bActivate;
        m_bReverse = true;
    }
    else
    {
        m_fDissolveAmount = 1.0f;
        m_bActivate = _bActivate;
        m_bReverse = true;
    }
}

_bool CBody_GhostSemy::IsAnimationFinish()
{
    _bool bIsAnimationFinish = m_pModelCom->GetAniFinish();

    return bIsAnimationFinish;
}

void CBody_GhostSemy::BindOff()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.0f);
    m_pModelCom->Set_LerpFinished(true);
}

HRESULT CBody_GhostSemy::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_GameObject_Model_GhoSemy_Body"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GhostSemyNoise"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pNoiseTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_GhostSemy::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_GhostSemy* CBody_GhostSemy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CBody_GhostSemy* pInstance = new CBody_GhostSemy(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_GhostSemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_GhostSemy::Clone(void* _pArg)
{
    CBody_GhostSemy* pInstance = new CBody_GhostSemy(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_GhostSemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_GhostSemy::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pNoiseTextureCom);
}
