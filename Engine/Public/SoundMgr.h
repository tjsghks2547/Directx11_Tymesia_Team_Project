#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
private:
    struct ChannelSound
    {
        FMOD::Channel* pChannel = { nullptr };
        _float fTime = { 0.f };
        _float fMaxTime = { 1.f };
        _float fCurVolume = { 0.3f };
        _bool bIncrease = { false };
    };

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;	
private:
    // ���� ���ҽ� ������ ���� ��ü 
    map<const _tchar*, FMOD::Sound*> m_mapSound;    

    // FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
    FMOD::Channel* m_pChannelArr[MAXCHANNEL];
    // ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
    FMOD::System* m_pSystem = { nullptr };
    _tchar* m_CurBGM;

    vector<ChannelSound> m_vecChannels;

public:
	HRESULT Initialize();	    
    void Update(_float fTimeDelta);
	void Release();	    


public:
    void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume); 
    void PlayBGM(const _tchar* pSoundKey, float fVolume);
    void StopSlowly(CHANNELID eID, _float fMaxTime, _float fCurVolume);
    void StopSound(CHANNELID eID);
    void StopAll();
    void SetChannelVolume(CHANNELID eID, float fVolume);
    bool CheckSoundPlaying(CHANNELID eID);
    _tchar* Get_CurBGM() { return m_CurBGM; };

private:
    void LoadSoundFile();

private:
    bool m_isPlaying = { false };

public:
    static CSoundMgr* Create(); 
    virtual void Free();

};

END
