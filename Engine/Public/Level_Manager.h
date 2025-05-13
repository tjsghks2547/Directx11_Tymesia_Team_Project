#pragma once

#include "Base.h"

/* 1. ���� �Ҵ�� ������ �ּҸ� �����ϳ�. */
/* 2. �����ϰ� �ִ� ������ �ݺ����� ���Ű� ��ο츦 �����Ѵ�. */
/* 3. ���ο� ������ �Ҵ� ��, ���� ������ �ı��ϰ� ���� ������ �ڿ��鵵 �ı��Ѵ�. */


BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Open_Level(_uint iNewLevelIndex, class CLevel* pNewLevel);
	void Set_NextLevel_Open(_bool bOpen, _uint _iNumberLevel);
	_uint Get_Current_Level_Index();	

public:
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	_uint					m_iLevelIndex = {};
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END