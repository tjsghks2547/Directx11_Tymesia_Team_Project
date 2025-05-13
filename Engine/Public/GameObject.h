#pragma once

#include "Transform.h"

BEGIN(Engine)

class CModel;

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		_char* szName = {};
		_float4x4 _ParentMatrix = {};
		_float4  _fPosition = {};
		_uint iCurLevel = {};
	};

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() { return S_OK; }
	virtual HRESULT Render_Blur() { return S_OK; }
	virtual HRESULT Render_HighLight() { return S_OK; }
	virtual HRESULT Render_Distortion() { return S_OK; }
	virtual HRESULT Render_Glow() { return S_OK; }
	virtual HRESULT Render_Motion_Blur() { return S_OK; }
	virtual HRESULT Render_Gaussian_Blur() { return S_OK; }
	virtual HRESULT Render_Occulusion() { return S_OK; }
	virtual HRESULT Render_WeightBlend() { return S_OK; }
	virtual HRESULT	Render_Bloom() { return S_OK; }
	virtual HRESULT	Render_Fog_Front() { return S_OK; }
	virtual HRESULT	Render_Fog_Back() { return S_OK; }
	virtual HRESULT	Render_Fog_Final(ID3D11ShaderResourceView* pNoiseSRV) { return S_OK; }
	virtual HRESULT	Render_Reflection() { return S_OK; }


	/* 11월 11일 추가*/
	map<const wstring, class CComponent*>* Get_Components() { return &m_Components; }

	const _char* Get_Name()
	{
		return m_szName;
	}

	void   Set_Name(_char* _szName)
	{
		strcpy_s(m_szName, _szName);
	}


public:
	class CComponent* Find_Component(const _wstring& strComponetTag);
	_uint Get_Player_Hitted_State() const { return m_iPlayer_Hitted_State; }
	_uint Get_Monster_State() const { return m_iMonster_State; }	
	_uint Get_Monster_Execution_Category() const { return m_iMonster_Execution_Category; }
	void Set_Locked_On(_bool pLocked_On) { m_bLocked_On = pLocked_On; }
	CModel* Get_GameObject_Model() { return m_pGameObjectModel; }
	void Set_Object_UV_Pos(_float2 UvPos) { m_fObject_UV_Pos = UvPos; }
	_float2 Get_Object_UV_Pos() { return m_fObject_UV_Pos; }	
	_float* Get_Skill_AttackPower() { return &m_fSkill_AttackPower; }	

protected:
	_char				        m_szName[MAX_PATH] = {};
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CModel* m_pGameObjectModel = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };	

	_uint                       m_iPlayer_Hitted_State = { Player_Hitted_State::PLAYER_HURT_END };
	_uint                       m_iMonster_Execution_Category = { MONSTER_EXECUTION_CATEGORY::MONSTER_START };
	_uint                       m_iMonster_State = { MONSTER_STATE::STATE_END };	

	_float2					    m_fObject_UV_Pos = {};		
	_bool						m_bLocked_On = { false };		

	_float						m_fSkill_AttackPower = {};	

protected:
	map<const _wstring, class CComponent*>		m_Components;	

protected:
	_bool  m_bTriggerOnOff = { false };
	_bool  m_bTriggerEvent2OnOff = { false };

protected:

	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);


public:
	class CTransform* Get_Transfrom() { return m_pTransformCom; }

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information) {};
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information) {};
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information) {};


public:
	virtual void IsTriggerEnter() {};
	virtual void IsTriggerOn(_wstring _EventName) {};
	virtual void IsTriggerExit() {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END