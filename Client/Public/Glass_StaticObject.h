#pragma once

#include "Object.h"

BEGIN(Client)

class CGlass_StaticObject final : public CObject
{
public:
	typedef struct GlassObjectDesc : public CObject::OBJECT_DESC
	{
	}GLASSOBJECTDESC;

private:
	CGlass_StaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGlass_StaticObject(const CGlass_StaticObject& Prototype);
	virtual ~CGlass_StaticObject() = default;

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
	_uint	m_iGlassObject = { 0 };
	_bool   m_bAllGlass = { false };

public:
	static CGlass_StaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END