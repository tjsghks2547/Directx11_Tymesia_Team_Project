#include "Channel.h"
#include "Model.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
    strcpy_s(m_szName, pAIChannel->mNodeName.data);

    /* �� ä��(��)�� �ð��� �´� ����(���)�� �������ϴ� �۾��� ���� �����ؾ��ϰ� */
    /* ��������� ���� �̸��� ���� ���� m_TransformationMatrix�� ���� ��������� �ϱ� ������ */

    m_iBoneIndex = pModel->Get_BoneIndex(m_szName); 

    m_iNumKeyframes = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
    m_iNumKeyframes = max(m_iNumKeyframes, pAIChannel->mNumPositionKeys);
  

    _float3  vScale       = {};
    _float4  vRotation    = {};
    _float3  vTranslation = {};


    for(size_t i=0; i< m_iNumKeyframes; i++)
    {
        KEYFRAME   KeyFrame{};

        if(pAIChannel->mNumScalingKeys > i)
        {
            memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mScalingKeys[i].mTime);
        }
        
        if(pAIChannel->mNumRotationKeys > i)
        {
            vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
            vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
            vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
            vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;

            KeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mRotationKeys[i].mTime);
        }

        if(pAIChannel->mNumPositionKeys > i)
        {
            memcpy(&vTranslation, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
            KeyFrame.fTrackPosition = static_cast<_float>(pAIChannel->mPositionKeys[i].mTime);
        }

        KeyFrame.vScale = vScale; 
        KeyFrame.vRotation = vRotation;    
        KeyFrame.vTranslation = vTranslation;

        m_Keyframes.push_back(KeyFrame);    
    }

    /* ��Ʈ ��� ���� �߰� �ڵ�*/
    XMStoreFloat4x4(&m_WorldTranslation, XMMatrixIdentity()); // ���� ��ķ� �ʱ�ȭ
   
    return S_OK;
}


void CChannel::Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones)
{
 
     if (0.f == fCurrentTrackPosition)
         *pCurrentKeyFrameIndex = 0;

     /* ���� �����ġ�� �´� ���¸� ���Ѵ�. */
     _vector   vScale, vRotation, vTranslation;

     /* ���� �����ġ�� ������ Ű�������� ��ġ�� �Ѿ�� ������ Ű�������� ���¸� �־��ش�*/
     KEYFRAME     LastKeyFrame = m_Keyframes.back();

     if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
     {
         vScale = XMLoadFloat3(&LastKeyFrame.vScale);
         vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
         vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
     }

     /* ������ ��� ���� : ��, �� Ű������ ������ ���¸� ������ ���� ���س����Ѵ�. */
     else
     {
         /* while�� �־��� ������ ������ ��������� ���� �������� ������Ʈ�� �ȵǰ� �Ѳ����� ����ɶ� */
         /* ���� ũ�⿡ ������ �ֹǷ� */
         while (fCurrentTrackPosition >= m_Keyframes[*pCurrentKeyFrameIndex + 1].fTrackPosition)
             ++*pCurrentKeyFrameIndex;

         _float  fRatio = (fCurrentTrackPosition - m_Keyframes[*pCurrentKeyFrameIndex].fTrackPosition) /
             (m_Keyframes[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_Keyframes[*pCurrentKeyFrameIndex].fTrackPosition);


         vScale = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex + 1].vScale), fRatio);
         vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex + 1].vRotation), fRatio);
         vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex + 1].vTranslation), fRatio), 1.f);

     }

     _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
     Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

 

}

void CChannel::Update_Reverse_TransformationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones)
{
    if (0.f >= fCurrentTrackPosition)
        *pCurrentKeyFrameIndex = 1; //0412 ������

    /* ���� �����ġ�� �´� ���¸� ���Ѵ�. */
    _vector   vScale, vRotation, vTranslation;

    /* ���� �����ġ�� ù ��° Ű�������� ��ġ�� �Ѿ�� ù ��° Ű�������� ���¸� �־��ش�*/
    KEYFRAME     FirstKeyFrame = m_Keyframes.front();

    if (fCurrentTrackPosition <= FirstKeyFrame.fTrackPosition)
    {
        vScale = XMLoadFloat3(&FirstKeyFrame.vScale);
        vRotation = XMLoadFloat4(&FirstKeyFrame.vRotation);
        vTranslation = XMVectorSetW(XMLoadFloat3(&FirstKeyFrame.vTranslation), 1.f);
    }

    /* ������ ��� ���� : ��, �� Ű������ ������ ���¸� ������ ���� ���س����Ѵ�. */
    else
    {
        /* while�� �־��� ������ ������ ��������� ���� �������� ������Ʈ�� �ȵǰ� �Ѳ����� ����ɶ� */
        /* ���� ũ�⿡ ������ �ֹǷ� */
        while (fCurrentTrackPosition <= m_Keyframes[*pCurrentKeyFrameIndex - 1].fTrackPosition)  //      pCurrentKeyFrameIndex �̰� 0���� �����ϳ� ����!   
            --*pCurrentKeyFrameIndex;

        _float  fRatio = (m_Keyframes[*pCurrentKeyFrameIndex].fTrackPosition - fCurrentTrackPosition) /
            (m_Keyframes[*pCurrentKeyFrameIndex].fTrackPosition - m_Keyframes[*pCurrentKeyFrameIndex - 1].fTrackPosition);


        vScale = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex - 1].vScale), fRatio);
        vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex - 1].vRotation), fRatio);
        vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex - 1].vTranslation), fRatio), 1.f);
    }

    _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

}

