#pragma once

#include "PartObject.h"

BEGIN(Engine)

class ENGINE_DLL CProjectile abstract : public CPartObject
{
public:
	struct PROJECTILE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* iDamage = {};
		_float* fDelete_Time = {};
	};

protected:
	CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile(const CProjectile& Prototype);
	virtual ~CProjectile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_bool Get_IsFire() { return m_bIsFire; }
	void Set_IsFire(_bool bCheck) { m_bIsFire = bCheck; }
public:
	void Reset_Projectile();
	void Set_Target(_vector vDir);
	virtual void Set_Projectile_Effect();
	virtual void Stop_Projectile_Effect();
protected:
	PxRigidDynamic* m_pActor = { nullptr };
protected:
	_bool m_bIsFire = { false };
	_float m_fDelete_Time = {};

	_float4 m_vStartPos = {};
	_float4 m_vEndPos = {};
	_float4 m_vDir = {};
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END