#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CMonster_HP_Gage_Effect final : public CPartObject
{
public:
	struct Monster_HP_Gage_Effect_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		_float* fMaxHP = {};
		_float* fCurHP = {};
		_float* fShieldHP = {};
	};
private:
	CMonster_HP_Gage_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_HP_Gage_Effect(const CMonster_HP_Gage_Effect& Prototype);
	virtual ~CMonster_HP_Gage_Effect() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_bool              m_bActive = false;

	_float			   m_fTimeDelta = { 0.f };

	_float* m_fMaxHP = {};
	_float* m_fCurHP = {};
	_float* m_fShieldHP = {};

	_float             m_fPreCurHP = {};
	_float             m_fShieldPreHP = {};


private:
	const _uint* m_pParentState = { nullptr };


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
public:
	static CMonster_HP_Gage_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END