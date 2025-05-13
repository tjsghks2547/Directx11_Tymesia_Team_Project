#include "Trigger.h"

CTrigger::CTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : m_pDevice{ _pDevice }
    , m_pContext{ _pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CTrigger::Initialize()
{
    return S_OK;
}

void CTrigger::Execute()
{
    if (true == m_bFadeActivated)
        return;
    m_bFadeActivated = true;

    switch (m_eTriggerType)
    {
    case TRIGGER_TYPE::TT_FADE_IN:
        m_fAlpha = 0.0f;
        break;
    case TRIGGER_TYPE::TT_FADE_OUT:
        m_fAlpha = 1.0f;
        break;
    case TRIGGER_TYPE::TT_INTERACTION:
        break;
    }
}

void CTrigger::Update(_float _fTimeDelta)
{
    // È¤½Ã¸ô¶ó¼­ ³ÀµÒ;;°³¹«¼­¿ò;;¤·¤¸?¤·¤·¤¶¤·¤¸
    /*if (nullptr == m_pTarget || false == m_bFadeActivated)
        return;

    if (true == m_bFadeOutCompleted || true == m_bFadeInCompleted)
        return;

    if (m_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
    {
        if (m_fAlpha > 0.0f)
        {
            m_fAlpha -= _fTimeDelta / m_fDuration;
            if (m_fAlpha < 0.0f)
            {
                m_fAlpha = 0.0f;
                m_bFadeActivated = false;
                m_bFadeInCompleted = true;
            }
        }
    }
    else if (m_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
    {
        if (m_fAlpha < 1.0f)
        {
            m_fAlpha += _fTimeDelta / m_fDuration;
            if (m_fAlpha > 1.0f)
            {
                m_fAlpha = 1.0f;
                m_bFadeActivated = false;
                m_bFadeOutCompleted = true;
            }
        }
    }

    if (m_pUITarget)
        m_pUITarget->Set_Alpha(m_fAlpha);*/


    if (nullptr == m_pTarget || !m_bFadeActivated || nullptr == m_pUITarget)
        return;

    switch (m_eTriggerType)
    {
    case TRIGGER_TYPE::TT_FADE_IN:
        if (m_fAlpha > 0.0f)
        {
            m_fAlpha -= _fTimeDelta / m_fDuration;
            if (m_fAlpha <= 0.0f)
            {
                m_fAlpha = 0.0f;
                m_bFadeActivated = false;
                m_bFadeInCompleted = true;
            }
        }
        break;

    case TRIGGER_TYPE::TT_FADE_OUT:
        if (m_fAlpha < 1.0f)
        {
            m_fAlpha += _fTimeDelta / m_fDuration;
            if (m_fAlpha >= 1.0f)
            {
                m_fAlpha = 1.0f;
                m_bFadeActivated = false;
                m_bFadeOutCompleted = true;
            }
        }
        break;

    case TRIGGER_TYPE::TT_FADE_AUTO:
        if (!m_bFadeOutCompleted)
        {
            m_fAlpha += _fTimeDelta / m_fAutoFadeOutDuration;
            if (m_fAlpha >= 1.0f)
            {
                m_fAlpha = 1.0f;
                m_bFadeOutCompleted = true;

                m_bFadeActivated = true;
                m_fDuration = m_fAutoFadeInDuration;
            }
        }
        else if (!m_bFadeInCompleted)
        {
            m_fAlpha -= _fTimeDelta / m_fAutoFadeInDuration;
            if (m_fAlpha <= 0.0f)
            {
                m_fAlpha = 0.0f;
                m_bFadeActivated = false;
                m_bFadeInCompleted = true;
            }
        }
        break;
    }

    if (m_pUITarget)
        m_pUITarget->Set_Alpha(m_fAlpha);
}

HRESULT CTrigger::Fade_Out(_float _fDuration)
{
    if (nullptr == m_pTarget)
        return E_FAIL;

    m_fAlpha = 0.0f;
    m_fDuration = _fDuration;
    m_bFadeActivated = true;
    m_bFadeOutCompleted = false;

    m_pUITarget = dynamic_cast<CUIObject*>(m_pTarget);

    return S_OK;
}

HRESULT CTrigger::Fade_In(_float _fDuration)
{
    if (nullptr == m_pTarget)
        return E_FAIL;

    m_fAlpha = 1.0f;
    m_fDuration = _fDuration;
    m_bFadeActivated = true;
    m_bFadeInCompleted = false;


    m_pUITarget = dynamic_cast<CUIObject*>(m_pTarget);

    return S_OK;
}

HRESULT CTrigger::Fade_Auto(_float _fFadeOutDuration, _float _fFadeInDuration)
{
    if (nullptr == m_pTarget)
        return E_FAIL;

    m_fAlpha = 0.0f;
    m_fAutoFadeOutDuration = _fFadeOutDuration;
    m_fAutoFadeInDuration = _fFadeInDuration;

    m_fDuration = m_fAutoFadeOutDuration;
    m_bFadeActivated = true;

    m_bFadeOutCompleted = false;
    m_bFadeInCompleted = false;
    m_eTriggerType = TRIGGER_TYPE::TT_FADE_AUTO;

    m_pUITarget = dynamic_cast<CUIObject*>(m_pTarget);

    return S_OK;
}

CTrigger* CTrigger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext/*, CGameObject* _pTarget*/)
{
    CTrigger* pInstance = new CTrigger(_pDevice, _pContext/*, _pTarget*/);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CTrigger");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTrigger::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
