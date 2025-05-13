#include "Item.h"
#include "GameInstance.h"

CItem::CItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{ _pDevice, _pContext }
{
}

CItem::CItem(const CItem& _Prototype)
    :CGameObject(_Prototype)
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* _pArg)
{
    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    return S_OK;
}

void CItem::Priority_Update(_float _fTimeDelta)
{
}

void CItem::Update(_float _fTimeDelta)
{
}

void CItem::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
    return S_OK;
}

void CItem::Set_Position(_fvector _vWorldPos)
{
    _float fPosX = XMVectorGetX(_vWorldPos);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vWorldPos);
}

void CItem::Set_BezierPosition(const _float4& _vStartPos, CGameObject* _pGameObject)
{
    m_bActivate = true;

    m_vInitialPos = _vStartPos;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_vStartPos));

    m_fElapsedTime = 0.0f;

    _vector vTargetPos = _pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    const _float fRadius = 0.5f;
    _float fRandomX = (rand() % 100 / 100.f - 0.5f) * 2.0f * fRadius;
    _float fRandomZ = (rand() % 100 / 100.f - 0.5f) * 2.0f * fRadius;
    vTargetPos = XMVectorSetX(vTargetPos, XMVectorGetX(vTargetPos) + fRandomX);
    vTargetPos = XMVectorSetZ(vTargetPos, XMVectorGetZ(vTargetPos) + fRandomZ);

    _vector vTargetDir = XMVector3Normalize(vTargetPos - XMLoadFloat4(&_vStartPos));

    m_pTransformCom->LookAt(XMLoadFloat4(&_vStartPos) + vTargetDir);

    _vector vMiddlePoint = (XMLoadFloat4(&_vStartPos) + vTargetPos) * 0.5f;
    vMiddlePoint = XMVectorSetY(vMiddlePoint, XMVectorGetY(vMiddlePoint) + 2.0f);

    vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 0.5f);

    XMStoreFloat4(&m_vEndPos, vTargetPos);
    XMStoreFloat4(&m_vCurvePos, vMiddlePoint);

    m_vLastPos = _vStartPos;
}

_float4 CItem::Bezier(_float4 _vStartPos, _float4 _vCurvePos, _float4 _vEndPos, _float _fTimeDelta)
{
    _float4 vResult;
    XMStoreFloat4(&vResult, ((1 - _fTimeDelta) * (1 - _fTimeDelta) * XMLoadFloat4(&_vStartPos) + 2 * (1 - _fTimeDelta) * _fTimeDelta * XMLoadFloat4(&_vCurvePos) + _fTimeDelta * _fTimeDelta * XMLoadFloat4(&_vEndPos)));

    return  vResult;
}

_float CItem::Compute_LerpItemScale(_float _fStart, _float _fEnd, _float _fRatio)
{
    return _fStart + (_fEnd - _fStart) * _fRatio;
}

void CItem::Reset_ItemState()
{
    m_bAcquired = false;
    m_bStartAcquireEffect = false;
    m_bFinishAcquireEffect = false;
    m_fAcquireEffectTime = 0.f;
    m_fAlphaValue.w = 1.0f;


    m_fEnLargingTime = 0.0f;
    m_bEnLargingDone = false;
    m_bEnLargingDone = false;
    m_bActivate = false;
}

void CItem::Reset_ItemActivate()
{
    m_fDissolveTime = 0.0f;
    m_bActivate = false;
    m_bDissolving = false;
}

void CItem::Set_ColliderRender(_bool _bRender)
{
    if (_bRender)
        m_pGameInstance->Add_Actor_Scene(m_pActor);
    else
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
}


void CItem::Free()
{
    __super::Free();
}
