#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CTriggerObject final : public CGameObject
{
public:
    struct TC_DESC : public CGameObject::GAMEOBJECT_DESC
    {
        _float4 fPosition = {};
        _float3 fRotation = {};
        _float3 fScale = {};
    };

    struct TC_INFO
    {
        _float4 fPosition = {};
        _float3 fRotation = {};
        _float3 fScale = {};
    };

private:
    CTriggerObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CTriggerObject(const CTriggerObject& _Prototype);
    virtual ~CTriggerObject() = default;

public:
    virtual HRESULT										Initialize_Prototype() override;
    virtual HRESULT										Initialize(void* _pArg) override;

    virtual void										Priority_Update(_float _fTimeDelta) override;
    virtual void										Update(_float _fTimeDelta) override;
    virtual void										Late_Update(_float _fTimeDelta) override;

    virtual HRESULT										Render() override;

    TC_INFO                                             Get_Info() const { return m_tagInfoTempCollider; }
    void                                                Set_Info(TC_INFO _tagInfo) { m_tagInfoTempCollider = _tagInfo; }
public:
    HRESULT                                             Ready_Components();

    TRIGGER_TYPE                                        m_eTriggerType = { TRIGGER_TYPE::TT_END };
private:
    PxRigidDynamic*                                     m_pActor = { nullptr };


private:
    _bool                                               m_bFade = { false };
    TC_INFO                                             m_tagInfoTempCollider;

public:
    virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
    static CTriggerObject*                              Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject*                                Clone(void* _pArg) override;
    virtual void                                        Free() override;
};
END
