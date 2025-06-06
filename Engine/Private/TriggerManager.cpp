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
    // 天獣 侯虞, 袷 傾婚 角嬢亜壱 蟹辞 Add_Trigger 歯罫惟 拝 凶, 益撹 暁 背操忽毘;; 照竣戚惟 
    // 悦汽 蒸嬢亀 鞠澗暗旭延廃汽;; 袷 隔嬢忽毘;; 葛雌淫昔牛;;
    // 箸獣侯虞辞 垣客;;しじ?ししざしじ
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
    // 鎧亜 鷺窟什滴鍵戚虞檎? 食奄辞 戚薦 曽紗鞠忽毘; triggerManager 亜 淫軒馬澗 map 珍砺戚格拭 眼延 trigger 亜 鷺窟是亀酔亜 鞠澗暗績
    // 悦汽 鎧亜 Client 拭辞澗 傾婚穿発馬檎辞 樟穿発鞠檎辞 傾戚嬢 遁錠 劾虞亜檎辞 鷺窟什滴鍵亀 紫虞走澗汽
    // 食奄 眼延暗澗 戚薦 舛端研 硝呪 蒸澗 寄越元 匂昔斗亜 鞠忽備澗暗績; "赤延 赤澗汽 舛溌備 硝 呪 蒸澗 曽切" 亜 鞠獄軒澗暗績;;
    // 肢薦澗 媒澗汽 食奄辞 亨備 更 闇窮闇蒸摂焼 剰澗 戚耕 眼壱責譲壱 map拭陥亜 悦汽 是拭辞 走趨獄掲陥艦猿?!
    // 益掘辞 Trigger 適掘什税 Update亜 析舘精 域紗 災軒奄澗 馬艦猿, 吃 凶 照吃 凶 責澗牛;; ししざしじ
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
