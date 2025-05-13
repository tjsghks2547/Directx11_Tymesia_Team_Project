#pragma once

#include "Base.h"

/* ���� ������(aiNode, aiBone, aiAniNode) �� ǥ���Ѵ�. */
/* aiNode :  ������� ���� ������ ǥ�����ش�. ���� ������ ������ ���� ���� ����� ���ŵǾ�� �ϱ� ������.*/

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
	/* const vector<class CBone*>* Bones �� �ϸ� �ȵǴ� ������ ���Ϳ� push back�� �� �� ������ ũ�⺸�� ũ�� ���ʹ�
	�ƿ� �� �����ϰ� �ٽ� �޸� �Ҵ��ؼ� ���ο� �޸� ������ �����ϱ� ������ �����ͷ� ������ �ش� ������ ù�ּ��̱� ������
	ù �ּҰ� nullptr�� �� �� �ִ�.*/

	/* �׿��� ��������! �̿���!*/

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

	/* �θ�������� ǥ���� �ڽĻ�(�����)���� ��������� ǥ���Ѵ�. �� �θ�������� �ڽ��� ���� �����̽� ��� �ǹ� */
	_float4x4          m_TransformationMatrix = {};


	/* �� �� ��� * �θ��� ������� ( �ڽ��� ���ý����̽� ��� * �θ��� ���� �����̽� ��� )  */
	_float4x4          m_CombinedTransformationMatrix = {};

	_float4x4		   m_PreCombinedTransformationMatrix = {};

	_int               m_iParentBoneIndex = { -1 };

	_float4			   m_vCurrentPos = {};




	/* ��Ʈ ��� �̵���� */
	_float4x4		   m_RootMotionMatrix = {};
	_float4x4		   m_pPreRootMatrix = {};


	/* ���̳��� �� */
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