#pragma once

#include "Base.h"

/* ���� ������ ���� ��, ��������� �����Ѵ�. */
/* �ݺ����� ������ ���� �� ����� ����ĵ� ���Ͽ� �����Ѵ�. */
/* Set, Get�Լ��� ���� ����� ���� �Ǵ� ���t����. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE
	{
		D3DTS_VIEW,
		D3DTS_PROJ,
		D3DTS_VIEW_FRUSTUM,
		D3DTS_PROJ_FRUSTUM,
		D3DTS_END
	};
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	_float4x4 Get_Transform_Float4x4(D3DTRANSFORMSTATE eState) const {
		return m_TransformationMatrices[eState];
	}
	_matrix Get_Transform_Matrix(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_TransformationMatrices[eState]);
	}
	_float4x4 Get_Transform_Float4x4_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_TransformationInverseMatrices[eState];
	}
	_matrix Get_Transform_Matrix_Inverse(D3DTRANSFORMSTATE eState)const {
		return XMLoadFloat4x4(&m_TransformationInverseMatrices[eState]);
	}

	/* ��� ���� ���� ���� �������� ���� ���� ���� */
	_float4x4 Get_PreTransform_Float4x4(D3DTRANSFORMSTATE eState) const {
		return m_PreTransformationMatrices[eState];
	}
	_matrix Get_PreTransform_Matrix(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_PreTransformationMatrices[eState]);
	}
	_float4x4 Get_PreTransform_Float4x4_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_PreTransformationInverseMatrices[eState];
	}
	_matrix Get_PreTransform_Matrix_Inverse(D3DTRANSFORMSTATE eState)const {
		return XMLoadFloat4x4(&m_PreTransformationInverseMatrices[eState]);
	}


	_float4 Get_CamPosition() const {
		return m_vCamPosition;
	}

	/* Zoom ���� ���� ĳ���� uv ������ ���� */
	void  Set_Zoom_Blur_Center(_float2 _fPos) { m_fZoomBlurCenter = _fPos; }	
	_float2 Get_Zoom_Blur_Center() { return m_fZoomBlurCenter; }	


	// ���� �ݻ�Ǵ� ī�޶� ����� �������� �����Լ���
	_float4x4 Get_Reflection_Transform_Float4x4(D3DTRANSFORMSTATE eState) const {
		return m_ReflectionTransformationMatrices[eState];
	}
	_matrix Get_Reflection_Transform_Matrix(D3DTRANSFORMSTATE eState) const {
		return XMLoadFloat4x4(&m_ReflectionTransformationMatrices[eState]);
	}
	_float4x4 Get_Reflection_Transform_Float4x4_Inverse(D3DTRANSFORMSTATE eState) const {
		return m_ReflectionTransformationInverseMatrices[eState];
	}
	_matrix Get_Reflection_Transform_Matrix_Inverse(D3DTRANSFORMSTATE eState)const {
		return XMLoadFloat4x4(&m_ReflectionTransformationInverseMatrices[eState]);
	}
	_float4 Get_Reflection_CamPosition() const {
		return m_vReflectionCamPosition;
	}

public:
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrices[eState], TransformMatrix);
	}

	void Set_Reflection_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) {
		XMStoreFloat4x4(&m_ReflectionTransformationMatrices[eState], TransformMatrix);
	}

public:
	HRESULT Initialize();

	/* ��� ����  ���� �������� ��, ��������� ������� ���ؼ� �����Ѵ�. */
	void Priority_Update();

	/* ���� ������ ���� ��, ��������� ������� ���ؼ� �����Ѵ�. */
	/* �ϴ� �迡 ī�޶� ��ġ�� ���س��´�. */
	void Update();

private:
	_float4x4				m_TransformationMatrices[D3DTS_END] = {};
	_float4x4				m_TransformationInverseMatrices[D3DTS_END] = {};

	/* ��� ���� ���� ���� �� ����� �����ϴ� �ɹ� ������ */
	_float4x4				m_PreTransformationMatrices[D3DTS_END] = {};
	_float4x4				m_PreTransformationInverseMatrices[D3DTS_END] = {};

	/* ���� �ݻ�Ǵ� ǳ���� �׸��� �� ����� �����ϴ� �ɹ� ������*/

	_float4x4				m_ReflectionTransformationMatrices[D3DTS_END] = {};
	_float4x4				m_ReflectionTransformationInverseMatrices[D3DTS_END] = {};
	_float4					m_vReflectionCamPosition = {};



	_float4					m_vCamPosition = {};

	_float2					m_fZoomBlurCenter = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

END