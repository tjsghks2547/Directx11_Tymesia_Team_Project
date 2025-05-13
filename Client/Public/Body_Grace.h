#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CBody_Grace final : public CPartObject
{
	enum COLLIDER_CATEGORY { COLLIDER_HAND_RIGHT, COLLIDER_HAND_LEFT, COLLIDER_KICK_LEFT, COLLIDER_KICK_RIGHT, COLLIDER_PARRY, COLLIDER_END };

public:
	struct BODY_GRACE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		_uint* iAttack = { nullptr };
		_bool* bDead = {};
	};

private:
	CBody_Grace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Grace(const CBody_Grace& Prototype);
	virtual ~CBody_Grace() = default;

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
	PxRigidDynamic* m_pActor[COLLIDER_END] = { nullptr };
	const _float4x4* m_pSocketMatrix[COLLIDER_END] = { nullptr };
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
	static CBody_Grace* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END