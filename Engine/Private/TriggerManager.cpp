#include "TriggerManager.h"

CTriggerManager::CTriggerManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :m_pDevice(_pDevice)
    , m_pContext(_pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CTriggerManager::Initialize()
{
    return S_OK;
}

HRESULT CTriggerManager::Set_BlackScreen(CUIObject* _pBlackScreen)
{
    if (nullptr == _pBlackScreen)
        return E_FAIL;

    m_pBlackScreen = _pBlackScreen;

    return S_OK;
}

HRESULT CTriggerManager::Add_Trigger(TRIGGER_TYPE _eTriggerType, CGameObject* _pTarget)
{
    // 킹시 몰라, 걍 레벨 넘어가고 나서 Add_Trigger 새롭게 할 때, 그냥 또 해줘벌힘;; 안쌓이게 
    // 근데 없어도 되는거같긴한데;; 걍 넣어벌힘;; 노상관인듯;;
    // 혹시몰라서 냅둠;;ㅇㅈ?ㅇㅇㅆㅇㅈ
    if (m_mapTriggers[_eTriggerType].size() > 0)
    {
        for (auto& Pair : m_mapTriggers)
        {
            for (auto& pTrigger : Pair.second)
                Safe_Release(pTrigger);

            Pair.second.clear();
        }

        m_mapTriggers[_eTriggerType].clear();
    }

    m_mapTriggers[_eTriggerType].clear();

    if (nullptr == _pTarget)
    {
        _pTarget = m_pBlackScreen;
    }

    for (auto& pTrigger : m_mapTriggers[_eTriggerType])
    {
        if (pTrigger->Get_TargetObject() == _pTarget)
        {
            return S_OK;
        }
    }

    CTrigger* pTrigger = CTrigger::Create(m_pDevice, m_pContext);
    if (nullptr == pTrigger)
        return E_FAIL;

    pTrigger->Set_Type(_eTriggerType);
    // 내가 블랙스크린이라면? 여기서 이제 종속되벌힘; triggerManager 가 관리하는 map 컨테이너에 담긴 trigger 가 블랙위도우가 되는거임
    // 근데 내가 Client 에서는 레벨전환하면서 씬전환되면서 레이어 퍼엉 날라가면서 블랙스크린도 사라지는데
    // 여기 담긴거는 이제 정체를 알수 없는 댕글링 포인터가 되벌히는거임; "있긴 있는데 정확히 알 수 없는 종자" 가 되버리는거임;;
    // 삭제는 햇는데 여기서 딱히 뭐 건든건없잖아 얘는 이미 담고잇엇고 map에다가 근데 위에서 지워버렷다니까?!
    // 그래서 Trigger 클래스의 Update가 일단은 계속 불리기는 하니까, 될 때 안될 때 잇는듯;; ㅇㅇㅆㅇㅈ
    pTrigger->Set_TargetObject(_pTarget);

    m_mapTriggers[_eTriggerType].push_back(pTrigger);

    return S_OK;
}

void CTriggerManager::Update(_float _fTimeDelta)
{
    _bool          bUpdateTrigger = false;

    for (auto iter = m_mapTriggers.begin(); iter != m_mapTriggers.end(); ++iter)
    {
        TRIGGER_TYPE eTriggerType = iter->first;
        vector<CTrigger*>& vecTriggers = iter->second;

        for (auto& pTrigger : vecTriggers)
        {
            bUpdateTrigger = true;
            pTrigger->Update(_fTimeDelta);
        }
    }

    if (!bUpdateTrigger)
        return;
}

_bool CTriggerManager::Is_Fade_Complete(TRIGGER_TYPE _eTriggerType)
{
    auto iter = m_mapTriggers.find(_eTriggerType);

    if (iter == m_mapTriggers.end())
        return false;

    for (CTrigger* pTrigger : m_mapTriggers[_eTriggerType])
    {
        if (pTrigger)
        {
           /* if (_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT && !pTrigger->IsFadeOutComplete())
                return false;
            if (_eTriggerType == TRIGGER_TYPE::TT_FADE_IN && !pTrigger->IsFadeInComplete())
                return false;*/


            if (_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
            {
                if (!pTrigger->IsFadeOutComplete())
                    return false;

                if (!m_bFadeOutCompletedOnce)
                {
                    m_bFadeOutCompletedOnce = true;
                    return true;
                }

                return false; 
            }

            if (_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
            {
                if (!pTrigger->IsFadeInComplete())
                    return false;

                if (!m_bFadeInCompletedOnce) 
                {
                    m_bFadeInCompletedOnce = true;
                    return true;
                }

                return false; 
            }
        }
    }

    return true;
}

void CTriggerManager::Setting_NULL_BlackScreen()
{
    for (auto& Pair : m_mapTriggers)
    {
        for (auto& pTrigger : Pair.second)
            Safe_Release(pTrigger);

        Pair.second.clear();
    }

    m_pBlackScreen = nullptr;
}

void CTriggerManager::Trigger_Clear()
{
    for (auto& Pair : m_mapTriggers)
    {
        for (auto& pTrigger : Pair.second)
            Safe_Release(pTrigger);

        Pair.second.clear();
    }
    m_mapTriggers.clear();
}

HRESULT CTriggerManager::Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _fDuration, _float _fDurationSecond)
{
  /*  if (nullptr == m_pBlackScreen)
        return E_FAIL;

    auto& iter = m_mapTriggers.find(_eTriggerType);
    if (iter == m_mapTriggers.end())
    {
        return E_FAIL;
    }

    for (auto& pTrigger : iter->second)
    {
        if (pTrigger->Get_TargetObject() == m_pBlackScreen)
        {
            if (_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
            {
                pTrigger->Fade_Out(_fDuration);
                m_bFadeOutCompletedOnce = false;
            }
            else if (_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
            {
                pTrigger->Fade_In(_fDuration);
                m_bFadeInCompletedOnce = false;
            }
            return S_OK;
        }
    }

    return E_FAIL;*/


    if (nullptr == m_pBlackScreen)
        return E_FAIL;

    auto& iter = m_mapTriggers.find(_eTriggerType);
    if (iter == m_mapTriggers.end())
    {
        if (_eTriggerType != TRIGGER_TYPE::TT_FADE_AUTO)
            return E_FAIL;
        return E_FAIL;
    }

    for (auto& pTrigger : iter->second)
    {
        /* if (pTrigger->Get_TargetObject() == m_pBlackScreen)
         {
             if (_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
             {
                 pTrigger->Fade_Out(_fDuration);
                 m_bFadeOutCompletedOnce = false;
             }
             else if (_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
             {
                 pTrigger->Fade_In(_fDuration);
                 m_bFadeInCompletedOnce = false;
             }
             return S_OK;
         }*/



        if (pTrigger->Get_TargetObject() == m_pBlackScreen)
        {
            switch (_eTriggerType)
            {
            case TRIGGER_TYPE::TT_FADE_OUT:
                pTrigger->Fade_Out(_fDuration);
                m_bFadeOutCompletedOnce = false;
                return S_OK;

            case TRIGGER_TYPE::TT_FADE_IN:
                pTrigger->Fade_In(_fDuration);
                m_bFadeInCompletedOnce = false;
                return S_OK;

            case TRIGGER_TYPE::TT_FADE_AUTO:

                pTrigger->Fade_Auto(_fDuration, _fDurationSecond);
                m_bFadeOutCompletedOnce = false;
                m_bFadeInCompletedOnce = false;
                return S_OK;
            }
        }
    }

    return E_FAIL;
}

HRESULT CTriggerManager::Activate_Awake(TRIGGER_TYPE _eTirggerType, CGameObject* _pTarget, _bool _bAwake)
{
    for (auto& pTrigger : m_vecTriggers)
    {
        if (pTrigger->Get_TargetObject() == _pTarget)
        {
            if (_eTirggerType == TRIGGER_TYPE::TT_INTERACTION && pTrigger->GetType() == TRIGGER_TYPE::TT_INTERACTION)
            {
                //pTrigger->
            }
        }
    }
    return S_OK;
}

CTriggerManager* CTriggerManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CTriggerManager* pInstance = new CTriggerManager(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CTriggerManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTriggerManager::Free()
{
    __super::Free();

    for (auto& Pair : m_mapTriggers)
    {
        for (auto& pTrigger : Pair.second)
            Safe_Release(pTrigger);

        Pair.second.clear();
    }
    m_mapTriggers.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
