#pragma once

#include "Base.h"

/* 원근 투영을 위한 뷰, 투영행렬을 저장한다. */
/* 반복적인 갱신을 통해 각 행렬의 역행렬도 구하여 저장한다. */
/* Set, Get함수를 통해 행렬을 저장 또는 리턶나다. */

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

	/* 모션 블러를 위한 이전 프레임의 뷰의 내용 저장 */
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

	/* Zoom 블러를 위한 캐릭터 uv 포지션 저장 */
	void  Set_Zoom_Blur_Center(_float2 _fPos) { m_fZoomBlurCenter = _fPos; }	
	_float2 Get_Zoom_Blur_Center() { return m_fZoomBlurCenter; }	


	// 물에 반사되는 카메라 행렬을 가져오기 위한함수들
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

	/* 모션 블러를  이전 프레임의 뷰, 투영행렬의 역행렬을 구해서 저장한다. */
	void Priority_Update();

	/* 원근 투영을 위한 뷰, 투영행렬의 역행렬을 구해서 저장한다. */
	/* 하는 김에 카메라 위치도 구해놓는다. */
	void Update();

private:
	_float4x4				m_TransformationMatrices[D3DTS_END] = {};
	_float4x4				m_TransformationInverseMatrices[D3DTS_END] = {};

	/* 모션 블러를 위한 이전 뷰 행렬을 저장하는 맴버 변수들 */
	_float4x4				m_PreTransformationMatrices[D3DTS_END] = {};
	_float4x4				m_PreTransformationInverseMatrices[D3DTS_END] = {};

	/* 물에 반사되는 풍경을 그리는 뷰 행렬을 저장하는 맴버 변수들*/

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