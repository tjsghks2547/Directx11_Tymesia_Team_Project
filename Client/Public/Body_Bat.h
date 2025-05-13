#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END

BEGIN(Client)

class CCamera_Free;

class CBody_Bat final : public CPartObject
{
	enum COLLIDER_CATEGORY
	{
		COLLIDER_LEFT_HAND,
		COLLIDER_RIGHT_HAND,
		COLLIDER_MOUTH,
		COLLIDER_BODY,
		COLLIDER_LARGE,
		COLLIDER_WHOLE,
		COLLIDER_END
	};

public:
	struct BODY_BAT_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState = { nullptr };
		_uint* iAttack = { nullptr };
		_bool* bDead = { nullptr };
	};

private:
	CBody_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Bat(const CBody_Bat& Prototype);
	virtual ~CBody_Bat() = default;

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
	const _float4x4* m_pSocketMatrix[3] = { nullptr };
private:
	_bool* m_bDead = {};
	_bool  m_bColliderOff = {};
	_uint m_iPassNum = {};
	const _uint* m_pParentState = {};

	CCamera_Free* m_pCamera = { nullptr };	

	_float m_fFinishTime = {};
	_float m_fDeadTimer = {};
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBody_Bat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END