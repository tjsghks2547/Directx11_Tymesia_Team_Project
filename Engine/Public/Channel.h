#pragma once

#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default; 

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	void Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones);
	void Update_Reverse_TransformationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones);		
	void Lerp_TransformationMatrix(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex);
	void Lerp_Reverse_TransformationMatrix(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex);	
	void Lerp_TransformationMatrix_Offset(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex, _float OffSet);	
	void Reset_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex);
	void Reset_ReverseTransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex);	


	/*void Lerp(_uint _iCurrentAnimIndex)*/
	const _char* Get_Name() const {	
		return m_szName;	
	}	

private:
	char		m_szName[MAX_PATH] = {};
	
	/* �� ���� Ư�� �ִϸ��̼��� �����ϱ� ���� Ư�� �ð������� ���¸� ������. */

	/* KeyFrame : Ư�� �ð��� ǥ���ؾ��� ���� ����. */
	_uint					m_iNumKeyframes = {};
	vector<KEYFRAME>		m_Keyframes;

	_uint                   m_iCurrentKeyFrameIndex = {};

	/* �� ä��(��)�� �̸��� ���� �̸��� ���� ��(��ü��)���� ���� ã�Ƽ� �� ���� �ε����� �����Ѵ�.*/
	_uint					m_iBoneIndex = {};


	/* 12�� 10�� ��Ʈ ��� */
	/* �� ä��(��)�� ����� �����Ʈ������ �����ֱ� ���� ��� ���� ����*/
	_float4x4               m_WorldTranslation = { };		



public:
	HRESULT Save_Channel(ostream& os);	
	HRESULT Load_Channel(istream& is);	
	static CChannel* LoadCreate(istream& is);	
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	virtual void Free() override; 

};

END