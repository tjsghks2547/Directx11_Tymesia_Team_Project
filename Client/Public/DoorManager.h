#pragma once
#include "SpecificObject.h"
#include "Button.h"

BEGIN(Client)

class CDoorObject;

class CDoorManager final : public CSpecificObject
{
public:
    struct DOOR_MANAGER_DESC : public CSpecificObject::SpecificObject_Desc
    {
        _uint		    iObjectNumber = {};
        CDoorObject*    pObjectLeft = { nullptr };
        CDoorObject*    pObjectRight = { nullptr };

        _float4         pLeftPosition = {};
        _float4         pRightPosition = {};
    };

private:
    CDoorManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CDoorManager(const CDoorManager& _Prototype);
    virtual ~CDoorManager() = default;

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
    virtual HRESULT				Bind_ShaderResources() override;

private:
    class CGameObject*          m_pInteractButton = { nullptr };
    //CGameObject*                m_pButtonGameObject = { nullptr };
    CButton*                     m_pButton = { nullptr };
    _bool						m_bInteractOn = { false };
    _float						m_fAlphaValue = {};
    _bool                       m_bFadingIn = { false };
    _bool                       m_bFadingOut = { false };

private:
    CDoorObject*                m_pDoorLeft = { nullptr };
    CDoorObject*                m_pDoorRight = { nullptr };

    _bool                       m_bOpened = { false };
    _float                      m_fOpenTime = { 0.f };

public:
    static CDoorManager* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void                Free() override;
};

END