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
class CElevator_Door final : public CSpecificObject
{
public:
    struct ElevatorDoorDesc : public CSpecificObject::SpecificObject_Desc
    {
        _bool isCulling = { false };
        _tchar* pCullingLayerTag = {};
        _tchar* pLoadLayerTag = {};
        vector<_uint> iLoadIndex = {};
        class CTransform* pPlayerTransform = { nullptr };
        class CNavigation* pPlayerNavigation = { nullptr };
    };

private:
    CElevator_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CElevator_Door(const CElevator_Door& Prototype);
    virtual ~CElevator_Door() = default;

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
    HRESULT                         Load_Objects(_int iObject_Level);

public:
    static                          CElevator_Door* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void                    Free() override;

private:
    _bool m_isCulling = { false };
    _tchar* m_wstrCullingLayerTag = {};
    _tchar* m_wstrLoadLayerTag = {};
    vector<_uint> m_vecLoadIndex = { 0 };
    class CTransform* m_pPlayerTransform = { nullptr };
    class CNavigation* m_pPlayerNavigation = { nullptr };
};

END