#pragma once
#include "Base.h"
#include "Trigger.h"

BEGIN(Engine)
class CTriggerManager final : public CBase
{
private:
    CTriggerManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CTriggerManager() = default;

public:
    HRESULT                                     Initialize();
    HRESULT                                     Set_BlackScreen(CUIObject* _pBlackScreen);
    HRESULT                                     Add_Trigger(TRIGGER_TYPE _eTriggerType, CGameObject* _pTarget = nullptr);
    HRESULT                                     Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _Duration, _float _fDurationSecond = 0.0f);
    HRESULT                                     Activate_Awake(TRIGGER_TYPE _eTirggerType, CGameObject* _pTarget, _bool _bAwake);
    void                                        Update(_float _fTimeDelta);

    _bool                                       Is_Fade_Complete(TRIGGER_TYPE _eTriggerType);
    _bool                                       IsFading() { return m_bFading; }

    void                                        Setting_NULL_BlackScreen();
    void                                        Trigger_Clear();

private:
    ID3D11Device*                               m_pDevice = { nullptr };
    ID3D11DeviceContext*                        m_pContext = { nullptr };

    CUIObject*                                  m_pBlackScreen = { nullptr };
    _bool                                       m_bFading = { false };
    bool                                        m_bFadeOutCompletedOnce = { false };
    bool                                        m_bFadeInCompletedOnce = { false };

private:
    vector<CTrigger*>                           m_vecTriggers;
    map<TRIGGER_TYPE, vector<CTrigger*>>        m_mapTriggers;
    TRIGGER_TYPE                                m_eTriggerType = { TRIGGER_TYPE::TT_END };
public:
    static CTriggerManager*                     Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void                                Free() override;
};
END
