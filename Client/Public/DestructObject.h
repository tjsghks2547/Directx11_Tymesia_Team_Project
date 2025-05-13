#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CDestructObject final : public CGameObject
{
public:
    struct DestructObject_Desc : public CGameObject::GAMEOBJECT_DESC
    {
        _float      fFrustumRadius = {};
        _float4		fRotation = { };
        _float3     fScale = {};
        string		ObjectName = {};
        _uint		iPassNum = { 0 };
        _uint       iPairNum = {};
    };

    struct DestructObject_Info
    {
        _char   szName[MAX_PATH] = {};
        _float4	fPosition;
        _float3 fScale;
        _float4 fRotation;
        _float  fFrustumRadius;
        _uint	iPassNum;
    };

private:
    CDestructObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CDestructObject(const CDestructObject& Prototype);
    virtual ~CDestructObject() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    _float		                    m_fFrustumRadius = { 0.f };
    //_float4		                    m_fRotation = { 0.f, 0.f, 0.f , 0.0f };
    //_float3		                    m_fScale = { 0.f, 0.f, 0.f };
    //_float4                         m_fPosition = { 0.0f, 0.0f, 0.0f ,0.0f };
    //_char		                    m_MeshName[MAX_PATH] = {};
    _bool		                    m_bFrustumSphere = { true };

    DestructObject_Info             m_tagSpecificInfo = {};
    DestructObject_Desc             m_tagDesc = {};


public:
    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();


public:
    virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);


private:
    PxRigidDynamic*                 m_pActor = { nullptr };
    _uint                           m_iCurrentLevel = {};

private:
    _char				            m_szIndivisualName[MAX_PATH] = {};

private:
    // 지메쉐에서 모델의 좌표를 던져서 4등분이든 8등분이든 나누기위해서
    // 중심점이랑, m_fModelPos
    // 모델의 높이점( 보통 posY + 블렌더에서 말하는 비율로 하니까 맞는듯 ㅇㄱㄹㅇ;;)  m_fModelHeightCenterY
    // 모델의 밑바닥 y지점 ( 중력으로 이제 떨구는데, 파편 스탑잇 해야하는 위치를 알아야해서..;;) m_fModelBottomValueY
    // 터지는 힘의 크기 (맞을 때, 얼마나 작용할지 ) m_fExplosionPower
    // 떨어질 때 작용되는 중력의 작용시간 m_fFallingTime

    _float4                         m_fModelPos = {};
    _float                          m_fModelHeightCenterY = {};
    _float                          m_fModelBottomValueY = {};
    _float                          m_fExplosionPower = {};
    _float                          m_fFallingTime = {};
    _float                          m_fLifeTime = {};

    _bool                           m_bHitted = { false };
    _bool                           m_bUpdating = { false };
    _bool                           m_bDropped = { false };

    _float                          m_fDissolveAmount = { };
public:
    static CDestructObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void                    Free() override;
};
END