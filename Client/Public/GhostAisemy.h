
#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"
#include "Body_GhostSemy.h"
#include "Lamp.h"
#include "Button.h"

BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)
class CGhostAisemy : public CContainerObject
{
public:
	enum GHOST_SEMY_STATE
	{
		STATE_GOSEMY_APPROACH = 0x00000001,
		STATE_GOSEMY_LIGHT_OFF = 0X00000002,
		STATE_GOSEMY_LIGHT_ON = 0X00000003,
		STATE_GOSEMY_LIGHT_LOOP = 0X00000004,
		STATE_GOSEMY_END = 0x00000005
	};

public:
	struct GHOST_SEMY_DESC : public CGameObject::GAMEOBJECT_DESC
	{

	};

private:
	CGhostAisemy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CGhostAisemy(const CGhostAisemy& _Prototype);
	virtual ~CGhostAisemy() = default;
public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* _pArg) override;
	virtual void					Priority_Update(_float _fTimeDelta) override;
	virtual void					Update(_float _fTimeDelta) override;
	virtual void					Late_Update(_float _fTimeDelta) override;
	virtual HRESULT					Render() override;

	void							Spawn_Gosemy(_float4 _vPos, _bool _bFirstAppear = true);
	void							Spawn_Conversation_Gosemy(_float4 _vPos, _bool _bColliderOn);

	void							Set_AnimState(_uint _iAnimStateNum);

public:
	HRESULT							Ready_Components();
	HRESULT							Ready_PartObjects();
	void							Culling();

public:
	virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

	void							Setting_Approach(_float _fTimeDelta);
	void							Setting_LightOn(_float _fTimeDelta);
	void							Setting_LightOff(_float _fTimeDelta);
	void							Setting_LightLoop(_float _fTimeDelta);

	CLamp* Get_SemyLamp()const { return m_pLamp; }
	CBody_GhostSemy* Get_SemyBody() const { return m_pBody_GhoSemy; }

private:
	_float4                         m_vPlayerPos = {};

	_bool                           m_bActive = {};
	_bool                           m_bNeed_Rotation = {};
	_bool                           m_bNeedControl = {};
	_bool                           m_bCulling = {};
	_bool							m_bColliderOn = { false };


	_float                          m_fRotateDegree = {};
	_float                          m_fAngle = {};
	_float                          m_fDelayTime = {};

	_float                          m_fDistance = {};
	_float                          m_fTimeDelta = {};

private:
	_uint							m_iState = {};
	LEVELID							m_eMyLevel = {};
private:
	CBody_GhostSemy* m_pBody_GhoSemy = { nullptr };
	CLamp* m_pLamp = { nullptr };
private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };

protected:
	CGameObject* m_pButtonGameObject = { nullptr };
	CButton* m_pButton = { nullptr };

public:
	static CGhostAisemy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
