#pragma once 

#include "Base.h"

BEGIN(Engine)


class CAnimation : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;


public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	_bool Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop);
	_bool Update_ReverseTransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop);	

	/* 11�� 27�� �߰��� �ڵ� */
	void  Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton);
	void  Reverse_Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton);		
	_uint Get_ChannelIndex(const _char* pChannelName);
	_bool Lerp_NextAnimation(_float fTimeDelta, class CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices);
	_bool Lerp_Reverse_NextAnimation(_float fTimeDelta, class CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices);	
	void  SetLerpTime(_float _LerpTime) { m_LerpTime = _LerpTime; }

	/*1�� 15�� �߰��� �ڵ�*/
	_bool isAniMationFinish() { return m_isFinished; }


	/* 2�� 19�� ���� ���� */
	void Set_LerpTime(_float _fLerpTime) { m_LerpTime = _fLerpTime; }
	void Set_StartOffSetTrackPosition(_float _fOffsetTrackValue) { m_fSetStartOffSetTrackPosition = _fOffsetTrackValue; }
	/* 3�� 1�� ���� ���� */
	_float Get_StartOffSetTrackPosition() { return m_fSetStartOffSetTrackPosition; }

	vector<ANIMEVENT>* Get_vecEvent() { return &m_vecAnimFrameEvent; }
	void Set_HitStopTime(_float _fHitStopTime) { m_fHitStopTime = _fHitStopTime; }
	_float& Get_CurAnimation_FinalSpeed() { return m_fSpeed_Final; } //3.16 ���� �߰�
	_float  Get_Current_TrackPoisition() { return m_fCurrentTrackPosition; }
	void Set_AnimationSpeed(_float _fSpeed) { m_fAnimationSpeed = _fSpeed; }

private:
	_bool                      m_bReset = { false };
	_bool					   m_isFinished = { false };
	_bool					   m_bFirst = { true };

	_uint					   m_iNumChannels = {};

	_float					   m_fDuration = {};
	_float					   m_fTickPerSecond = {};
	_float					   m_fCurrentTrackPosition = {};
	_float					   m_fAnimationSpeed = { 1.f };
	_float					   m_fSetStartOffSetTrackPosition = {};
	_float					   m_LerpTime = 0.2f;
	_float					   m_SaveLerpTime = {};	
	_float					   m_LerpTimeAcc = 0.f;
	_float					   m_fHitStopTime = 1.f;
	_float					   m_fSpeed_Final = {}; //3.16 ���� �߰�

	/*Channel == Bone*/
	vector<class CChannel*>    m_Channels;

	_char					   m_szName[MAX_PATH];

	vector<_uint>* m_CurrentKeyFameIndices;
	vector<_float>			   m_vecKeyFrameAnimationSpeed;


	vector<ANIMEVENT>		   m_vecAnimFrameEvent;
	_int					   m_iCountFrameEvent = {};

public:

	HRESULT Save_Anim(ostream& os);
	HRESULT Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices);

	static CAnimation* Create(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	static CAnimation* LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices);
	virtual void Free() override;

};
END