#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


BEGIN(Client)

class CWeapon_Magician_Sword final : public CPartObject
{
	enum COLLIDER_CATEGORY
	{
		COLLIDER_SWORD,
		COLLIDER_KICK,
		COLLIDER_SLASH,
		COLLIDER_CATCH,
		COLLIDER_BURST,
		COLLIDER_END
	};

	enum eDissolveState
	{
		DISSOLVE_NONE,           // 아무것도 안 하는 상태
		DISSOLVE_ON,    // 사라지는 중
		DISSOLVE_OFF    // 나타나는 중
	};
public:
	struct MAGICIAN_SWORD_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* IsDissolveOn = { nullptr };
		_bool* IsDissolveOff = { nullptr };
		_bool* bRender = { nullptr };
		_bool* Is_Change_Sword_Bone = { nullptr };
		_bool* bCane_Collider_On = { nullptr };
		_bool* bSpecial_Skill_Progress = { nullptr };
		_bool* bCatch_Special_Attack = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CWeapon_Magician_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Magician_Sword(const CWeapon_Magician_Sword& Prototype);
	virtual ~CWeapon_Magician_Sword() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	PxRigidDynamic* m_pActor[COLLIDER_END];

	const _float4x4* m_pSocket_Right_Matrix = { nullptr };
	const _float4x4* m_pSocket_Left_Matrix = { nullptr };
	const _float4x4* m_pSocket_Leg_Matrix = { nullptr };

private:
	_bool* m_IsDissolveOn = { nullptr };
	_bool* m_IsDissolveOff = { nullptr };
	_bool* m_bRender = { nullptr };
	_bool* m_Is_Change_Sword_Bone = { nullptr };
	_bool* m_bCane_Collider_On = { nullptr };
	_bool* m_bSpecial_Skill_Progress = { nullptr };
	_bool* m_bCatch_Special_Attack = { nullptr };
	_bool  m_bColliderOff = {};
private:
	eDissolveState m_eDissolveState = DISSOLVE_NONE;
	_float m_fDissolveTimer = 0.f; // 공용으로 사용
	_uint m_iPassNum = {};
private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CWeapon_Magician_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END