#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent{ pDevice, pContext }
{

}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());	

	return S_OK;	
}

HRESULT CTransform::Initialize(void * pArg)
{
	TRANSFORM_DESC*		pDesc = static_cast<TRANSFORM_DESC*>(pArg);

	if (nullptr == pDesc)
		return S_OK;

	m_fSpeedPerSec = pDesc->fSpeedPerSec;
	m_fRotationPerSec = pDesc->fRotationPerSec;	

	if (!(pDesc->fPosition.x == 0.f && pDesc->fPosition.y == 0.f && pDesc->fPosition.z == 0.f))
	{
		m_fPosition = pDesc->fPosition;
		m_fScaling = pDesc->fScaling;

		Scaling(m_fScaling);
		Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_fPosition));	
	}

	return S_OK;
}

void CTransform::Go_Straight_NoNavi(_float fTimeDelta)
{	
	_vector		vPosition = Get_State(STATE_POSITION);	
	_vector		vLook = Get_State(STATE_LOOK);	

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;	

	Set_State(STATE_POSITION, vPosition);	
}

void CTransform::Go_Straight(_float fTimeDelta, class CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_POSITION, vPosition);
		//pNavigation->Set_First(true);
	}

	else if (!pNavigation->isMove(vPosition))
	{
		/* 여기서 해당 방향으로 갈 수 있게 해주기  Dir 을 더해줘서 */

		_vector		vDir = pNavigation->Setting_SlidingMove(vPosition, vLook);

		vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
		vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

		if (pNavigation->isMove(vPosition))
		{
			Set_State(STATE_POSITION, vPosition);
		}

		else
		{
			Go_Backward(fTimeDelta * 0.1f);
			//Set_State(STATE_POSITION, vPosition);
		}
	}
}

void CTransform::Go_Straight_Astar(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector      vPosition = Get_State(STATE_POSITION);
	_vector      vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_POSITION, vPosition);
	}
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward_With_Navi(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);	
	_vector		vLook = Get_State(STATE_LOOK);	

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;	

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))	
	{
		Set_State(STATE_POSITION, vPosition);	
	}
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left_Navi(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_POSITION, vPosition);
	}
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right_Navi(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(STATE_POSITION);	
	_vector		vRight = Get_State(STATE_RIGHT);	

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;	

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))	
	{
		Set_State(STATE_POSITION, vPosition);	
	}
}

void CTransform::Go_Dir(_fvector vDir, class CNavigation* pNavigation, _float fTimeDelta)	
{
	_vector		vPosition = Get_State(STATE_POSITION);


	vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_POSITION, vPosition);
	}
}

void CTransform::Look(_fvector _vDir)
{
	_float3		vScale = Compute_Scaled();

	_vector		vLook = _vDir;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::LookAt(_fvector vAt)
{
	_float3		vScale = Compute_Scaled();

	_vector		vLook = vAt - Get_State(STATE_POSITION);
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);	
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::LookAt_Astar(_fvector vAt)
{
	_float3		vScale = Compute_Scaled();

	_vector		vLook = vAt;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Sliding_Move(_float fTimeDelta, CNavigation* pNavigation, _vector vTargetPos)
{
	_vector vMyLook = XMVector3Normalize(Get_State(CTransform::STATE_LOOK));
	_vector vNoramlVec = XMVector3Normalize(Get_State(CTransform::STATE_POSITION) - vTargetPos);

	if (XMVectorGetX(XMVector3Dot(vMyLook, vNoramlVec)) < 0.f)
	{
		_vector vProj = vNoramlVec * XMVectorGetX(XMVector3Dot(vMyLook, vNoramlVec));
		_vector vSlider = vMyLook - vProj;

		Go_Backward_With_Navi(fTimeDelta * 0.075f, pNavigation);
		Go_Dir(vSlider, pNavigation, fTimeDelta * 0.075f);
	}
}

void CTransform::Sliding_Root_Ani(_float fTimeDelta, CNavigation* pNavigation, _float MoveSpeed)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * MoveSpeed;

	if (!pNavigation->isMove(vPosition))
	{
		/* 여기서 해당 방향으로 갈 수 있게 해주기  Dir 을 더해줘서 */

		_vector		vDir = pNavigation->Setting_SlidingMove(vPosition, vLook);

		vPosition -= XMVector3Normalize(vLook) * MoveSpeed;
		vPosition += XMVector3Normalize(vDir) * MoveSpeed * 0.5f;

		if (pNavigation->isMove(vPosition))
		{
			Set_State(STATE_POSITION, vPosition);
		}

		else
		{
			//Go_Backward(fTimeDelta * 0.1f);
			//Set_State(STATE_POSITION, vPosition);
		}
	}
}

void CTransform::Sliding_Monster_Root_Ani(_float fTimeDelta, CNavigation* pNavigation, _float MoveSpeed)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK) * -1.f;

	vPosition += XMVector3Normalize(vLook) * MoveSpeed;

	if (!pNavigation->isMove(vPosition))
	{
		/* 여기서 해당 방향으로 갈 수 있게 해주기  Dir 을 더해줘서 */

		_vector		vDir = pNavigation->Setting_SlidingMove(vPosition, vLook);

		vPosition -= XMVector3Normalize(vLook) * MoveSpeed;
		vPosition += XMVector3Normalize(vDir) * MoveSpeed * 0.5f;

		if (pNavigation->isMove(vPosition))
		{
			Set_State(STATE_POSITION, vPosition);
		}
	}
}


