#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CBody_Punch_Man final : public CPartObject
{
public:
	struct BODY_PUNCH_MAN_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		_uint* iAttack = { nullptr };
		_bool* bDead = {};
	};

private:
	CBody_Punch_Man(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Punch_Man(const CBody_Punch_Man& Prototype);
	virtual ~CBody_Punch_Man() = default;

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
	PxRigidDynamic* m_pActor = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
private:
	_bool              m_bColliderOff = {};
	_bool* m_bDead = {};


	_uint			   m_iPassNum = {};
	const _uint* m_pParentState = { nullptr };


	_float			   m_fDeadTimer = {};
	_float			   m_fFinishTime = {};
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBody_Punch_Man* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END