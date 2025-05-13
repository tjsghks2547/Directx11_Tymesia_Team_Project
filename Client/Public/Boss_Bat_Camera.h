#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
END

BEGIN(Client)

class CCamera_Free;
class CPlayer;

class CBoss_Bat_Camera final : public CPartObject
{
public:
	struct CAMERA_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		PxRigidDynamic* pParentActor = { nullptr };
		CPlayer* pPlayer = { nullptr };
	};

private:
	CBoss_Bat_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Bat_Camera(const CBoss_Bat_Camera& Prototype);
	virtual ~CBoss_Bat_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	float			   m_AccColliderLifeAttack1 = { 0.f };
	float			   m_AccColliderLifeAttack2 = { 0.f };

	PxRigidDynamic* m_pActor = { nullptr };
	PxRigidDynamic* m_pParentActor = { nullptr };
private:
	_float			   m_fTimeDelta = { 0.f };
	_float			   m_fHitStopTime = { 0.f };

	_uint m_iCurrentLevel = {}; //종한 추가 Level전환때문에

private:
	const _uint* m_pParentState = { nullptr };
	_uint		      m_iPreParentState = { };
	class CPlayer* m_pPlayer = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static  CBoss_Bat_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END