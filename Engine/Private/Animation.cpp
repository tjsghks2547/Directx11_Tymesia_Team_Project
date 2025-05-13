#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
    /* 이 애니메이션의 이름*/
    memcpy(m_szName, pAIAnimation->mName.data, sizeof(_char) * MAX_PATH);


    /* 이 애니메이션의 전체 길이 (10) */
    m_fDuration = static_cast<_float>(pAIAnimation->mDuration);

    /* 이 애니메이션의 초당 재생 거리 */
    m_fTickPerSecond = static_cast<_float>(pAIAnimation->mTicksPerSecond);

    /* 이 애니메이션을 구동하는데 있어 필요한 뼈의 갯수*/
    m_iNumChannels = pAIAnimation->mNumChannels;

    CurrentKeyFrameIndices.resize(m_iNumChannels);

    m_CurrentKeyFameIndices = &CurrentKeyFrameIndices;

    for (size_t i = 0; i < m_iNumChannels; i++)
    {
        CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.push_back(pChannel);
    }

    m_vecKeyFrameAnimationSpeed.resize((int)m_fDuration + 1);
    std::fill(m_vecKeyFrameAnimationSpeed.begin(), m_vecKeyFrameAnimationSpeed.end(), 1.f);

    m_vecAnimFrameEvent.reserve(100);

    return S_OK;
}

_bool CAnimation::Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop)
{

    if (*pCurrentTrackPoisiton >= m_fDuration)
        m_fCurrentTrackPosition = m_fDuration;

    if (m_bFirst)
    {
        m_fSpeed_Final = m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton) * m_fHitStopTime;
        *pCurrentTrackPoisiton += m_fSetStartOffSetTrackPosition + m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton) * m_fHitStopTime;
        m_fCurrentTrackPosition = *pCurrentTrackPoisiton;
        m_bFirst = false;
    }


    else if (*pCurrentTrackPoisiton <= m_fDuration)
    {
        m_fSpeed_Final = m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton) * m_fHitStopTime;
        *pCurrentTrackPoisiton += m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton) * m_fHitStopTime;
        m_fCurrentTrackPosition = *pCurrentTrackPoisiton;
    }

    if (true == isLoop && *pCurrentTrackPoisiton >= m_fDuration)
    {
        m_isFinished = true;
        *pCurrentTrackPoisiton = 0.f;

        for (auto& pEvent : m_vecAnimFrameEvent)
        {
            pEvent.isPlay = false;
            pEvent.isEventActivate = false;
        }

    }


    /* 다음 애니메이션으로 이동전에는 여기 걸려서 true가 반환될거임.*/
    else if (*pCurrentTrackPoisiton >= m_fDuration)
    {
        m_isFinished = true;
        return true;
    }

    else
        m_isFinished = false;



    _uint			iNumChannels = {};

    for (auto& pChannel : m_Channels)
    {

        pChannel->Update_TransformationMatrix(*pCurrentTrackPoisiton, &CurrentKeyFrameIndices[iNumChannels++], Bones);

    }

    //애니메이션 이벤트

    for (auto& pEvent : m_vecAnimFrameEvent)
    {
        if (pEvent.fStartTime <= *pCurrentTrackPoisiton && pEvent.fEndTime > *pCurrentTrackPoisiton)
        {
            /* 여기서 실행 시켜줘야 함*/
            pEvent.isEventActivate = true;
        }

        else
        {
            pEvent.isEventActivate = false;
        }
    }

    return false;
}

