#pragma once

#include "Client_Defines.h"
#include "Monster.h"


BEGIN(Engine)
class CModel;
class CNavigation;
class CTexture;
END

BEGIN(Client)

class CCircus_Balloon final : public CContainerObject
{
private:
	CCircus_Balloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCircus_Balloon(const CCircus_Balloon& Prototype);
	virtual ~CCircus_Balloon() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual HRESULT Render_Fog_Front() override; // ¹°Ã¼ÀÇ ¾Õ¸é ±íÀÌ¸¦ ±â·Ï
	virtual HRESULT Render_Fog_Back() override; // ¹°Ã¼ÀÇ µÞ¸é ±íÀÌ¸¦ ±â·Ï
	virtual HRESULT Render_Fog_Final(ID3D11ShaderResourceView* pNoiseSRV) override; // ¾È°³ ÃÖÁ¾ ·»´õ¸µ
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);

	HRESULT Bind_ShaderResources();

private:
	CShader*						m_pShaderCom = { nullptr };
	CModel*							m_pModelCom = { nullptr };
	CModel*							m_pSecondModelCom = { nullptr };
	CNavigation*					m_pNavigationCom = { nullptr };
	PxRigidDynamic*					m_pActor = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CShader*						m_pFogShaderCom = { nullptr }; // ¾È°³ ·»´õ¸µ¿ë ½¦ÀÌ´õ Ãß°¡
private:
	class CGameObject*				m_pPlayer = { nullptr };
private:
	const _uint*					m_Player_Attack = { nullptr };
	const _uint*					m_Player_Phase = { nullptr };
	const _uint*					m_Player_State = { nullptr };
private:
	_float4                          m_vSpawnPoint = {};

	_bool							 m_bHP_Bar_Active = {};
	_bool							 m_bCanHit = true;
	_bool							 m_bFirstActive = {};
	_bool							 m_bDead = {};

	_uint							 m_iHit_Motion_Index = -1;

	_float                           m_fDistance = {};
	_float                           m_fHP_Bar_Height = {};
	_float                           m_fHP_Bar_Active_Timer = {};
	_float                           m_fMonsterMaxHP = {};
	_float                           m_fMonsterCurHP = {};
	_float                           m_fShieldHP = {};


private:
	_bool							m_bHitted = { false };
	_bool							m_bUpdating = { false };

	_float							m_fWigglingTime = {};
	_float							m_fLifeTime = {};
	_float							m_fInitialJumpPower = {};
	_float							m_fExplosionPowerOffsetValue = {};
	_float							m_fDissolveAmount = {};

	_float                          m_fExplosionPower = {};
	_float                          m_fFallingTime = {};
	_float                          m_fModelHeightCenterY = {};
	_float4							m_vModelPosition = {};

public:
	virtual void					OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void					OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void					OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CCircus_Balloon*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

END