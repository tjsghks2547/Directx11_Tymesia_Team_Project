#pragma once
#include "Client_Defines.h"
#include "Item.h"
#include "Button.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)
class CDeadBranch final : public CItem
{
public:
    struct DEADBRANCH_DESC : public CItem::ITEM_DESC
    {
        string                  GameItemName = {};
        ITEM_TYPE               eItemType = { ITEM_TYPE::ITEM_END };
        _uint                   iItemCount = {};
    };

private:
    CDeadBranch(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CDeadBranch(const CDeadBranch& _Prototype);
    virtual ~CDeadBranch() = default;

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

public:
    virtual void                OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

private:
    CShader*                    m_pShaderCom = { nullptr };
    CModel*                     m_pModelCom = { nullptr };
    PxRigidDynamic*             m_pActor = { nullptr };
    CTexture*                   m_pTextureCom = { nullptr };

private:
    CGameObject*                m_pButtonGameObject = { nullptr };
    CButton*                    m_pButton = { nullptr };

private:
    _float                      m_fBranchAlpha = {};
private:
    _uint                       m_pMyLevel = {}; // 어디 레벨에서 생성된건지 저장
public:
    static CDeadBranch*         Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual CGameObject*        Clone(void* _pArg);
    virtual void                Free() override;
};
END