#pragma once

/* ����ü�� ������ ������ �� ��������. */
/* �ȿ� �ִ� �ۿ� �ִٿ� ���� �Ǵ��� �� �� �ִ� ���. */

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void Update();

public:
	_bool isIn_WorldSpace(_fvector vWorldPoint, _float fRange, FRUSTUM_TYPE _eType);
	_bool isIn_AABB_Box(const XMFLOAT3& _fMin, const XMFLOAT3& _fMax);
private:
	class CGameInstance* m_pGameInstance = { nullptr };
	_float4					m_vOriginal_Points[8];
	_float4					m_vWorld_Points[8];
	_float4					m_vWorld_Points_Monster[8];
	_float4					m_vWorld_Planes[6];
	_float4					m_vWorld_Planes_Monster[6];

	_float					m_fOffSet = { 10.1f };

private:
	void Make_Planes(const _float4* pPoints, _float4* pPlanes);

public:
	static CFrustum* Create();
	virtual void Free() override;
};

END