#pragma once

#include "Object.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)

class CEmissive_StaticObject final : public CObject
{
public:
	typedef struct EmissiveObject : public CObject::OBJECT_DESC
	{

	}EMISSIVEOBJECTDESC;

private:
	CEmissive_StaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEmissive_StaticObject(const CEmissive_StaticObject& Prototype);
	virtual ~CEmissive_StaticObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Bind_ShaderResources() override;

private:
	CTexture* m_pTextureCom = { nullptr };
	_float		m_fEmissivePower = { 15.f };

public:
	static CEmissive_StaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END