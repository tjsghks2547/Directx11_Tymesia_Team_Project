#include "Bone.h"
#include "GameInstance.h"

CBone::CBone()
{


}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->mName.data); /* data를 붙여준 이유는 char로 반환해주기 때문에 data를 붙여줌*/


	/*pAINode->mTransformation**은 해당 노드의 로컬 변환 행렬*/
	/*지금 받아오는 노드는 루트노드라서 단위행렬이 맞음 (왜냐하면 루트노드의 부모가 없으니깐) */
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	/* 자료형이 다르기 때문에 대입이 안됨 그러나 안에 들어있는 자료형은 float으로 같기 때문에 복사를 이용해 메모리 크기만큼 복사를 진행*/

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	/* 전치행렬을 해주는 이유는 Assimp에서 행렬은 column 우선 즉 열 우선 방식으로 저장하므로
		_11, _21, _31, _41 순서로 저장된다 우리는 major 우선 방식이므로  _11 , _12 , _13, _14 로 순서로 저장해야 되기 때문에
		전치행렬을 적용시켰다.*/

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	/* 부모의 처음 뼈를 저장해야 함 즉 블랜더의 edit 모드일때. */

	/* 처음 부모와의 거리 저장해놓자 */


	/* 그럼 T포즈일 때의 뼈 기준으로 정해 놔야 하는데. */

	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix, _float fTimeDelta)
{

	if (-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, PreTransformMatrix * XMLoadFloat4x4(&m_TransformationMatrix));



	else
	{
		if (m_BoneType == DYNAMIC_BONE)
		{
			if (m_bDynamicInitialized)
			{
				XMStoreFloat4x4(&m_CombinedTransformationMatrix,
					XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

				// 초기 거리 // 
				m_fDistanceWithParent = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3] - XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix).r[3]));

				/* 여기가 본격적인 계산 작업 */
				XMStoreFloat4x4(&m_CombinedTransformationMatrix,
					XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

				/* 초기 */
				XMStoreFloat4x4(&m_matrixFirstCombine, XMLoadFloat4x4(&m_CombinedTransformationMatrix));


				m_bDynamicInitialized = false;
			}


			XMStoreFloat4x4(&m_PreCombinedTransformationMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));	// 이전 컴바인 저장		

			/* 여기가 본격적인 계산 작업 */
			XMStoreFloat4x4(&m_CombinedTransformationMatrix,
				XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

			_matrix test = {};
			test.r[0] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[0];
			test.r[1] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[1];
			test.r[2] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[2];


			XMVECTOR gravity = XMVectorSet(0.f, -9.8f, 0.f, 0.f);

			/* Verlet 적분 */
			_vector vNext = XMLoadFloat4x4(&m_matrixFirstCombine).r[3] + (XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3] - XMLoadFloat4x4(&m_matrixFirstCombine).r[3]) * 0.1f + gravity * fTimeDelta * fTimeDelta;// *fTimeDelta;	;	

			_vector delta = vNext - XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix).r[3];
			float dist = XMVectorGetX(XMVector3Length(delta));

			if (dist > 10.f)
			{
				XMVECTOR dir = XMVector3Normalize(delta);
				float diff = dist - m_fDistanceWithParent;
				vNext -= dir * diff;
			}

			m_matrixFirstCombine._41 = vNext.m128_f32[0];
			m_matrixFirstCombine._42 = vNext.m128_f32[1];
			m_matrixFirstCombine._43 = vNext.m128_f32[2];
			m_matrixFirstCombine._44 = 1.f;
			test.r[3] = vNext;

			//망토가 늘어지는 경우를 어떻게하지..		

			XMStoreFloat4x4(&m_CombinedTransformationMatrix, test);

		}
		else
		{

			XMStoreFloat4x4(&m_PreCombinedTransformationMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));	// 이전 컴바인 저장	

			XMStoreFloat4x4(&m_CombinedTransformationMatrix,
				XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

		}
	}

	/* 루트 모션 애니메이션 이동 행렬 저장하기*/
	if (m_iParentBoneIndex == 1)
	{

		XMMATRIX matrix = XMMatrixIdentity();

		if (fabs(m_CombinedTransformationMatrix._41) < 1e-5f &&
			fabs(m_CombinedTransformationMatrix._42) < 1e-5f &&
			fabs(m_CombinedTransformationMatrix._43) < 1e-5f)
		{
			// 단위 행렬로 초기화		
			XMStoreFloat4x4(&m_pPreRootMatrix, XMMatrixIdentity());

			/* 보간 때 발생하는 문제 해결해야함 */
		}


		matrix.r[3] = XMVectorSubtract(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3], XMLoadFloat4x4(&m_pPreRootMatrix).r[3]);
		matrix.r[3] = XMVectorSetW(matrix.r[3], 1.f); // W 값을 1로 고정	


		// 결과를 저장
		XMStoreFloat4x4(&m_RootMotionMatrix, matrix);
		XMStoreFloat4x4(&m_pPreRootMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));


		m_CombinedTransformationMatrix._41 = 0.f;
		m_CombinedTransformationMatrix._42 = 0.f;
		m_CombinedTransformationMatrix._43 = 0.f;
		m_CombinedTransformationMatrix._44 = 1.f;

	}

}

HRESULT CBone::Save_Bone(ostream& os)
{
	os.write(m_szName, sizeof(_char) * MAX_PATH);
	os.write((char*)&m_iParentBoneIndex, sizeof(_int));
	os.write((char*)&m_TransformationMatrix, sizeof(_float4x4));

	return S_OK;
}

HRESULT CBone::Load_Bone(istream& os)
{
	os.read(m_szName, sizeof(_char) * MAX_PATH);
	os.read((char*)&m_iParentBoneIndex, sizeof(_int));
	os.read((char*)&m_TransformationMatrix, sizeof(_float4x4));

	return S_OK;
}

CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone();
	if (FAILED(pInstance->Initialize(pAINode, iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::LoadCreate(std::istream& os)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Load_Bone(os)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
