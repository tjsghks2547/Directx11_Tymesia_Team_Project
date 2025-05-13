#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffect abstract : public CGameObject
{
public:
	struct EFFECT_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		//EFFECT_TYPE eType = { EFFECT_TYPE::EFFECT_TYPE_END };
		_uint iDiffuse = {};
		_float fMaxTimer = {};
	};
protected:
	CEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect(const CEffect& _Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* _pArg);
	virtual void Priority_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_IsPlaying(_bool _bIsPlaying);

public:

	_bool Get_IsPlaying() { return m_bIsPlaying; }
	
	void Set_MaxTimer(_float _fMaxTimer) { m_fMaxTimer = _fMaxTimer; }
	void Set_SettingMatrix(const _float4x4* _pSettingMatrix) { m_pSettingMatrix = _pSettingMatrix; }
	void Set_SocketMatrix(const _float4x4* _pSocketMatrix) { m_pSocketMatrix = _pSocketMatrix; }
	void Clear_Setting();
	void Set_Direction(_fvector _vDir);
	void Set_Pos_With_Matrix(_float4x4 _matWorld);

	void Set_Animation_Speed(const _float* _pAnimation_Speed);

protected:
	//EFFECT_TYPE m_eEffectType = { EFFECT_TYPE::EFFECT_TYPE_END };
	const _float4x4* m_pSettingMatrix = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	const _float* m_pAnimation_Speed = { nullptr };

	_float4x4 m_matCombined = {};
	_float4x4 m_matParentWorld = {};

	_bool m_bIsPlaying = { false };

	_float m_fMaxTimer = {};
	_float m_fDissolve = {}; //Dissolve Amount юс

	_float m_fTimerX = {};
	_float m_fTimerY = {};

	_float m_fTimer_SpeedX = { 1.f };
	_float m_fTimer_SpeedY = { 1.f };
	_float m_fDissolve_Speed = { 1.f };

	_float m_fTimer_Timelag = {};

protected:
	void Timer_Check(_float _fTimeDelta);

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END