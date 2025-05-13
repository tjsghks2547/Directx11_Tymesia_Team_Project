#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
END

BEGIN(Client)
class CBarrierScreen final : public CGameObject
{
public:
    struct BARRIER_SCREEN_DESC : public CGameObject::GAMEOBJECT_DESC
    {
    };

private:
    CBarrierScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBarrierScreen(const CBarrierScreen& Prototype);
    virtual ~CBarrierScreen() = default;
public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;
    virtual HRESULT                 Render_Glow() override;


public:
    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();

public:
    virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

private:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    PxRigidDynamic* m_pActor = { nullptr };

private:
    _uint                           m_iCurLevel = {};
    _float                          m_fFlowTime = {};

    _bool                           m_bActivateBarrier = { false };

public:
    static CBarrierScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void                    Free() override;
};
END

