#pragma once

#include "Base.h"

/* 모델의 뼈정보(aiNode, aiBone, aiAniNode) 를 표현한다. */
/* aiNode :  뼈대들의 계층 구조를 표현해준다. 계층 구조의 순서에 의해 뼈의 행렬이 갱신되어야 하기 때문에.*/

BEGIN(Engine)

class CGameInstance;

class ENGINE_DLL CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	const _char* Get_Name() const
	{
		return m_szName;
	}

	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	const _float4x4* Get_CombinedTransformationPtr() const {
		return &m_CombinedTransformationMatrix;
	}

	const _float4x4* Get_CombinedRootMotionTransformationPtr() const {
		return &m_RootMotionMatrix;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {

		//if (!strcmp(m_szName, "Bip001-Xtra05Opp"))
		//	int a = 4;

		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}


	void Set_CombineTransformationMatrix(_fmatrix TransformationMatrix) {

		//if (!strcmp(m_szName, "Bip001-Xtra05Opp"))
		//	int a = 4;	

		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix);
	}


public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix, _float fTimeDelta);
	/* const vector<class CBone*>* Bones 로 하면 안되는 이유가 벡터에 push back을 할 때 벡터의 크기보다 크면 벡터는
	아예 다 삭제하고 다시 메모리 할당해서 새로운 메모리 구조를 형성하기 때문에 포인터로 받으면 해당 벡터의 첫주소이기 때문에
	첫 주소가 nullptr이 될 수 있다.*/

	/* 네오플 면접문제! 이였음!*/

	void  Set_Dynamic_Bone(_bool _bUpdateOnOff)
	{
		m_BoneType = BONE_TYPE::DYNAMIC_BONE;
		m_bUpdateOnOff = _bUpdateOnOff;
	}
	BONE_TYPE Get_Bone_Type() { return m_BoneType; }
	_bool     Get_Bone_UpdateOnOff() { return m_bUpdateOnOff; }

	_float4x4* Get_Bone_Martix() { return &m_TransformationMatrix; }

private:
	_char		       m_szName[MAX_PATH] = {};

	/* 부모기준으로 표현된 자식뼈(현재뼈)만의 행렬정보를 표현한다. 즉 부모기준으로 자식은 로컬 스페이스 라는 의미 */
	_float4x4          m_TransformationMatrix = {};


	/* 내 뼈 행렬 * 부모의 최종행렬 ( 자식의 로컬스페이스 행렬 * 부모의 월드 스페이스 행렬 )  */
	_float4x4          m_CombinedTransformationMatrix = {};

	_float4x4		   m_PreCombinedTransformationMatrix = {};

	_int               m_iParentBoneIndex = { -1 };

	_float4			   m_vCurrentPos = {};




	/* 루트 모션 이동행렬 */
	_float4x4		   m_RootMotionMatrix = {};
	_float4x4		   m_pPreRootMatrix = {};


	/* 다이나믹 본 */
	BONE_TYPE           m_BoneType = { BONE };
	_float4			    m_vVelocity = {};
	_float4x4		    m_DynamicBoneOffsetMartix = {};
	_float3 		    m_vBindOffset = {};
	_float4			    m_fBindOffset = {};
	_bool			    m_bDynamicInitialized = { true };
	_bool				m_bUpdateOnOff = { false };

	_float              m_fDistanceWithParent = {};
	_float4x4			m_matrixFirstCombine = {};
	/* ============================================= */

public:
	HRESULT Save_Bone(ostream& os);
	HRESULT Load_Bone(istream& os);

	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	static CBone* LoadCreate(std::istream& os);

	CBone* Clone();
	virtual void Free() override;

};

END