#include "..\Public\Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Bone.h"	
#include "Shader.h"
#include "Animation.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CModel::CModel(const CModel & Prototype)
	:CComponent(Prototype)	
	,m_iNumMeshes(Prototype.m_iNumMeshes)	
	,m_Meshes(Prototype.m_Meshes)
	,m_iNumMaterials(Prototype.m_iNumMaterials)
	,m_Materials(Prototype.m_Materials)
	,m_eModelType(Prototype.m_eModelType)
	,m_iNumAnimations{ Prototype.m_iNumAnimations }
	,m_Animations{ Prototype.m_Animations }
	,m_CurrentKeyFrameIndices { Prototype.m_CurrentKeyFrameIndices}
	,m_PreTransformMatrix{Prototype.m_PreTransformMatrix}
	,m_fullpath{ Prototype.m_fullpath }
	,m_bFirst(true)

{
	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

	for (auto& pPrototypeBone : Prototype.m_Bones)		
		m_Bones.push_back(pPrototypeBone->Clone());		

	for (auto& pMaterial : m_Materials)	
		Safe_AddRef(pMaterial);

	for (auto& pMesh : m_Meshes)		
		Safe_AddRef(pMesh);


}

_uint CModel::Get_BoneIndex(const _char* pBoneName) const
{
	_uint iBondeIndex = {};

	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		++iBondeIndex;
		return false; });

	return iBondeIndex;	
}

void CModel::Set_BoneType(const _char* pBoneName, BONE_TYPE _eBoneType, _bool bUpdateOnOff)
{
	_uint iBondeIndex = {};

	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		++iBondeIndex;
		return false; });

	if (iter == m_Bones.end())
	{
		MSG_BOX("Failed to Find : Bone");
	}

	m_Bones[iBondeIndex]->Set_Dynamic_Bone(bUpdateOnOff);
}

const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedTransformationPtr();
}

const _float4x4* CModel::Get_RootMotionMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedRootMotionTransformationPtr();	
}

HRESULT CModel::Initialize_Prototype(MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, BINARY _eBinaryMode)
{
	_uint			iFlag = {};

	/* 바이너리화 관련 주소 저장*/
	m_fullpath = pModelFilePath;
	//

	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (MODEL_NONANIM == eModelType)
		iFlag |= aiProcess_PreTransformVertices;

	else
		iFlag |= aiProcess_GenNormals | aiProcess_CalcTangentSpace |
		aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType;
	
	/* 이것만으로 모든 로드작업은 끝난거다. */

	m_eModelType = eModelType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	

	/* 모델클래스의 주목적 : 모델을 움직이고 그린다. */
	/* 로드해준 데이터를 베이스로해서 움직이고 그리기위해 내 구조에 맞게 정리하는 작업이 필요하다. */
	

	/* Ready_Bones를 항상 Mesh나 Material보다 위에서 해야함  그래야 뼈에 저장된 행렬정보 기준으로
	메쉬의 있는 점들을 미리 다 옮겨주니깐.*/	

#pragma region 바이너리화 Save용도

	if (BINARY::BINARY_SAVE == _eBinaryMode || false == Load_Model(PreTransformMatrix))
	{

		//if (FAILED(Load_Model(PreTransformMatrix)))
		//	return E_FAIL; 과거의 유산

		// 만약 Binary_Save 를 하고싶다면 Loader 에서
		// ../Bin/Resources/Models/Boss/Boss_Magician/Boss_Magician.fbx 
		// 검색하여 매개변수 참고하시오.

		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
		if (nullptr == m_pAIScene)
			return E_FAIL;

		if (FAILED(Ready_Bones(m_pAIScene->mRootNode)))
			return E_FAIL;

		if (FAILED(Ready_Meshes(PreTransformMatrix)))
			return E_FAIL;

		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;


		if (FAILED(Ready_Animations()))
			return E_FAIL;

		if (FAILED(Save_Model(pModelFilePath)))
			return E_FAIL;
	}
	
	
#pragma endregion

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_InputAssembler();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Render_Instance(_uint _iMeshIndex, _uint _iNumInstanceNumber)
{
	if (!m_pInstanceBuffer)
		return E_FAIL;

	m_Meshes[_iMeshIndex]->Bind_InputAssembler_Instance(m_pInstanceBuffer);
	m_Meshes[_iMeshIndex]->Render_Instance(m_pInstanceBuffer, _iNumInstanceNumber);

	return S_OK;
}

//void CModel::Compute_BoundingBox(_float3& _vMin, _float3& _vMax)
//{
//	_vMin = _float3(FLT_MAX, FLT_MAX, FLT_MAX);
//	_vMax = _float3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
//
//	for (_uint i = 0; i < m_Meshes.size(); i++)
//	{
//		_float3 meshMin, meshMax;
//		m_Meshes[i]->Compute_BoundingBox(meshMin, meshMax);
//
//		_vMin.x = min(_vMin.x, meshMin.x);
//		_vMin.y = min(_vMin.y, meshMin.y);
//		_vMin.z = min(_vMin.z, meshMin.z);
//
//		_vMax.x = max(_vMax.x, meshMax.x);
//		_vMax.y = max(_vMax.y, meshMax.y);
//		_vMax.z = max(_vMax.z, meshMax.z);
//	}
//}

void CModel::SetUp_Animation(_uint iAnimIndex, _bool isLoop, _bool ReversePlay)	
{
	/* 역재생 여부 */	
	m_bModelReverseAni = ReversePlay;	

	// Initialize에서 초기 애니메이션 설정을 위해 범승추가
	if(m_bFirst)
	{
		m_iCurrentAnimIndex = iAnimIndex;
		m_bFirst = false;
	}

	else if ((m_iCurrentAnimIndex == iAnimIndex || m_bLerpFinished == false) && !m_bFirst)
	{
		m_isAnimLoop = isLoop;
		return;
	}

	m_iCurrentAnimationIndex = m_iCurrentAnimIndex;
	/* 다르다면  애니메이션 인덱스를 교체해준다.*/
	m_pNextAnimation    = m_Animations[iAnimIndex];
	m_pCurrentAnimation = m_Animations[m_iCurrentAnimationIndex];

	m_iCurrentAnimIndex = iAnimIndex;

	if (!m_bModelReverseAni)	
		m_Animations[m_iCurrentAnimIndex]->Reset(m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], &m_fCurrentTrackPosition);	
	else
		m_Animations[m_iCurrentAnimIndex]->Reverse_Reset(m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], &m_fCurrentTrackPosition); // 여기서 Current가 m_fDuration이 되네	

	m_bFinished = false;

	m_isAnimLoop = isLoop;

	

}

