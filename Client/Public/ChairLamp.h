#pragma once
#include "Client_Defines.h"
#include "SpecificObject.h"
#include "Chair.h"
#include "Lamp.h"
#include "GhostAisemy.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
END

BEGIN(Client)
class CChairLamp final : public CSpecificObject
{
public:
    struct CHAIRLAMP_DESC : public CSpecificObject::SpecificObject_Desc
    {

    };

private:
    CChairLamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CChairLamp(const CChairLamp& Prototype);
    virtual ~CChairLamp() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;
    virtual HRESULT                 Render_Glow() override;

    virtual _bool                   Get_ColliderOn() { return m_bColliderOn; }
    virtual void                    Set_ColliderOn(_bool _bColliderOn) { m_bColliderOn = _bColliderOn; }

public:
    virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);


    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();


    /* 의자 포인터 가져오기 위해서 만듬 ( 선환 ) */
    virtual CGameObject* Get_Chair_GameObj() { return m_pChair; }

private:
    _bool                           m_bFirstTouch = { false };
    _bool                           m_bActivateChairLamp = { false };
    CChair*                         m_pChair = { nullptr };
    CLamp*                          m_pLamp = { nullptr };
    CGhostAisemy*                   m_pGosemy = { nullptr };

    CTexture*                       m_pNoiseTextureCom = { nullptr };
    _float                          m_fDissolveValue = {};


#pragma region Effect
    _bool                           m_bEffect = { false };
#pragma endregion

public:
    static CChairLamp*              Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject*            Clone(void* pArg) override;
    virtual void                    Free() override;
};

END

