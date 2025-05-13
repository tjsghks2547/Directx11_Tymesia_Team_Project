#pragma once
#include "SpecificObject.h"
#include "Button.h"

BEGIN(Client)
class CLadderObject final : public CSpecificObject
{
public:
    struct LADDER_DESC : public CSpecificObject::SpecificObject_Desc
    {
        _uint		iObjectNumber = {};
    };

private:
    CLadderObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CLadderObject(const CLadderObject& _Prototype);
    virtual ~CLadderObject() = default;

public:
    virtual HRESULT				Initialize_Prototype() override;
    virtual HRESULT				Initialize(void* pArg) override;
    virtual void				Priority_Update(_float fTimeDelta) override;
    virtual void				Update(_float fTimeDelta) override;
    virtual void				Late_Update(_float fTimeDelta) override;
    virtual HRESULT				Render() override;
    virtual HRESULT             Render_Glow() override;

public:
    virtual void                OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

private:
    virtual HRESULT				Ready_Components() override;
    //HRESULT				    Ready_Ladder_Components();
    virtual HRESULT				Bind_ShaderResources() override;

private:
    class CGameObject* m_pInteractButton = { nullptr };
    CGameObject* m_pButtonGameObject = { nullptr };
    CButton* m_pButton = { nullptr };
    _bool						m_bInteractOn = { false };
    _float						m_fAlphaValue = {};
    _bool                       m_bFadingIn = { false };
    _bool                       m_bFadingOut = { false };

private:
    _char				        m_szLadderName[MAX_PATH] = {};


public:
    static CLadderObject* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void                Free() override;
};

END