_bool CModel::Play_Animation(_float fTimeDelta)
{
	/* 보간이 끝났는지를 체크해줘야할거같음 그래야 애니메이션이 안어색함*/

	if (!m_bModelReverseAni)
	{

		if (m_pNextAnimation)
		{
			m_bLerpFinished = false;
			m_bFinished = false;
			/* 여기서 이제 먼저 수행해주고!*/
			if (m_pCurrentAnimation->Lerp_NextAnimation(fTimeDelta, m_pNextAnimation, m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimationIndex]))
			{
				m_pNextAnimation = nullptr;

			}
			/* 보간이 끝나면 이 조건문을 벗어나야하니깐.*/

		}
		/* 아 보간의 duration이 끝나기전에 애니메이션 변경 했기 때문에 acctime이 남아서 문제가 발생 */

		else
		{
			/* 현재 애니메이션의 재생시간에 맞는 뼈의 상태를 받아와서 뼈들이 가지고 있는 TrnasformationMatrix를 갱신해준다. */
			/*_bool   isFinished = */
			if (m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, &m_fCurrentTrackPosition, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], m_isAnimLoop))
			{
				m_bFinished = true;
			}
			else
			{
				m_bFinished = false;
			}
			m_bLerpFinished = true;
		}


		/* 모든 뼈의 CombinedTransformationMatrix를 갱신한다. */
		/* CombinedTransformationMatrix = 나의 TrnasformationMatrix * 부모의 Combined */

		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix), fTimeDelta);	
		}

	}

	/* 역방향 애니메이션 진행 */
	else
	{

		if (m_pNextAnimation)
		{
			m_bLerpFinished = false;
			m_bFinished = false;
			/* 여기서 이제 먼저 수행해주고!*/
			if (m_pCurrentAnimation->Lerp_Reverse_NextAnimation(fTimeDelta, m_pNextAnimation, m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimationIndex]))
			{
				m_pNextAnimation = nullptr;

			}
			/* 보간이 끝나면 이 조건문을 벗어나야하니깐.*/

		}
		/* 아 보간의 duration이 끝나기전에 애니메이션 변경 했기 때문에 acctime이 남아서 문제가 발생 */

		else
		{
			/* 현재 애니메이션의 재생시간에 맞는 뼈의 상태를 받아와서 뼈들이 가지고 있는 TrnasformationMatrix를 갱신해준다. */
			/*_bool   isFinished = */
			if (m_Animations[m_iCurrentAnimIndex]->Update_ReverseTransformationMatrix(fTimeDelta, m_Bones, &m_fCurrentTrackPosition, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], m_isAnimLoop))
			{
				m_bFinished = true;
			}
			else
			{
				m_bFinished = false;
			}
			m_bLerpFinished = true;
		}

		/* 모든 뼈의 CombinedTransformationMatrix를 갱신한다. */
		/* CombinedTransformationMatrix = 나의 TrnasformationMatrix * 부모의 Combined */

		for (auto& pBone : m_Bones)
		{
			pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix), fTimeDelta);	
		}

	}

	return true;
}

