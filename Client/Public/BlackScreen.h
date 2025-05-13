#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CBlackScreen final : public CUIObject
{
public:
    struct BLACKSCREEN_DESC : public CUIObject::UIOBJECT_DESC
    {

    };

private:
    CBlackScreen(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CBlackScreen(const CBlackScreen& _Prototype);
    virtual ~CBlackScreen() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* _pArg)override;

    virtual void                    Priority_Update(_float _fTimeDelta)override;
    virtual void                    Update(_float _fTimeDelta)override;
    virtual void                    Late_Update(_float _fTimeDelta)override;

    virtual HRESULT                 Render()override;

    //void                            Set_Alpha(_float _fAlpha) { m_fAlpha = _fAlpha; }

private:
    CShader* m_pShaderCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    _bool                           m_bIsVisible = { false };
    //_float                          m_fAlpha = {};
public:
    HRESULT                         Ready_Component();
public:
    static CBlackScreen* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void                    Free() override;
};
END