void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector			vRight = Get_State(CTransform::STATE_RIGHT);
	_vector			vUp = Get_State(CTransform::STATE_UP);
	_vector			vLook = Get_State(CTransform::STATE_LOOK);

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Turn_Navi(_fvector vAxis, _float fTimeDelta, CNavigation* pNavigation)
{
	_vector			vRight = Get_State(CTransform::STATE_RIGHT);	
	_vector			vUp = Get_State(CTransform::STATE_UP);		
	_vector			vLook = Get_State(CTransform::STATE_LOOK);	

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);		

	_vector		vPosition = Get_State(STATE_POSITION);
	
	/* 그럼 여기서 예외조건을 처리해야겠네. */
	

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));	
	}
}


void CTransform::Turn_Degree(_fvector vAxis, _float Degree)
{

	_vector			vRight = Get_State(CTransform::STATE_RIGHT);
	_vector			vUp = Get_State(CTransform::STATE_UP);
	_vector			vLook = Get_State(CTransform::STATE_LOOK);

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Degree);		

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}


void CTransform::Turn_Degree_Navi(_fvector vAxis, _float Degree, CNavigation* pNavigation)	
{

	_vector			vRight = Get_State(CTransform::STATE_RIGHT);
	_vector			vUp = Get_State(CTransform::STATE_UP);
	_vector			vLook = Get_State(CTransform::STATE_LOOK);

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Degree);


	_vector		vPosition = Get_State(STATE_POSITION);	

	if (nullptr == pNavigation || true == pNavigation->isMove(vPosition))
	{
		Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
		Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
		Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
	}
}



void CTransform::Turn_Axis(_fvector vAxis)
{
	_vector			vRight = Get_State(CTransform::STATE_RIGHT);	
	_vector			vUp = Get_State(CTransform::STATE_UP);	
	_vector			vLook = Get_State(CTransform::STATE_LOOK);	
	
	
	
}

void CTransform::Turn_Move(_fvector vAxis, _float fTimeDelta)
{

	_vector			vPosition = Get_State(CTransform::STATE_POSITION);	
	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);


	Set_State(STATE_POSITION, XMVector3TransformCoord(vPosition, RotationMatrix));
}

void CTransform::Orbit_Move(_fvector vAxis, _float fTimeDelta, _fvector vCenter)	
{
	_vector			vPosition = Get_State(CTransform::STATE_POSITION);

	_vector         vRelativePos = vPosition - vCenter;	

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);	


	_vector         vNewCamPos =   XMVectorSetW(vCenter + XMVector3TransformCoord(vRelativePos, RotationMatrix),1.f);

	Set_State(STATE_POSITION, vNewCamPos);
}

void CTransform::Orbit_Move_Once(_fvector vAxis, _float Radian, _fvector vCenter)
{
	_vector			vPosition = Get_State(CTransform::STATE_POSITION);	
	_vector         vRelativePos = vPosition - vCenter;	
	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);	


	_vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformCoord(vRelativePos, RotationMatrix), 1.f);	

	Set_State(STATE_POSITION, vNewCamPos);	
}

_float4x4 CTransform::Orbit_Move_RelativePos(_fmatrix vWorldMatrix, _fvector vRelativePos, _fvector vAxis, _float Radian, _fvector vCenter)
{
	_vector			vRight = vWorldMatrix.r[0];
	_vector			vUp = vWorldMatrix.r[1];
	_vector			vLook = vWorldMatrix.r[2];

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	_vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformCoord(vRelativePos, RotationMatrix), 1.f);

	_matrix  ConvertedMatrix = XMMatrixIdentity();

	ConvertedMatrix.r[0] = vRight;
	ConvertedMatrix.r[1] = vUp;
	ConvertedMatrix.r[2] = vLook;
	ConvertedMatrix.r[3] = vNewCamPos;

	_float4x4  finalConvertedMatrix = {};
	XMStoreFloat4x4(&finalConvertedMatrix, ConvertedMatrix);

	return  finalConvertedMatrix;
}

void CTransform::Set_State_UIObj(STATE eState, _float2 _fPos)
{
	_uint2			vViewportSize = { 1600,900 };
	_float fZ = Get_State_UIObj(CTransform::STATE_POSITION).z;
	XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[eState]), XMVectorSet(_fPos.x - (vViewportSize.x * 0.5f), -_fPos.y + (vViewportSize.y * 0.5f), fZ, 1.f));
}

void CTransform::Rotation(_fvector vAxis, _float fRadians)
{
	_float3			vScaled = Compute_Scaled();

	_vector			vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector			vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, fRadians);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Rotation(_float fX, _float fY, _float fZ)
{
	_float3			vScaled = Compute_Scaled();

	_vector			vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector			vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_vector		vQuaternion = XMQuaternionRotationRollPitchYaw(fX, fY, fZ);

	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Rotation(_float fX, _float fY, _float fZ, _float fW)
{
	_float3			vScaled = Compute_Scaled();

	_vector			vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector			vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_vector		vQuaternion = XMVectorSet(fX, fY, fZ, fW);

	_matrix		RotationMatrix = XMMatrixRotationQuaternion(vQuaternion);

	Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

HRESULT CTransform::Bind_ShaderResource(CShader * pShader, const _char * pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

void CTransform::Scaling(const _float3 & vScale)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);

}

_float3 CTransform::Compute_Scaled()
{
	return _float3(		
		XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))), 
		XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)))
	);
}

CTransform * CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
