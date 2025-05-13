#include "..\Public\Frustum.h"
#include "GameInstance.h"

CFrustum::CFrustum()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFrustum::Initialize()
{
	m_vOriginal_Points[0] = _float4(-1.f, 1.f, 0.f, 1.f);
	m_vOriginal_Points[1] = _float4(1.f, 1.f, 0.f, 1.f);
	m_vOriginal_Points[2] = _float4(1.f, -1.f, 0.f, 1.f);
	m_vOriginal_Points[3] = _float4(-1.f, -1.f, 0.f, 1.f);

	m_vOriginal_Points[4] = _float4(-1.f, 1.f, 1.f, 1.f);
	m_vOriginal_Points[5] = _float4(1.f, 1.f, 1.f, 1.f);
	m_vOriginal_Points[6] = _float4(1.f, -1.f, 1.f, 1.f);
	m_vOriginal_Points[7] = _float4(-1.f, -1.f, 1.f, 1.f);

	return S_OK;
}

void CFrustum::Update()
{
	_matrix			ViewMatrixInverse = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW);
	_matrix			ProjMatrixInverse = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ);

	for (_uint i = 0; i < 8; i++)
	{
		XMStoreFloat4(&m_vWorld_Points[i], XMVector3TransformCoord(XMLoadFloat4(&m_vOriginal_Points[i]), ProjMatrixInverse));
		XMStoreFloat4(&m_vWorld_Points[i], XMVector3TransformCoord(XMLoadFloat4(&m_vWorld_Points[i]), ViewMatrixInverse));
	}

	Make_Planes(m_vWorld_Points, m_vWorld_Planes);



	_matrix			ViewMatrixInverse_Monster = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW_FRUSTUM); // ���Ϳ����� �ٲ����
	_matrix			ProjMatrixInverse_Monster = m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ_FRUSTUM); // ���Ϳ����� �ٲ����

	for (_uint i = 0; i < 8; i++)
	{
		XMStoreFloat4(&m_vWorld_Points_Monster[i], XMVector3TransformCoord(XMLoadFloat4(&m_vOriginal_Points[i]), ProjMatrixInverse_Monster));
		XMStoreFloat4(&m_vWorld_Points_Monster[i], XMVector3TransformCoord(XMLoadFloat4(&m_vWorld_Points_Monster[i]), ViewMatrixInverse_Monster));
	}

	Make_Planes(m_vWorld_Points_Monster, m_vWorld_Planes_Monster);
}

_bool CFrustum::isIn_WorldSpace(_fvector vWorldPoint, _float fRange, FRUSTUM_TYPE _eType)
{
	/*a b c d
	x y z 1

		ax + by + cz + d = ? ;*/

	switch (_eType)
	{
	case Engine::FRUSTUM_TYPE::FRUSTUM_OBJECT:
		for (_uint i = 0; i < 6; i++)
		{
			if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vWorld_Planes[i]), vWorldPoint)))
				return false;
		}
		break;
	case Engine::FRUSTUM_TYPE::FRUSTUM_MONSTER:
		for (_uint i = 0; i < 6; i++)
		{
			if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_vWorld_Planes_Monster[i]), vWorldPoint)))
				return false;
		}
		break;
	}
	return true;
}

_bool CFrustum::isIn_AABB_Box(const XMFLOAT3& _fMin, const XMFLOAT3& _fMax)
{
	XMFLOAT3 fAdjustMin = XMFLOAT3(_fMin.x - m_fOffSet, _fMin.y - m_fOffSet, _fMin.z - m_fOffSet);
	XMFLOAT3 fAdjustMax = XMFLOAT3(_fMin.x + m_fOffSet, _fMin.y + m_fOffSet, _fMin.z + m_fOffSet);

	XMFLOAT3 fPoints[8] =
	{
		/*XMFLOAT3(_fMin.x, _fMax.y, _fMin.z),
		XMFLOAT3(_fMax.x, _fMax.y, _fMin.z),
		XMFLOAT3(_fMax.x, _fMin.y, _fMin.z),
		XMFLOAT3(_fMin.x, _fMin.y, _fMin.z),

		XMFLOAT3(_fMin.x, _fMax.y, _fMax.z),
		XMFLOAT3(_fMax.x, _fMax.y, _fMax.z),
		XMFLOAT3(_fMax.x, _fMin.y, _fMax.z),
		XMFLOAT3(_fMin.x, _fMin.y, _fMax.z)*/

		XMFLOAT3(fAdjustMin.x, fAdjustMax.y, fAdjustMin.z),
		XMFLOAT3(fAdjustMax.x, fAdjustMax.y, fAdjustMin.z),
		XMFLOAT3(fAdjustMax.x, fAdjustMin.y, fAdjustMin.z),
		XMFLOAT3(fAdjustMin.x, fAdjustMin.y, fAdjustMin.z),

		XMFLOAT3(fAdjustMin.x, fAdjustMax.y, fAdjustMax.z),
		XMFLOAT3(fAdjustMax.x, fAdjustMax.y, fAdjustMax.z),
		XMFLOAT3(fAdjustMax.x, fAdjustMin.y, fAdjustMax.z),
		XMFLOAT3(fAdjustMin.x, fAdjustMin.y, fAdjustMax.z)
	};

	for (_uint i = 0; i < 6; ++i)
	{
		XMVECTOR vPlane = XMLoadFloat4(&m_vWorld_Planes[i]);

		_uint iCulledPointCount = {};

		for (_uint j = 0; j < 8; ++j)
		{
			XMVECTOR vPoint = XMLoadFloat3(&fPoints[j]);
			if (XMVectorGetX(XMPlaneDotCoord(vPlane, vPoint)) > 0.0f)
			{
				iCulledPointCount++;
			}
		}

		if (iCulledPointCount == 8)
			return false;
	}

	return true;
}

void CFrustum::Make_Planes(const _float4* pPoints, _float4* pPlanes)
{
	/* +x */
	XMStoreFloat4(&pPlanes[0], XMPlaneFromPoints(XMLoadFloat4(&pPoints[1]), XMLoadFloat4(&pPoints[5]), XMLoadFloat4(&pPoints[6])));
	/* -x */
	XMStoreFloat4(&pPlanes[1], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[0]),
		XMLoadFloat4(&pPoints[3])
	));

	/* +y */
	XMStoreFloat4(&pPlanes[2], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[5]),
		XMLoadFloat4(&pPoints[1])
	));

	/* -y */
	XMStoreFloat4(&pPlanes[3], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[3]),
		XMLoadFloat4(&pPoints[2]),
		XMLoadFloat4(&pPoints[6])
	));

	/* +z */
	XMStoreFloat4(&pPlanes[4], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[5]),
		XMLoadFloat4(&pPoints[4]),
		XMLoadFloat4(&pPoints[7])
	));

	/* -z */
	XMStoreFloat4(&pPlanes[5], XMPlaneFromPoints(
		XMLoadFloat4(&pPoints[0]),
		XMLoadFloat4(&pPoints[1]),
		XMLoadFloat4(&pPoints[2])
	));

}

CFrustum* CFrustum::Create()
{
	CFrustum* pInstance = new CFrustum();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}

