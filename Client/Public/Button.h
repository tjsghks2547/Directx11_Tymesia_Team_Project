#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CButton final : public CUIObject
{
public:
    struct  BUTTON_DESC : public CUIObject::UIOBJECT_DESC
    {
        _uint _iButtonTypeIndex = {};
        const _float4x4* pInteractObjectWorldMatrix = { nullptr };

    };

private:
    CButton(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CButton(const CButton& _Prototype);
    virtual ~CButton() = default;

public:
    virtual HRESULT             Initialize_Prototype() override;
    virtual HRESULT             Initialize(void* _pArg) override;
    virtual void                Priority_Update(_float _fTimeDelta) override;
    virtual void                Update(_float _fTimeDelta) override;
    virtual void                Late_Update(_float _fTimeDelta) override;
    virtual HRESULT             Render() override;

    void                        Activate_Button(_bool _bActivate);
    void                        Set_WorldPosition(_float4 _vPosition);

    void                        Set_ButtonText(const _wstring& _pText, const _wstring& _pDescText);


public:
    HRESULT                     Ready_Components();

private:
    CGameObject* m_pInteractObject = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    const _float4x4* m_pWorldMatrix = { nullptr };
    _float3                     m_fButtonPosition = {};
    _float4                     m_vWorldPosition = {};

    _bool                       m_bActivate = { false };

    _float                      m_fAlphaValue = 0.0f;
    _bool                       m_bFadingIn = { false };
    _bool                       m_bFadingOut = { false };

    wstring                     m_strText = TEXT("E");
    wstring                     m_strDescriptionText = TEXT("ащ╠Б");

    _uint                       m_iButtonTypeNum = {};


public:
    static CButton* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject* Clone(void* _pArg) override;
    virtual void                Free() override;
};
END