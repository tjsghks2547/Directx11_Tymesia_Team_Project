#include "GameInstance.h"
#include "EffectMgr.h"
#include "Effect.h"

CEffectMgr::CEffectMgr()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    m_vecEffect.resize((_uint)EFFECT_NAME::EFFECT_NAME_END);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CEffectMgr::Initialize()
{
    return S_OK;
}

void CEffectMgr::Priority_Update(_float _fTimeDelta)
{
    for (auto& iter : m_dequePlayingEffect)
        iter->Priority_Update(_fTimeDelta);

}

void CEffectMgr::Update(_float _fTimeDelta)
{
    for (auto& iter : m_dequePlayingEffect)
        iter->Update(_fTimeDelta);
}

void CEffectMgr::Late_Update(_float _fTimeDelta)
{
    for (auto& iter = m_dequePlayingEffect.begin(); iter != m_dequePlayingEffect.end();)
    {
        if (false == (*iter)->Get_IsPlaying())
        {
            (*iter)->Clear_Setting();
            if (1 == m_dequePlayingEffect.size())
            {
                m_dequePlayingEffect.clear();
                break;
            }
            iter = m_dequePlayingEffect.erase(iter);
        }
        else
        {
            (*iter)->Late_Update(_fTimeDelta);
            ++iter;
        }
    }
}

HRESULT CEffectMgr::Add_Effect(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, EFFECT_NAME _eEffectName, void* _pArg)
{
    CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, _iPrototypeLevelIndex, _strPrototypeTag, _pArg));
    if (nullptr == pGameObject)
        return E_FAIL;



    m_vecEffect[_uint(_eEffectName)].push_back(dynamic_cast<CEffect*>(pGameObject));

    return S_OK;
}

HRESULT CEffectMgr::Play_Effect(EFFECT_NAME _eEffectName, _fvector _vPos)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, _vPos);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_With_Timer(EFFECT_NAME _eEffectName, _float _fDuration, _fvector _vPos)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_MaxTimer(_fDuration);
            iter->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, _vPos);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_SettingMatrix(_pMatrix);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Speed_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float* _pAnimation_Speed)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_SettingMatrix(_pMatrix);
            iter->Set_Animation_Speed(_pAnimation_Speed);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_With_Timer_Matrix(EFFECT_NAME _eEffectName, _float _fDuration, const _float4x4* _pMatrix)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_MaxTimer(_fDuration);
            iter->Set_SettingMatrix(_pMatrix);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Matrix_With_Socket(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_SettingMatrix(_pMatrix);
            iter->Set_SocketMatrix(_pSocketMatrix);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Matrix_With_Socket_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix, _fvector _vDir)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_SettingMatrix(_pMatrix);
            iter->Set_SocketMatrix(_pSocketMatrix);
            iter->Set_Direction(_vDir);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Dir(EFFECT_NAME _eEffectName, _fvector _vPos, _fvector _vDir)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_Direction(_vDir);
            iter->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, _vPos);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Matrix_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, _fvector _vDir)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_Direction(_vDir);
            iter->Set_SettingMatrix(_pMatrix);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Play_Effect_Matrix_OneMoment(EFFECT_NAME _eEffectName, _float4x4 _matMatrix)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (false == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(true);
            iter->Set_Pos_With_Matrix(_matMatrix);
            m_dequePlayingEffect.push_back(iter);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Stop_Effect(EFFECT_NAME _eEffectName)
{
    for (auto& iter : m_vecEffect[(_uint)_eEffectName])
    {
        if (true == iter->Get_IsPlaying())
        {
            iter->Set_IsPlaying(false);
            break;
        }
    }
    return S_OK;
}

HRESULT CEffectMgr::Reset_Effect()
{
    for (_uint i = 0; i < static_cast<_uint>(EFFECT_NAME::EFFECT_NAME_END); i++)
    {
        for (auto& iter : m_vecEffect[i])
        {
            if (true == iter->Get_IsPlaying())
                iter->Set_IsPlaying(false);
        }
    }
    
    m_dequePlayingEffect.clear();

    return S_OK;
}


CEffectMgr* CEffectMgr::Create()
{
    CEffectMgr* pInstance = new CEffectMgr();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CEffectMgr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffectMgr::Free()
{
    __super::Free();

    m_dequePlayingEffect.clear();

    for (_uint i = 0; i < (_uint)EFFECT_NAME::EFFECT_NAME_END; i++)
    {
        for (auto& iter : m_vecEffect[i])
            Safe_Release(iter);

        m_vecEffect[i].clear();
    }

    m_vecEffect.clear();

    Safe_Release(m_pGameInstance);
}
