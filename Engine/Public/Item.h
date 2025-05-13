#pragma once
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CItem : public CGameObject
{
public:
    struct ITEM_DESC : public CGameObject::GAMEOBJECT_DESC
    {
        ITEM_TYPE           eITemType = ITEM_TYPE::ITEM_END;
        _vector             vItemPos;
        _bool               bTaken = { false };
    };

public:
    CItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CItem(const CItem& _Prototype);
    virtual ~CItem() = default;

public:
    virtual HRESULT             Initialize_Prototype() override;
    virtual HRESULT             Initialize(void* _pArg) override;
    virtual void                Priority_Update(_float _fTimeDelta) override;
    virtual void                Update(_float _fTimeDelta) override;
    virtual void                Late_Update(_float fTimeDelta) override;
    virtual HRESULT             Render()override;
    _bool                       Get_BeAcquired() const { return m_bAcquired; }
    void                        Set_BeAcquired(_bool _bAcquired) { m_bAcquired = _bAcquired; }
    void                        Set_BeActivated(_bool _bActivated) { m_bActivate = _bActivated; }
    _bool                       Get_BeDropping() const { return m_bDropping; }
    void                        Set_BeDropping(_bool _bDropping) { m_bDropping = _bDropping; }
public:
    virtual void                Set_Position(_fvector _vWorldPos);
    virtual void                Set_ItemCount(_uint _iItemCount) { m_iItemCount = _iItemCount; }
    virtual void                Set_DropItemCount(_uint _iItemCount) { m_iDropItemCount = _iItemCount; }
    virtual void                Set_BezierPosition(const _float4& _vStartPos, CGameObject* _pGameObject);

    _uint                       Get_DropItemCount() const { return m_iDropItemCount; }

    _float4                     Bezier(_float4 _vStartPos, _float4 _vCurvePos, _float4 _vEndPos, _float _fTimeDelta);

    virtual _float              Compute_LerpItemScale(_float _fStart, _float _fEnd, _float _fRatio);
    virtual void                Reset_ItemState();
    virtual void                Reset_ItemActivate();

    void                        Set_ColliderRender(_bool _bRender);

protected:
    _uint                       m_iItemCount = {};
    _uint                       m_iDropItemCount = { };
    _bool                       m_bAcquired = { false };
    _bool                       m_bDropping = { false };
protected:
    _bool                       m_bBezierMove = { false };
    _float4                     m_vInitialPos = {};
    _float4                     m_vStartPos = {};
    _float4                     m_vEndPos = {};
    _float4                     m_vCurvePos = {};
    _float4                     m_vLastPos = {};
    _float                      m_fElapsedTime = {};

protected:
    _float4                     m_fAlphaValue = {};
    _float3                     m_vCurrentScale = {};
    _float                      m_fAcquireEffectTime = 0.f;
    _float                      m_fEnLargingTime = 0.f;
    _bool                       m_bStartAcquireEffect = { false };
    _bool                       m_bFinishAcquireEffect = { false };
    _bool                       m_bEnLarging = false;
    _bool                       m_bEnLargingDone = false;

protected:
    _bool                       m_bActivate = { false };
    _bool                       m_bDissolving = { false };
    _float                      m_fDissolveTime = {};
    PxRigidDynamic*             m_pActor = { nullptr };

public:
    virtual CGameObject* Clone(void* _pArg) = 0;
    virtual void Free() override;
};

END