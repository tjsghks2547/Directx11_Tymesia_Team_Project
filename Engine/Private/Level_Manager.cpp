#include "..\Public\Level_Manager.h"
#include "Level.h"

#include "GameInstance.h"


CLevel_Manager::CLevel_Manager()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Open_Level(_uint iNewLevelIndex, CLevel * pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
		m_pGameInstance->Clear(m_iLevelIndex);

	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	m_iLevelIndex = iNewLevelIndex;

	return S_OK;
}

void CLevel_Manager::Set_NextLevel_Open(_bool bOpen, _uint _iNumberLevel)
{
	m_pCurrentLevel->Set_NextLevel_Open(bOpen, _iNumberLevel);
}

_uint CLevel_Manager::Get_Current_Level_Index()	
{
	if (nullptr == m_pCurrentLevel)
		return  0;

	return m_pCurrentLevel->Get_Current_Level_Index();	
}


void CLevel_Manager::Update(_float fTimeDelta)
{
	if(nullptr != m_pCurrentLevel)
		m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

CLevel_Manager * CLevel_Manager::Create()
{
	return new CLevel_Manager();	
}

void CLevel_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pCurrentLevel);

	Safe_Release(m_pGameInstance);
}

