#include "Bone.h"
#include "GameInstance.h"

CBone::CBone()
{


}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	strcpy_s(m_szName, pAINode->mName.data); /* data�� �ٿ��� ������ char�� ��ȯ���ֱ� ������ data�� �ٿ���*/


	/*pAINode->mTransformation**�� �ش� ����� ���� ��ȯ ���*/
	/*���� �޾ƿ��� ���� ��Ʈ���� ��������� ���� (�ֳ��ϸ� ��Ʈ����� �θ� �����ϱ�) */
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
	/* �ڷ����� �ٸ��� ������ ������ �ȵ� �׷��� �ȿ� ����ִ� �ڷ����� float���� ���� ������ ���縦 �̿��� �޸� ũ�⸸ŭ ���縦 ����*/

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	/* ��ġ����� ���ִ� ������ Assimp���� ����� column �켱 �� �� �켱 ������� �����ϹǷ�
		_11, _21, _31, _41 ������ ����ȴ� �츮�� major �켱 ����̹Ƿ�  _11 , _12 , _13, _14 �� ������ �����ؾ� �Ǳ� ������
		��ġ����� ������״�.*/

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	/* �θ��� ó�� ���� �����ؾ� �� �� ������ edit ����϶�. */

	/* ó�� �θ���� �Ÿ� �����س��� */


	/* �׷� T������ ���� �� �������� ���� ���� �ϴµ�. */

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

				// �ʱ� �Ÿ� // 
				m_fDistanceWithParent = XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3] - XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix).r[3]));

				/* ���Ⱑ �������� ��� �۾� */
				XMStoreFloat4x4(&m_CombinedTransformationMatrix,
					XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

				/* �ʱ� */
				XMStoreFloat4x4(&m_matrixFirstCombine, XMLoadFloat4x4(&m_CombinedTransformationMatrix));


				m_bDynamicInitialized = false;
			}


			XMStoreFloat4x4(&m_PreCombinedTransformationMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));	// ���� �Ĺ��� ����		

			/* ���Ⱑ �������� ��� �۾� */
			XMStoreFloat4x4(&m_CombinedTransformationMatrix,
				XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

			_matrix test = {};
			test.r[0] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[0];
			test.r[1] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[1];
			test.r[2] = XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[2];


			XMVECTOR gravity = XMVectorSet(0.f, -9.8f, 0.f, 0.f);

			/* Verlet ���� */
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

			//���䰡 �þ����� ��츦 �������..		

			XMStoreFloat4x4(&m_CombinedTransformationMatrix, test);

		}
		else
		{

			XMStoreFloat4x4(&m_PreCombinedTransformationMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));	// ���� �Ĺ��� ����	

			XMStoreFloat4x4(&m_CombinedTransformationMatrix,
				XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));

		}
	}

	/* ��Ʈ ��� �ִϸ��̼� �̵� ��� �����ϱ�*/
	if (m_iParentBoneIndex == 1)
	{

		XMMATRIX matrix = XMMatrixIdentity();

		if (fabs(m_CombinedTransformationMatrix._41) < 1e-5f &&
			fabs(m_CombinedTransformationMatrix._42) < 1e-5f &&
			fabs(m_CombinedTransformationMatrix._43) < 1e-5f)
		{
			// ���� ��ķ� �ʱ�ȭ		
			XMStoreFloat4x4(&m_pPreRootMatrix, XMMatrixIdentity());

			/* ���� �� �߻��ϴ� ���� �ذ��ؾ��� */
		}


		matrix.r[3] = XMVectorSubtract(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3], XMLoadFloat4x4(&m_pPreRootMatrix).r[3]);
		matrix.r[3] = XMVectorSetW(matrix.r[3], 1.f); // W ���� 1�� ����	


		// ����� ����
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