HRESULT CModel::Bind_Material(CShader* pShader, _uint iMeshIndex, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex)
{
	_uint  iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	return m_Materials[iMaterialIndex]->Bind_Material(pShader, eType, pConstantName, iTextureIndex);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, _uint iMeshIndex, const _char* pConstantName)
{
	/* 여기서 부터 다시 */
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;	
	
	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader, pConstantName, m_Bones);	
}

HRESULT CModel::Create_InstanceBuffer(_uint _iNumInstances, const VTX_MODEL_INSTANCE* _TagInstanceData)
{
	if (m_pInstanceBuffer)
	{
		m_pInstanceBuffer->Release();
		m_pInstanceBuffer = nullptr;
	}

	m_iNumInstances = _iNumInstances;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VTX_MODEL_INSTANCE) * _iNumInstances;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(VTX_MODEL_INSTANCE);


	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = _TagInstanceData;

	HRESULT hr = m_pDevice->CreateBuffer(&bufferDesc, &initData, &m_pInstanceBuffer);
	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Update_InstanceBuffer(_uint _iNumInstances, const VTX_MODEL_INSTANCE* _TagInstanceData)
{
	if (nullptr == m_pInstanceBuffer || nullptr == _TagInstanceData)
		return E_FAIL;

	if (_iNumInstances == 0 || _iNumInstances > 3000)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE tagSubResource = {};
	HRESULT hr = m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tagSubResource);

	if (FAILED(hr) || nullptr == tagSubResource.pData)
		return E_FAIL;

	memcpy(tagSubResource.pData, _TagInstanceData, sizeof(VTX_MODEL_INSTANCE) * _iNumInstances);

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	return S_OK;
}

_float& CModel::Get_CurAnimation_FinalSpeed() //3.16 종한 추가
{
	return m_Animations[m_iCurrentAnimIndex]->Get_CurAnimation_FinalSpeed();
}

HRESULT CModel::Save_Model(const _char* pModelFilePath)
{
	string binaryFile = "../Bin/BinaryFile/";
	// 파일 이름이 포함된 문자열

	// 파일 이름에서 디렉토리 경로를 제외한 부분 추출 
	string filenamePart(m_fullpath);
	size_t lastSlashIndex = filenamePart.find_last_of('/');
	if (lastSlashIndex != string::npos)
	{
		filenamePart = filenamePart.substr(lastSlashIndex + 1);
	}

	size_t lastDotIndex = filenamePart.find_last_of('.');
	if (lastDotIndex != string::npos)
	{
		filenamePart = filenamePart.substr(0, lastDotIndex) + ".bin";
	}

	// 최종적인 경로 조합
	binaryFile += filenamePart;

	ofstream file(binaryFile, ios::out | std::ios::binary);	
	//binaryFile = 경로 + 이름, 여기에 똑같은 파일이 있으면 덮어쓰고, 없으면 새로 만든다.

	if(file.is_open())
	{
		file.write(reinterpret_cast<char*>(&m_PreTransformMatrix), sizeof(_float4x4));	

		/* 뼈 저장 */
		_uint bonesize = static_cast<_uint>(m_Bones.size());	
		file.write((char*)&bonesize, sizeof(_uint));	
		for (auto& bone : m_Bones)
			bone->Save_Bone(file);
		//


		/* 매쉬 저장 */
		file.write((char*)&m_iNumMeshes, sizeof(_uint));	
		for (auto& Mesh : m_Meshes)	
			Mesh->Save_Mesh(file);	
		//


		/* Material 저장 */
		file.write((char*)&m_iNumMaterials, sizeof(_uint));	
		/*for (auto& Material : m_Materials)
		{*/
		for (size_t i = 0; i < m_iNumMaterials; i++)
		{
			m_Materials[i]->Save_Material(file, m_pAIScene->mMaterials[i], pModelFilePath);
		}
		

		/* 애니메이션 및 채널 저장*/
		file.write((char*)&m_iNumAnimations, sizeof(_uint));	

		_uint AnimSize = static_cast<_uint>(m_Animations.size());	
		file.write((char*)&AnimSize, sizeof(_uint));	
		for (auto& Anim : m_Animations)
			Anim->Save_Anim(file);	
		//

		file.close();
	}

	else
	{
		MSG_BOX("Failed to OpenFile");
		file.close();
		return E_FAIL;
	}

	return S_OK;

}

