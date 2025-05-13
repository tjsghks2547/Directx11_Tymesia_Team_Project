#pragma once

#include "Component.h"

BEGIN(Engine)
class CAnimation;	

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODEL { MODEL_NONANIM, MODEL_ANIM, MODEL_END };	
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const 
	{
		return m_iNumMeshes;
	}

	_uint Get_BoneIndex(const _char* pBoneName) const;
	void  Set_BoneType(const _char* pBoneName, BONE_TYPE _eBoneType, _bool bUpdateOnOff);	
	const _float4x4* Get_BoneMatrix(const _char* pBoneName) const;
	const _float4x4* Get_RootMotionMatrix(const _char* pBoneName) const;

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, BINARY _eBinaryMode);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);	
	virtual HRESULT Render_Instance(_uint _iMeshIndex, _uint _iNumInstanceNumber);

	//void			Compute_BoundingBox(_float3& _vMin, _float3& _vMax);

public:
	void SetUp_Animation(_uint iAnimIndex, _bool isLoop, _bool ReversePlay = false);	
	void Set_LerpFinished(_bool bCheck) { m_bLerpFinished = bCheck; }	
	_bool Play_Animation(_float fTimeDelta);
	HRESULT Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_BoneMatrices(class CShader* pShader, _uint iMeshIndex, const _char* pConstantName);	
	CAnimation* Get_CurAnimation() { return m_pCurrentAnimation; }	
	CAnimation* Get_NextAnimation() { return m_pNextAnimation; }
	vector<class CAnimation*>& Get_VecAnimation() { return  m_Animations; }	
	_uint   Get_Current_Animation_Index() { return m_iCurrentAnimIndex; }


	/* 2�� 19�� �߰� */
	_float Get_CurrentAnmationTrackPosition() { return m_fCurrentTrackPosition; }	


	/* 2�� 25�� �߰� */
	HRESULT	Create_InstanceBuffer(_uint _iNumInstances, const VTX_MODEL_INSTANCE* _TagInstanceData);
	/* 2�� 28�� �߰� */
	HRESULT Update_InstanceBuffer(_uint _iNumInstances, const VTX_MODEL_INSTANCE* _TagInstanceData);


	/* 3�� 7�� �߰� */

	/* �ִϸ��̼� ���� ������ ���� �Լ� */
	void Set_Continuous_Ani(_bool _first) { m_bFirst = _first; }		

	_float& Get_CurAnimation_FinalSpeed(); //3.16 ���� �߰�

	void Set_Model_ReverseAnim(_bool _OnOff) { m_bModelReverseAni = _OnOff; }	

private:
	/* ������ ������ �����Ѵ�. */
	//const aiScene*					m_pAIScene = { nullptr };
	const aiScene*					m_pAIScene = { nullptr };

	/* ������ �� �а� �������ش�. */
	//Assimp::Importer				m_Importer = {};
	Assimp::Importer				m_Importer = {};  /* ���� ������� �ٽ� �޸� ȹ�� */

private:
	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;

	MODEL							m_eModelType = MODEL_END;

private:
	_uint							m_iNumMaterials = {};
	vector<class CMaterial*>        m_Materials; 

private:
	vector<class CBone*>            m_Bones;

private:
	_uint							m_iNumAnimations = {};
	_uint							m_iCurrentAnimIndex = {};
	/* 11�� 27�� �߰�*/
	_uint							m_iPAnimIndex = {};
	//
	vector<class CAnimation*>       m_Animations; 
	_bool							m_isAnimLoop = {};

	/* �ܲ� �߰� */
	_float							m_fCurrentTrackPosition = {};
	vector<vector<_uint>>			m_CurrentKeyFrameIndices; /* �ִϸ��̼��� ���� �ϴ� �ε��� */
	_float4x4						m_PreTransformMatrix = {};


	/* �ִϸ��̼� ����*/
	_bool							m_bFirst = { true };
	_bool							m_bModelReverseAni = { false };


private:
	/* �������� �߰��۾�*/
	CAnimation* m_pNextAnimation = nullptr;
	CAnimation* m_pCurrentAnimation = nullptr;
	_uint       m_iNextAnimationIndex = {};
	_bool       m_bFinished = { false };
	_uint       m_iCurrentAnimationIndex = {};
	_bool       m_bLerpFinished = { false };

public:
	_bool       Get_LerpFinished()	    { return m_bLerpFinished; }
	/* ��Ʈ ��� �� �߰�*/
	_bool		GetAniFinish()	 { return m_bFinished; }
	

/* ���̳ʸ�ȭ ���� */
private:
	string			m_fullpath; 
	_bool			isFile = { false };

private:
	HRESULT		Save_Model(const _char* pModelFilePath);
	_bool		Load_Model(_fmatrix PreTransformMatrix);

private:
	ID3D11Buffer*				m_pInstanceBuffer = { nullptr };
	ID3D11ShaderResourceView*	m_pInstanceBufferSRV = { nullptr };
	_uint						m_iNumInstances = {};

private:
	HRESULT Ready_Meshes(_fmatrix PreTransformMatrix);	
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(const aiNode* pAIBone, _int iParentBoneIndex = -1);
	HRESULT Ready_Animations();

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MODEL eModelType, _fmatrix  PreTransformMatrix = XMMatrixIdentity(), BINARY _eBinaryMode = BINARY::BINARY_LOAD);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END