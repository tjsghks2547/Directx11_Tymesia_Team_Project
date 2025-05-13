#include "SoundMgr.h"

CSoundMgr::CSoundMgr()
{
	ZeroMemory(&m_pChannelArr, sizeof(FMOD::Channel*) * MAXCHANNEL);
	m_pSystem = nullptr;	
}


HRESULT CSoundMgr::Initialize()
{
	// ���带 ����ϴ� ��ǥ��ü�� �����ϴ� �Լ�
	FMOD::System_Create(&m_pSystem);
	// 1. �ý��� ������, 2. ����� ����ä�� �� , �ʱ�ȭ ���) 
	m_pSystem->init(32, FMOD_INIT_NORMAL, nullptr);
	LoadSoundFile();



	return S_OK;
}

void CSoundMgr::Update(_float fTimeDelta)
{
	for (vector<ChannelSound>::iterator pChannel = m_vecChannels.begin(); pChannel != m_vecChannels.end();)
	{
		if (((*pChannel).fTime += fTimeDelta) > (*pChannel).fMaxTime)
		{
			(*pChannel).pChannel->stop();

			pChannel = m_vecChannels.erase(pChannel);
		}
		else
		{
			if (!(*pChannel).bIncrease)
			{
				(*pChannel).pChannel->setVolume((*pChannel).fCurVolume * (((*pChannel).fMaxTime - (*pChannel).fTime) / (*pChannel).fMaxTime));
			}
			else
			{
				(*pChannel).pChannel->setVolume((*pChannel).fCurVolume * ((*pChannel).fTime / (*pChannel).fMaxTime));
			}
			pChannel++;
		}
	}
}

void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		Mypair.second->release();
	}
		
	m_mapSound.clear();

	m_pSystem->close();	
	m_pSystem->release();	
}

void CSoundMgr::Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	map<const _tchar*, FMOD::Sound*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;


	FMOD_BOOL bPlay = FALSE;

	bool isPlaying = false;
	if (m_pChannelArr[eID])
	{
		m_pChannelArr[eID]->isPlaying(&isPlaying);
		if (isPlaying)
		{
			m_pChannelArr[eID]->stop();
		}
	}

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[eID]);
	m_pChannelArr[eID]->setVolume(fVolume);

		m_pSystem->update();
}

void CSoundMgr::PlayBGM(const _tchar* pSoundKey, float fVolume)
{
	//if (m_CurBGM == pSoundKey)
	//	return;
	//else if(m_CurBGM != nullptr)
	//	StopSound(SOUND_BGM);
	
	//m_CurBGM = pSoundKey;

	map<const _tchar*, FMOD::Sound*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	m_pSystem->playSound(iter->second, nullptr, false, &m_pChannelArr[SOUND_BGM]);
	m_pChannelArr[SOUND_BGM]->setMode(FMOD_LOOP_NORMAL);
	m_pChannelArr[SOUND_BGM]->setVolume(fVolume);

	m_pSystem->update();

	
}

void CSoundMgr::StopSlowly(CHANNELID eID, _float fMaxTime, _float fCurVolume)
{
	if (m_pChannelArr[eID])
	{
		ChannelSound pDesc = {};

		pDesc.pChannel = m_pChannelArr[eID];
		pDesc.fMaxTime = fMaxTime;
		pDesc.bIncrease = false;
		pDesc.fCurVolume = fCurVolume;

		m_vecChannels.push_back(pDesc);
	}
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	if (m_pChannelArr[eID])
	{
		m_pChannelArr[eID]->stop();
	}
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
	{
		if (m_pChannelArr[i])
		{
			m_pChannelArr[i]->stop();
		}
	}
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	if (m_pChannelArr[eID])
	{
		m_pChannelArr[eID]->setVolume(fVolume);
	}
	m_pSystem->update();
}

bool CSoundMgr::CheckSoundPlaying(CHANNELID eID)
{
	bool isPlaying = false;	
	m_pChannelArr[eID]->isPlaying(&isPlaying);	

	return isPlaying;	
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
	_finddata_t fd;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�
	intptr_t handle = _findfirst("../../Client/Bin/Resources/Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../../Client/Bin/Resources/Sound/";	 // ��� ���
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../Sound/" + "Success.wav"
		strcat_s(szFullPath, fd.name);
		// "../Sound/Success.wav"

		FMOD::Sound* pSound = nullptr;

		FMOD_RESULT eRes = m_pSystem->createSound(szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			_tchar* pSoundKey = new _tchar[iLength];
			ZeroMemory(pSoundKey, sizeof(_tchar) * iLength);

			// �ƽ�Ű �ڵ� ���ڿ��� �����ڵ� ���ڿ��� ��ȯ�����ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0, fd.name, -1, pSoundKey, static_cast<int>(iLength));

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _findnext(handle, &fd);
	}

	m_pSystem->update();

	_findclose(handle);
}

CSoundMgr* CSoundMgr::Create()
{
 	CSoundMgr* pInstance = new CSoundMgr();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : SoundMgr");
	}

	return pInstance;
}

void CSoundMgr::Free()
{
	__super::Free(); 
	
	delete this; 
	//Release();
}