_bool CModel::Load_Model(_fmatrix PreTransformMatrix)
{
	string binaryFile = "../Bin/BinaryFile/";
	// 파일 이름이 포함된 문자열


	// 파일 이름에서 디렉터리 경로를 제외한 부분 추출
	string filenamePart(m_fullpath);
	size_t lastSlashIndex = filenamePart.find_last_of('/');
	if (lastSlashIndex != string::npos)
	{
		filenamePart = filenamePart.substr(lastSlashIndex + 1);
	}

	// 확장자를 bin 으로 변경 
	size_t lastDotIndex = filenamePart.find_last_of('.');
	if (lastDotIndex != string::npos)
	{
		filenamePart = filenamePart.substr(0, lastDotIndex) + ".bin";
	}

	// 최종적인 경로 조합 
	binaryFile += filenamePart;

	ifstream inFile(binaryFile, std::ios::binary);
	if (!inFile.good()) // 경로 안에 파일이 없으면
	{
		isFile = false;  // isFile 을 false하고 리턴
		return isFile;
	}
	else
		isFile = true;   // isFile을 true하고 계속 진행


	if (!inFile.is_open())
	{
		// 이진 파일 열기 실패
		MSG_BOX("Failed to Open File");
		return false;
	}

	else
	{
		inFile.read(reinterpret_cast<char*>(&m_PreTransformMatrix), sizeof(_float4x4));


		_uint bonenum = 0;
		inFile.read((char*)&bonenum, sizeof(_uint));
		for (size_t i = 0; i < bonenum; ++i)
		{
			CBone* bone = CBone::LoadCreate(inFile);
			m_Bones.push_back(bone);
		}


		inFile.read((char*)&m_iNumMeshes, sizeof(_uint));
		for (size_t i = 0; i < m_iNumMeshes; ++i)
		{
			CMesh* mesh = CMesh::LoadCreate(m_pDevice, m_pContext, inFile);
			m_Meshes.push_back(mesh);
		}


		inFile.read((char*)&m_iNumMaterials, sizeof(_uint));
		for (size_t i = 0; i < m_iNumMaterials; ++i)
		{
			CMaterial* Material = CMaterial::LoadCreate(m_pDevice, m_pContext, inFile);
			m_Materials.push_back(Material);
		}


		inFile.read((char*)&m_iNumAnimations, sizeof(_uint));
		_uint AnimSize = 0;
		inFile.read((char*)&AnimSize, sizeof(_uint));
		m_CurrentKeyFrameIndices.resize(m_iNumAnimations);
		for (size_t i = 0; i < AnimSize; ++i)
		{
			CAnimation* Anim = CAnimation::LoadCreate(inFile, &m_CurrentKeyFrameIndices[i]);
			m_Animations.emplace_back(Anim);
		}

	}

	inFile.close();

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);


	return isFile;
}

HRESULT CModel::Ready_Meshes(_fmatrix PreTransformMatrix)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	//aiMesh* pMesh = m_pAIScene->mMeshes[1];	
	//aiBone* pBone = pMesh->mBones[1];	


	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh*		pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, this, m_pAIScene->mMeshes[i], PreTransformMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	};

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	/* 해당 모델에 쓰이는 매쉬의 머터리얼 개수 총 계산 후 
	   해당 머터리얼을 백터 자료형에 넣고 보관 */

	/* 각각의 material 클래스는 매쉬 1개의 머터리얼을 18개로 분리하여
	벡터에 저장하는 형태*/
	m_iNumMaterials = m_pAIScene->mNumMaterials;	
	
	for(size_t i =0; i<m_iNumMaterials; i++)
	{
		CMaterial*  pAIMaterial = CMaterial::Create(m_pDevice, m_pContext, m_pAIScene->mMaterials[i], pModelFilePath);
		if (nullptr == pAIMaterial)
			return E_FAIL;

		m_Materials.push_back(pAIMaterial);	
	}

	return S_OK;

}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL; 

	m_Bones.push_back(pBone);

	_int  iParent = static_cast<_int>(m_Bones.size()) - 1; 

	for (size_t i=0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(pAINode->mChildren[i], iParent); /* 여기서 iParentBoneIndex는 벡터의 인덱스를 의미 11월19일 수업정리 참고*/
	}

	return S_OK;

}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	m_CurrentKeyFrameIndices.resize(m_iNumAnimations);

	for(size_t i = 0; i <m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this, m_CurrentKeyFrameIndices[i]);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);

	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _char * pModelFilePath, MODEL eModelType, _fmatrix PreTransformMatrix, BINARY _eBinaryMode)
{
	CModel*	pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType,pModelFilePath,PreTransformMatrix, _eBinaryMode)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();


	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	for (auto& pMaterial : m_Materials)
		Safe_Release(pMaterial);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Animations.clear();		
	m_Bones.clear();		
	m_Materials.clear();	
	m_Meshes.clear();	


	Safe_Release(m_pInstanceBuffer);
}
