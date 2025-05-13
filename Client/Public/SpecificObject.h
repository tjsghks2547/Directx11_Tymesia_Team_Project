#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "Button.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CSpecificObject abstract : public CGameObject
{
public:
    struct SpecificObject_Desc : public CGameObject::GAMEOBJECT_DESC
    {
        _float      fFrustumRadius = {};
        _float4		fRotation = { };
        _float3     fScale = {};
        string		ObjectName = {};
        _uint		iPassNum = { 0 };
        _uint       iPairNum = {};
    };

    struct SpecificObject_Info
    {
        _char   szName[MAX_PATH] = {};
        _float4	fPosition;
        _float3 fScale;
        _float4 fRotation;
        _float  fFrustumRadius;
        _uint	iPassNum;
    };

protected:
    CSpecificObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSpecificObject(const CSpecificObject& Prototype);
    virtual ~CSpecificObject() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;
    virtual HRESULT                 Render_Glow() override;

    _bool                           Get_ColliderRender() { return m_bColliderOn; }
    void                            Set_ColliderRender(_bool _bRendering) { m_bColliderOn = _bRendering; }

protected:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    _float		                    m_fFrustumRadius = { 0.f };
    _float4		                    m_fRotation = { 0.f, 0.f, 0.f , 0.0f };
    _float3		                    m_fScale = { 0.f, 0.f, 0.f };
    _float4                         m_fPosition = { 0.0f, 0.0f, 0.0f ,0.0f };
    _char		                    m_MeshName[MAX_PATH] = {};
    _bool		                    m_bFrustumSphere = { true };

    SpecificObject_Info             m_tagSpecificInfo = {};
    SpecificObject_Desc             m_tagDesc = {};


protected:
    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();

protected:
    CGameObject* m_pButtonGameObject = { nullptr };
    CButton* m_pButton = { nullptr };
    _bool						    m_bInteractOn = { false };
    _float						    m_fAlphaValue = {};
    _bool                           m_bFadingIn = { false };
    _bool                           m_bFadingOut = { false };

    _bool                           m_bColliderOn = { false };

    PxRigidDynamic* m_pActor = { nullptr };
    _uint                           m_iCurrentLevel = {};
private:
    // 램프 녀석 어루만져줄 때 쓰는 변수들 모음.
    _bool                           m_bFirstTouch = { false }; // 의자 최초 터치 시 beacon found 알림 팝업 열기 용도 - 유빈

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void                    Free() override;
};
END