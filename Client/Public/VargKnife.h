
#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;


END


BEGIN(Client)

class CCamera_Free;

class CVargKnife final : public CPartObject
{
	enum COLLIDER_CATEGORY
	{
		COLLIDER_SWORD,
		COLLIDER_HAND,
		COLLIDER_STUN,
		COLLIDER_END
	};


public:
	struct VARGKNIFE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* Is_Catch = { nullptr };
		_uint* iAttack = { nullptr };
	};
private:
	CVargKnife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVargKnife(const CVargKnife& Prototype);
	virtual ~CVargKnife() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual _float4x4 Get_HandWorldMatrix() { return m_HandWorldMatrix; }	

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	PxRigidDynamic* m_pActor[COLLIDER_END];

	const _float4x4* m_pSocketMatrix = { nullptr };
	const _float4x4* m_pSocket_Hand_Matrix = { nullptr };

private:
	_bool*			   m_Is_Catch = { nullptr };
	_bool              m_bColliderOff = {};
	_float			   m_fTimeDelta = { 0.f };
	_uint			   m_iPreAnimIndex = -1;
	_float4x4          m_HandWorldMatrix = {};	

	CCamera_Free* m_pCamera = { nullptr };

private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CVargKnife* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END