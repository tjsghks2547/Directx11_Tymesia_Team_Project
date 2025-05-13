#pragma once
#include "Client_Defines.h"
#include "SpecificObject.h"
#include "GhostAisemy.h"
#include "Body_GhostSemy.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CChair final : public CSpecificObject
{
public:
    struct CHAIR_DESC : public CSpecificObject::SpecificObject_Desc
    {

    };

private:
    CChair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CChair(const CChair& Prototype);
    virtual ~CChair() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;
    virtual HRESULT                 Render_Glow() override;

public:
    virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();

private:
    CGhostAisemy* m_pGosemy = { nullptr };
    CBody_GhostSemy* m_pBodyGosemy = { nullptr };
    _uint                           m_iColliderCount = {};

public:
    static                          CChair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void                    Free() override;


};

END