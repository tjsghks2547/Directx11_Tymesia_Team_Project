#pragma once

#include "Base.h"

BEGIN(Engine)

class CEffect;


class CEffectMgr final : public CBase
{
private:
	CEffectMgr();
	virtual ~CEffectMgr() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float _fTimeDelta);
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

public:
	HRESULT Add_Effect(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, EFFECT_NAME _eEffectName, void* _pArg);
	HRESULT Play_Effect(EFFECT_NAME _eEffectName, _fvector _vPos);
	HRESULT Play_Effect_With_Timer(EFFECT_NAME _eEffectName, _float _fDuration, _fvector _vPos);

	HRESULT Play_Effect_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix);
	HRESULT Play_Effect_Speed_Matrix(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float* _pAnimation_Speed);
	HRESULT Play_Effect_With_Timer_Matrix(EFFECT_NAME _eEffectName, _float _fDuration, const _float4x4* _pMatrix);
	HRESULT Play_Effect_Matrix_With_Socket(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix);
	HRESULT Play_Effect_Matrix_With_Socket_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, const _float4x4* _pSocketMatrix, _fvector _vDir);

	HRESULT Play_Effect_Dir(EFFECT_NAME _eEffectName, _fvector _vPos, _fvector _vDir);
	HRESULT Play_Effect_Matrix_Dir(EFFECT_NAME _eEffectName, const _float4x4* _pMatrix, _fvector _vDir);
	HRESULT Play_Effect_Matrix_OneMoment(EFFECT_NAME _eEffectName, _float4x4 _matMatrix);

	HRESULT Stop_Effect(EFFECT_NAME _eEffectName);

	HRESULT Reset_Effect();

private:
	vector<vector<CEffect*>>	m_vecEffect; //Effect 저장용 누가누가 안쓰고있나 Check 용도
	deque<CEffect*>				m_dequePlayingEffect; //Effect를 진짜 출력해야할놈은 여기로 넣어서 Update 돌리는 녀석 알아서 빠져나갈것임
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CEffectMgr* Create();
	virtual void Free() override;
};

END