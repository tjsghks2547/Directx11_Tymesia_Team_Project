#pragma once

#include "Object.h"
#include "Button.h"

BEGIN(Client)

class CStaticObject : public CObject
{
public:
	typedef struct StaticObjectDesc : public CObject::OBJECT_DESC
	{

	}STATIC_OBJECT_DESC;

protected:
	CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStaticObject(const CStaticObject& Prototype);
	virtual ~CStaticObject() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

	HRESULT						Render_Shadow() override;
	virtual HRESULT				Render_Motion_Blur() override;	

protected:
	virtual HRESULT				Ready_Components() override;
	virtual HRESULT				Bind_ShaderResources() override;
	virtual HRESULT				Bind_Motion_Blur_ShaderResources();

protected:
	PxRigidDynamic*				m_pActor = { nullptr };

public:
	static CStaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END