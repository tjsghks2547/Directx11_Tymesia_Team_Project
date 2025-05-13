#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"
#include "Button.h"

BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)

class CAisemy final : public CContainerObject
{
private:
	CAisemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAisemy(const CAisemy& Prototype);
	virtual ~CAisemy() = default;
public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	HRESULT							Ready_Components(void* pArg);
	HRESULT							Ready_PartObjects();
public:
	void							CalCulate_Distance();
	void							Culling();
public:
	void							RootAnimation();
	void							RotateDegree_To_Player();
	void							Rotation_To_Player();
private:
	_float4                          m_vPlayerPos = {};

	_bool                            m_bActive = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_bNeedControl = {};
	_bool                            m_bCulling = {};

	_float                           m_fRotateDegree = {};
	_float                           m_fAngle = {};
	_float                           m_fDelayTime = {};

	_float                           m_fDistance = {};
	_float                           m_fTimeDelta = {};

private:
	const _float4x4*				m_pRootMatrix = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CNavigation*					m_pNavigationCom = { nullptr };
	CState_Machine<CAisemy>*		m_pState_Manager = { nullptr };
	PxRigidDynamic*					m_pActor = { nullptr };
private:
	class CGameObject*				m_pPlayer = { nullptr };
public:
	virtual void					OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void					OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void					OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CAisemy*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;


	class Idle_State : public CStates<CAisemy>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CAisemy* pObject) override;
		void State_Update(_float fTimeDelta, CAisemy* pObject) override;
		void State_Exit(CAisemy* pObject) override;
	};

private:
	CGameObject*					m_pButtonGameObject = { nullptr };
	CButton*						m_pButton = { nullptr };

public:
	class Intro_State : public CStates<CAisemy>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CAisemy* pObject) override;
		void State_Update(_float fTimeDelta, CAisemy* pObject) override;
		void State_Exit(CAisemy* pObject) override;
	};
};

END