_bool CAnimation::Update_ReverseTransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop)
{
    // 역방향 순서 애니메이션 
    if (m_bFirst)
    {
        *pCurrentTrackPoisiton -= m_fSetStartOffSetTrackPosition + m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed;
        m_bFirst = false;
    }


    else if (*pCurrentTrackPoisiton >= 0.f)
    {
        *pCurrentTrackPoisiton -= m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed;
    }

    if (true == isLoop && *pCurrentTrackPoisiton <= 0.f)
    {
        m_isFinished = true;
        *pCurrentTrackPoisiton = 0.f;
    }


    /* 다음 애니메이션으로 이동전에는 여기 걸려서 true가 반환될거임.*/
    else if (*pCurrentTrackPoisiton <= 0.f)
    {
        m_isFinished = true;
        return true;
    }

    else
        m_isFinished = false;



    _uint			iNumChannels = {};

    for (auto& pChannel : m_Channels)
    {

        pChannel->Update_Reverse_TransformationMatrix(*pCurrentTrackPoisiton, &CurrentKeyFrameIndices[iNumChannels++], Bones);

    }

    //애니메이션 이벤트

    for (auto& pEvent : m_vecAnimFrameEvent)
    {
        if (pEvent.fStartTime >= m_fCurrentTrackPosition && pEvent.fEndTime < m_fCurrentTrackPosition) // 방향 반대 되야한다.       
        {
            /* 여기서 실행 시켜줘야 함*/
            pEvent.isEventActivate = true;
        }

        else
        {
            pEvent.isEventActivate = false;
        }
    }

    return false;
}

void CAnimation::Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton)
{
    *pCurrentTrackPoisiton = 0.0f;
    m_isFinished = false;
    m_fCurrentTrackPosition = 0.f;
    m_bFirst = true;
    m_LerpTime = m_SaveLerpTime;

    _uint  iChannelIndex = { 0 };
    for (auto& pChannel : m_Channels)
    {
        pChannel->Reset_TransformationMatrix(Bones, &CurrentKeyFrameIndices[iChannelIndex++]);
    }

    for (auto& pEvent : m_vecAnimFrameEvent)
    {
        pEvent.isPlay = false;
        pEvent.isEventActivate = false;
    }


}

void CAnimation::Reverse_Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton)
{
    *pCurrentTrackPoisiton = m_fDuration;
    m_isFinished = false;
    m_fCurrentTrackPosition = m_fDuration;
    m_bFirst = true;
    m_LerpTime = m_SaveLerpTime;

    _uint  iChannelIndex = { 0 };
    for (auto& pChannel : m_Channels)
    {
        pChannel->Reset_ReverseTransformationMatrix(Bones, &CurrentKeyFrameIndices[iChannelIndex++]);
    }

    for (auto& pEvent : m_vecAnimFrameEvent)
    {
        pEvent.isPlay = false;
        pEvent.isEventActivate = false;
    }
}

_uint CAnimation::Get_ChannelIndex(const _char* pChannelName)
{
    _uint	iChannelIndex = {};

    auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)->_bool {
        if (false == strcmp(pChannel->Get_Name(), pChannelName))
            return true;

        ++iChannelIndex;
        return false;
        });

    return iChannelIndex;
}

_bool CAnimation::Lerp_NextAnimation(_float fTimeDelta, CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices)
{
    m_LerpTimeAcc += fTimeDelta;


    if (pNextAnimation && m_LerpTimeAcc <= m_LerpTime)
    {
        _uint iChannelIndex = 0;


        if (pNextAnimation->Get_StartOffSetTrackPosition() > 0.f)
        {
            for (auto& pChannel : m_Channels)
            {
                pChannel->Lerp_TransformationMatrix_Offset(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex], pNextAnimation->Get_StartOffSetTrackPosition());
                iChannelIndex++;
            }
        }
        else
        {
            for (auto& pChannel : m_Channels)
            {
                pChannel->Lerp_TransformationMatrix(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex]);
                iChannelIndex++;
            }
        }

        return false;
    }

    else
    {
        m_LerpTimeAcc = 0.0f;
        return true;
    }


}

_bool CAnimation::Lerp_Reverse_NextAnimation(_float fTimeDelta, CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices)
{
    m_LerpTimeAcc += fTimeDelta;


    if (pNextAnimation && m_LerpTimeAcc <= m_LerpTime)
    {
        _uint iChannelIndex = 0;


        for (auto& pChannel : m_Channels)
        {
            pChannel->Lerp_Reverse_TransformationMatrix(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex]);
            iChannelIndex++;
        }


        return false;
    }

    else
    {
        m_LerpTimeAcc = 0.0f;
        return true;
    }

}

