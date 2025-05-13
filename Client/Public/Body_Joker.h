#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CBody_Joker final : public CPartObject
{
public:
	struct BODY_JOKER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		_bool* bDead = { nullptr };
		_bool* bActive = { nullptr };
	};

private:
	CBody_Joker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Joker(const CBody_Joker& Prototype);
	virtual ~CBody_Joker() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
private:
	const _uint* m_pParentState = { nullptr };
	_uint m_iPassNum = {};
	_float m_fDeadTimer = 1.f;
	_float m_fFinishTime = 1.f;
	_bool* m_bDead = {};
	_bool* m_bActive = {};


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Joker* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END