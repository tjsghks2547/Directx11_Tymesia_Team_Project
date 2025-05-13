#pragma once
#include "SpecificObject.h"
#include "Button.h"

BEGIN(Client)
class CDoorObject final : public CSpecificObject
{
public:
    struct DOOR_DESC : public CSpecificObject::SpecificObject_Desc
    {
        _uint		    iObjectNumber = {};
        CDoorObject*    pDoorObject = { nullptr };
    };

private:
    CDoorObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CDoorObject(const CDoorObject& _Prototype);
    virtual ~CDoorObject() = default;

public:
    virtual HRESULT				Initialize_Prototype() override;
    virtual HRESULT				Initialize(void* pArg) override;
    virtual void				Priority_Update(_float fTimeDelta) override;
    virtual void				Update(_float fTimeDelta) override;
    virtual void				Late_Update(_float fTimeDelta) override;
    virtual HRESULT				Render() override;
    virtual HRESULT             Render_Glow() override;

public:
    void Set_Open() { m_bOpen = !m_bOpen; }
    void Set_Close() { m_bClose = !m_bClose; }

public:
    virtual void                OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

private:
    virtual HRESULT				Ready_Components() override;
    virtual HRESULT				Bind_ShaderResources() override;

private:
   // class CGameObject* m_pInteractButton = { nullptr };
    //CGameObject* m_pButtonGameObject = { nullptr };
    //CButton* m_pButton = { nullptr };
    _bool						m_bInteractOn = { false };
    _float						m_fAlphaValue = {};
    _bool                       m_bFadingIn = { false };
    _bool                       m_bFadingOut = { false };
    _bool                       m_bOpen = { false };
    _bool                       m_bClose = { false };
    _float                      m_fRotationPerSec = { 30.f };

public:
    static CDoorObject* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void                Free() override;
};

END