HRESULT CAnimation::Save_Anim(ostream& os)
{
    os.write((char*)&m_szName, sizeof(_char) * MAX_PATH);

    os.write((char*)&m_LerpTime, sizeof(_float));
    os.write((char*)&m_fAnimationSpeed, sizeof(_float));

    os.write((char*)&m_fDuration, sizeof(_float));

    /* 애니메이션 툴 관련 추가 작업 */
    for (int i = 0; i <= (int)m_fDuration; i++)
    {
        os.write((char*)&m_vecKeyFrameAnimationSpeed.at(i), sizeof(_float));
    }

    /* ================================*/

    os.write((char*)&m_fTickPerSecond, sizeof(_float));
    os.write((char*)&m_fCurrentTrackPosition, sizeof(_float));
    os.write((char*)&m_isFinished, sizeof(_bool));
    os.write((char*)&m_iNumChannels, sizeof(_uint));

    for (auto& channel : m_Channels)
        channel->Save_Channel(os);

    m_iCountFrameEvent = static_cast<_int>(m_vecAnimFrameEvent.size());

    os.write((char*)&m_iCountFrameEvent, sizeof(int));

    for (auto& iter : m_vecAnimFrameEvent)
    {
        os.write((char*)&iter.eType, sizeof(EVENT_FRAME_TYPE));
        os.write((char*)&iter.fStartTime, sizeof(_float));
        os.write((char*)&iter.fEndTime, sizeof(_float));
        os.write((char*)&iter.isEventActivate, sizeof(_bool));
        os.write((char*)&iter.isPlay, sizeof(_bool));
        os.write((char*)&iter.szName, sizeof(_char) * MAX_PATH);
    }



    return S_OK;
}

HRESULT CAnimation::Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices)
{
    is.read((char*)&m_szName, sizeof(_char) * MAX_PATH);

    is.read((char*)&m_LerpTime, sizeof(_float));
    is.read((char*)&m_fAnimationSpeed, sizeof(_float));

    is.read((char*)&m_fDuration, sizeof(_float));
    /* 애니메이션 툴 관련 추가 작업 */
    m_vecKeyFrameAnimationSpeed.resize(((int)m_fDuration) + 1);

    for (int i = 0; i <= (int)m_fDuration; i++)
    {
        is.read((char*)&m_vecKeyFrameAnimationSpeed.at(i), sizeof(_float));
    }
    /* ============================= */
    is.read((char*)&m_fTickPerSecond, sizeof(_float));
    is.read((char*)&m_fCurrentTrackPosition, sizeof(_float));
    is.read((char*)&m_isFinished, sizeof(_bool));
    is.read((char*)&m_iNumChannels, sizeof(_uint));
    /*readVectorFromBinary(is, m_CurrentKeyFrameIndices);*/
    CurrentKeyFrameIndices.resize(m_iNumChannels);


    for (_uint i = 0; i < m_iNumChannels; ++i)
    {
        CChannel* pChannel = CChannel::LoadCreate(is);
        m_Channels.push_back(pChannel);
    }

    is.read((char*)&m_iCountFrameEvent, sizeof(int));

    for (_int i = 0; i < m_iCountFrameEvent; i++)
    {
        ANIMEVENT Event;
        is.read((char*)&Event.eType, sizeof(EVENT_FRAME_TYPE));
        is.read((char*)&Event.fStartTime, sizeof(_float));
        is.read((char*)&Event.fEndTime, sizeof(_float));
        is.read((char*)&Event.isEventActivate, sizeof(_bool));
        is.read((char*)&Event.isPlay, sizeof(_bool));
        is.read((char*)&Event.szName, sizeof(_char) * MAX_PATH);

        m_vecAnimFrameEvent.push_back(Event);
    }

    m_SaveLerpTime = m_LerpTime;    

    return S_OK;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
    CAnimation* pInstance = new CAnimation();
    if (FAILED(pInstance->Initialize(pAIAnimation, pModel, CurrentKeyFrameIndices)))
    {
        MSG_BOX("Failed to Created : Animation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CAnimation* CAnimation::LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices)
{
    CAnimation* pInstance = new CAnimation();
    if (FAILED(pInstance->Load_Anim(is, *_CurrentKeyFrameIndices)))
    {
        MSG_BOX("Failed To LoadCreated : CAnimation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimation::Free()
{
    __super::Free();

    for (auto& pChannels : m_Channels)
        Safe_Release(pChannels);

    m_vecKeyFrameAnimationSpeed.clear();
    m_vecAnimFrameEvent.clear();

    m_Channels.clear();
}