void CChannel::Lerp_TransformationMatrix(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex)
{
    /* 11�� 29�� ���⼭ ���� �ٽ� �ϱ� */
    _vector   vScale, vRotation, vTranslation;

    _float		fRatio = LerpTimeAcc / LerpTime;        
   
    /* ��������*/
    /* ���⼭ �۾�������ҵ��� */
    /* ���� ��ġ�� �ִϸ��̼� ����������ҵ�*/

      // ���� ��ġ�� ���� ��ġ�� ���� ���
   
    vScale       = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&pNextChannel->m_Keyframes.front().vScale), fRatio);
    vRotation    = XMQuaternionSlerp(XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&pNextChannel->m_Keyframes.front().vRotation), fRatio);    
    vTranslation = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&pNextChannel->m_Keyframes.front().vTranslation), fRatio);        
    vTranslation = XMVectorSetW(vTranslation, 1.f); 


    //if(m_iBoneIndex==2)
    //{
    //    vTranslation = { 0.f,0.f,0.f,1.f }; 
    //
    //}

    _matrix  TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);


}

void CChannel::Lerp_Reverse_TransformationMatrix(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex)
{

    _vector   vScale, vRotation, vTranslation;

    _float		fRatio = LerpTimeAcc / LerpTime;

    vScale = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&pNextChannel->m_Keyframes.back().vScale), fRatio);
    vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&pNextChannel->m_Keyframes.back().vRotation), fRatio);
    vTranslation = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&pNextChannel->m_Keyframes.back().vTranslation), fRatio);
    vTranslation = XMVectorSetW(vTranslation, 1.f);


    _matrix  TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

}

void CChannel::Lerp_TransformationMatrix_Offset(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex, _float OffSet)
{
    /* 3�� 18�� ���⼭ ���� �ٽ� �ϱ� */
    _vector   vScale, vRotation, vTranslation;

    _float		fRatio = LerpTimeAcc / LerpTime;



    _uint iNextIndex = 0;

    // OffSet�� �´� Ű������ ã��	
    while (OffSet >= pNextChannel->m_Keyframes[iNextIndex].fTrackPosition)
    {
        ++iNextIndex;
    }

    /* ��������*/
    vScale = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vScale), XMLoadFloat3(&pNextChannel->m_Keyframes.at(iNextIndex).vScale), fRatio);
    vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_Keyframes[*pCurrentKeyFrameIndex].vRotation), XMLoadFloat4(&pNextChannel->m_Keyframes.at(iNextIndex).vRotation), fRatio);
    vTranslation = XMVectorLerp(XMLoadFloat3(&m_Keyframes[*pCurrentKeyFrameIndex].vTranslation), XMLoadFloat3(&pNextChannel->m_Keyframes.at(iNextIndex).vTranslation), fRatio);
    vTranslation = XMVectorSetW(vTranslation, 1.f);

    _matrix  TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);
}

void CChannel::Reset_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex)
{
    *pCurrentKeyFrameIndex = 0;

    KEYFRAME FirstKeyFrame = m_Keyframes.front();   

    _vector vScale, vRotation, vTranslation;    
    
    vScale = XMLoadFloat3(&FirstKeyFrame.vScale);
    vRotation = XMLoadFloat4(&FirstKeyFrame.vRotation);
    vTranslation = XMVectorSetW(XMLoadFloat3(&FirstKeyFrame.vTranslation), 1.f);

    _matrix  TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
    
    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);

}

void CChannel::Reset_ReverseTransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex)
{
    *pCurrentKeyFrameIndex = static_cast<_uint>(m_Keyframes.size()) - 1;    

    KEYFRAME LastKeyFrame = m_Keyframes.back(); 

    _vector vScale, vRotation, vTranslation;    

    vScale = XMLoadFloat3(&LastKeyFrame.vScale);    
    vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);  
    vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f); 

    _matrix  TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation); 

    Bones[m_iBoneIndex]->Set_TransformationMatrix(TransformationMatrix);    
}

HRESULT CChannel::Save_Channel(ostream& os)
{
    os.write(m_szName, sizeof(_char) * MAX_PATH);
    os.write((char*)&m_iBoneIndex, sizeof(_uint));
    os.write((char*)&m_iNumKeyframes, sizeof(_uint));
    writeVectorToBinary(m_Keyframes, os);

    return S_OK;

}

HRESULT CChannel::Load_Channel(istream& is)
{
    is.read(m_szName, sizeof(_char) * MAX_PATH);
    is.read((char*)&m_iBoneIndex, sizeof(_uint));
    is.read((char*)&m_iNumKeyframes, sizeof(_uint));
    readVectorFromBinary(is, m_Keyframes);

    return S_OK;
}

CChannel* CChannel::LoadCreate(istream& is)
{
    CChannel* pInstance = new CChannel();
    if (FAILED(pInstance->Load_Channel(is)))
    {
        MSG_BOX("Failed To LoadCreate : CChannel");
        Safe_Release(pInstance);
    }
    return pInstance;
}

CChannel* CChannel::Create(const aiNodeAnim* pAIChannel, const CModel* pModel)
{
    
    CChannel* pInstance = new CChannel();

    if(FAILED(pInstance->Initialize(pAIChannel,pModel)))
    {
        MSG_BOX("Failed to Created : Channel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CChannel::Free()
{
    __super::Free();
}
