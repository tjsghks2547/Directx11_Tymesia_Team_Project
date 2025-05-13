#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CModel;
class CShader;
END

BEGIN(Client)
class CLamp final : public CPartObject
{
public:
    struct LAMP_DESC : public CPartObject::PARTOBJECT_DESC
    {
        const _float4x4* pSocketMatrix = { nullptr };
    };

private:
    CLamp(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CLamp(const CLamp& _Prototype);
    virtual ~CLamp() = default;

public:
    virtual HRESULT										Initialize_Prototype() override;
    virtual HRESULT										Initialize(void* _pArg) override;
    virtual void										Priority_Update(_float _fTimeDelta) override;
    virtual void										Update(_float _fTimeDelta) override;
    virtual void										Late_Update(_float _fTimeDelta) override;
    virtual HRESULT										Render() override;
    virtual HRESULT                                     Render_Glow() override;

    void                                                Activate_Lamp(_bool _bActivate) { m_bActivate = _bActivate; }
    void                                                LightUp_Lamp(_bool _bActivate) { m_bLightOn = _bActivate; }

public:
    HRESULT												Ready_Components();
    HRESULT												Bind_ShaderResources();

private:
    CShader*                                            m_pShaderCom = { nullptr };
    CModel*                                             m_pModelCom = { nullptr };
    const _float4x4*                                    m_pSocketMatrix = { nullptr };

private:
    _float                                              m_fAlphaValue = {};
    _bool                                               m_bActivate = { false };
    _bool                                               m_bLightOn = { false };

public:
    static CLamp* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void										Free() override;
};
END