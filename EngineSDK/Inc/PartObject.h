#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	struct PARTOBJECT_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		const _float4x4* pParentWorldMatrix; 
		CGameObject* pParent;
	};

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default; 
public:
	_uint* Get_Monster_Attack_Ptr()const { return m_iMonster_Attack; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render(); 
	virtual const _float4x4* Get_ParentWorldMatrix() { return m_pParentWorldMatrix; }
	virtual CGameObject* Get_Parent_Ptr() { return m_pParent; }

	_float4x4 Get_CombineWorldMatrix() { return m_CombinedWorldMatrix; }

protected:
	CGameObject* m_pParent	= { nullptr };
	const _float4x4*				m_pParentWorldMatrix = { nullptr };
	_float4x4						m_CombinedWorldMatrix;
	/* 루트 모션 때매 추가 */
	_float4x4*						m_MotionWorldMatrix = { nullptr };
	_uint*                          m_iMonster_Attack = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0; 
	virtual void Free() override;
};

END