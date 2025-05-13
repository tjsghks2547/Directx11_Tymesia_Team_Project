#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "Button.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CGameItem final : public CItem
{
public:
    struct GAMEITEM_DESC : public CItem::ITEM_DESC
    {
        string                  GameItemName = {};
        ITEM_TYPE               eItemType = { ITEM_TYPE::ITEM_END };
        _uint                   iItemCount = {};
    };

private:
    CGameItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CGameItem(const CGameItem& _Prototype);
    virtual ~CGameItem() = default;

public:
    virtual HRESULT             Initialize_Prototype() override;
    virtual HRESULT             Initialize(void* _pArg) override;
    virtual void                Priority_Update(_float _fTimeDelta) override;
    virtual void                Update(_float _fTimeDelta)/* override*/;
    virtual void                Late_Update(_float fTimeDelta)override;
    virtual HRESULT             Render()override;
    virtual HRESULT             Render_Glow() override;


public:
    virtual HRESULT             Ready_Components();
    virtual HRESULT             Bind_ShaderResources();
    void                        Set_ItemPos(_fvector _vPosition);

    void                        Setting_BillBoard();

public:
    virtual void                OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollisionExit(CGameObject* _pOther, PxContactPair _information);
private:
    CShader*                    m_pShaderCom = { nullptr };
    CTexture*                   m_pTextureCom = { nullptr };
    CTexture*                   m_pNoiseTextureCom = { nullptr };
    CVIBuffer_Rect*             m_pVIBufferCom = { nullptr };

private:
    ITEM_TYPE                   m_eItemType = { ITEM_TYPE::ITEM_END };
    _char		                m_GameItemName[MAX_PATH] = {};

    class CGameObject*          m_pInteractButton = { nullptr };

    CGameObject*                m_pButtonGameObject = { nullptr };
    CButton*                    m_pButton = { nullptr };
    
    CGameObject*                m_pGroupInven = { nullptr };

    _float                      m_fTime = {};
    _uint                       m_iItemTypeNumber = {};


public:
    static CGameItem*           Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject*        Clone(void* _pArg);
    virtual void                Free() override;
};
END