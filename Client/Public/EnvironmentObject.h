
#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"


BEGIN(Engine)
class CModel;
class CShader;
class CMesh;
END

BEGIN(Client)
class CEnvironmentObject abstract : public CGameObject
{
public:
    enum ENV_TYPE {
        ENV_DEFAULT = 0x00000001,
        ENV_EMISSIVE = 0x00000002,
    };

    struct ENVIRONMENT_OBJECT_DESC : public CGameObject::GAMEOBJECT_DESC
    {
        _float fFrustumRadius = { 0.f };
        _float4 fRotation = {};
        string ObjectName = {};
        _float fRange = {};
        _float fSpace = {};
        //_float fPosition= {};

        _float fInstanceCount = {};
        _bool   isBasicMode = { false };

        vector<_float3>             vecInstancePosition = {};
        vector<_float4>             vecInstanceRotation = {};
        vector<_float3>             vecInstanceScale = {};

        vector<VTX_MODEL_INSTANCE>  vecStructModelInstance;
        vector<_int>                vecBoxSize = {};
        _int                        iBoxSize = {  };

        _bool                       isCullingObject = { false };

        _uint                       iPassIndex = { 0 };

        LEVELID                     eLevelID = { LEVEL_TUTORIAL }; // 현재 레벨 확인용

        _uint                       iInstanceType = { ENV_DEFAULT };
    };

public:
    struct EN_OBJECT_INFO
    {
        _char szName[MAX_PATH] = {};
        _float4 fPosition;
        _float3 fScale;
        _float3 fRotation;
        _float fFrustumRadius;
        _int iBoxSize;

        _bool       bCullingObject = { false };
    };

    struct EN_MESH_INFO
    {
        _float fDist;
        _float3 fPosition;
    };

protected:
    CEnvironmentObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CEnvironmentObject(const CEnvironmentObject& _Prototype);
    virtual ~CEnvironmentObject() = default;

protected:
    virtual HRESULT Initialize(void* _pArg) override;
public:

    void            Set_FrustumSphere(_bool _bSphere) {
        m_bFrustumSphere = _bSphere;
    }

protected:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
protected:
    vector<CMesh*> m_vecMeshes;

    _float		   m_fFrustumRadius = { 0.f };
    _float3		   m_fRotation = { 0.f, 0.f, 0.f };
    _float3		   m_fScale = { 0.f, 0.f, 0.f };
    _char		   m_EnvironmentMeshName[MAX_PATH] = {};
    _bool		   m_bFrustumSphere = { true };
    _int           m_iBoxSize = { 1 };


protected:
    vector<_float3>     m_vecInstancePosition = {};
    vector<_float4>     m_vecInstanceRotation = {};
    vector<_float3>     m_vecInstanceScale = {};
    _bool               m_bModeSelected = { false };
    vector<_int>        m_vecBoxSize = {};



    _uint               m_iPassIndex = { 0 };

    _bool               m_bCullingObject = { false };

    LEVELID		        m_eCurrentLevel = { LEVEL_TUTORIAL };

    _uint               m_iInstanceType = { ENV_DEFAULT };

    CGameObject*        m_pCamera = { nullptr };
    _bool*              m_pDitheringOnOff = { nullptr };
protected:
    vector<VTX_MODEL_INSTANCE> m_vecInstanceData;
    _uint m_iNumInstance = {};

public:
    vector<VTX_MODEL_INSTANCE> Get_ModelInstanceVector() { return m_vecInstanceData; }

    //INSTANCE_INFO g_Instances[2] =
    //{
    //    {XMMatrixTranspose(XMMatrixIdentity())},
    //    {XMMatrixTranspose(XMMatrixTranslation(2.0f, 0.0f, 0.0f))}
    //};

protected:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources() PURE;

public:
    virtual CGameObject* Clone(void* _pArg) = 0;
    virtual void Free() override;

};

END
