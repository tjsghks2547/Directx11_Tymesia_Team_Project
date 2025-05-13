#pragma once

#include "Component.h"

BEGIN(Engine)
class CShader;		

class ENGINE_DLL CTransform final : public CComponent
{
public:
	struct TRANSFORM_DESC
	{
		_float			fSpeedPerSec    = {};	
		_float			fRotationPerSec = {};	
		_float4         fPosition = {};
		_float3         fScaling = {};
	};

	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	const _float4x4* Get_WorldMatrix_Ptr() const {	
		return &m_WorldMatrix;		
	}	

	void Set_WorldMatrix(_float4x4 worldmat) { m_WorldMatrix = worldmat; }		

	/* set �ϳ����������*/
	void Set_MulWorldMatrix(const _float4x4* _Matrix)	
	{
		// _float4x4 -> XMMATRIX ��ȯ �� ���� ����
		XMMATRIX result = XMLoadFloat4x4(_Matrix) * XMLoadFloat4x4(&m_WorldMatrix);	

		// ����� _float4x4�� ��ȯ�Ͽ� m_WorldMatrix�� ����
		XMStoreFloat4x4(&m_WorldMatrix, result);			
	}


	void Set_State(STATE eState, _fvector vState) {
		/*_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		WorldMatrix.r[eState] = vState;
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);*/
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[eState]), vState);		
	}

	_float3 Get_State_UIObj(STATE eState) {

		_float3 fPos = {};
		_uint2			vViewportSize = { 1600,900 };

		XMStoreFloat3(&fPos, XMLoadFloat4x4(&m_WorldMatrix).r[eState]);

		return { fPos.x + (vViewportSize.x * 0.5f), -fPos.y + (vViewportSize.y * 0.5f), fPos.z };
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Go_Straight_NoNavi(_float fTimeDelta);	
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Straight_Astar(_float fTimeDelta, CNavigation* pNavigation);
	void Go_Backward(_float fTimeDelta);
	void Go_Backward_With_Navi(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Left(_float fTimeDelta);
	void Go_Left_Navi(_float fTimeDelta, class CNavigation* pNavigation);
	void Go_Right(_float fTimeDelta);
	void Go_Right_Navi(_float fTimeDelta, class CNavigation* pNavigation);
	void Go_Dir(_fvector vDir, class CNavigation* pNavigation, _float fTimeDelta);
	void Look(_fvector _vDir); //����->������ ���⸸ �ٲ���ϱ⿡ �߰� 03.16
	void LookAt(_fvector vAt);
	void LookAt_Astar(_fvector vAt);
	void Sliding_Move(_float fTimeDelta, class CNavigation* pNavigation, _vector vTargetPos);
	void Sliding_Root_Ani(_float fTimeDelta, class CNavigation* pNavigation, _float MoveSpeed);	
	void Sliding_Monster_Root_Ani(_float fTimeDelta, class CNavigation* pNavigation, _float MoveSpeed);
	
	void Turn(_fvector vAxis, _float fTimeDelta);/* �ǽð� ȸ�� : �� �����Ӹ��� ���� ���¸� �������� ������ �ӵ��� �߰��� ȸ���س�����. */
	void Turn_Navi(_fvector vAxis, _float fTimeDelta, CNavigation* pNavigation);
	
	void Turn_Degree(_fvector vAxis, _float Degree);	
	void Turn_Degree_Navi(_fvector vAxis, _float Degree, CNavigation* pNavigation);
	void Turn_Axis(_fvector vAxis);
	void Rotation(_fvector vAxis, _float fRadians); /* ���� ȸ�� : �׻� �׵���¸� �������� �������� ������ ȸ���س��´�.  */
	void Rotation(_float fX, _float fY, _float fZ);
	void Rotation(_float fX, _float fY, _float fZ, _float fW);
	void Turn_Move(_fvector vAxis, _float fTimeDelta);	
	void Orbit_Move(_fvector vAxis, _float fTimeDelta, _fvector vCenter);	
	void Orbit_Move_Once(_fvector vAxis, _float Radian, _fvector vCenter);	
	_float4x4 Orbit_Move_RelativePos(_fmatrix vWorldMatrix, _fvector vRelativePos, _fvector vAxis, _float Radian, _fvector vCenter);
	
	void Set_State_UIObj(STATE eState, _float2 _fPos);

public:
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);
	

public:
	void Scaling(const _float3& vScale);
	void Set_Speed(_float _fSpeed) { m_fSpeedPerSec = _fSpeed; }
	void Set_RotationSpeed(_float _fSpeed) { m_fRotationPerSec = _fSpeed; }
	_float3 Compute_Scaled();
	

private:
	/* ��ü�� ���� ���¸� ������ �������� ǥ���� ���(right, up, look, position).  */
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = { 0.f };
	_float					m_fRotationPerSec = { 0.f };
	_float4					m_fPosition = {};
	_float3					m_fScaling = {};
	_float3					m_fRotation = {};



public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) { return nullptr; };
	virtual void Free() override;
};

END