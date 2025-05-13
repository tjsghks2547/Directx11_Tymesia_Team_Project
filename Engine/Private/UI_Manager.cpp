#include "UI_Manager.h"
#include "UI_Scene.h"
#include "UIObject.h"

#include "GameInstance.h"
#include "GameObject.h"

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CUI_Manager::Initialize(_uint iNumScenes)
{
	m_iNumScenes = iNumScenes;

	m_pScenes = new map<const _wstring, class CUI_Scene*>[m_iNumScenes];

	return S_OK;
}
void CUI_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
		{
			Pair.second->Priority_Update(fTimeDelta);
		}
	}
}
void CUI_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Pair.second->Update(fTimeDelta);
	}
}
void CUI_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}
void CUI_Manager::Clear(_uint iScenelIndex)
{
	for (auto& Pair : m_pScenes[iScenelIndex])
		Safe_Release(Pair.second);

	m_pScenes[iScenelIndex].clear();
}

void CUI_Manager::Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj)
{
	CUI_Scene* pUIScene = Find_UIScene(iScenelIndex, strSceneTag);
	if (nullptr != pUIScene)
	{
		pUIScene->Clear_Choice(iUIType, pUIObj);
	}
}

void CUI_Manager::Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag)
{
	CUI_Scene* pUIScene = Find_UIScene(iScenelIndex, strSceneTag);
	if (nullptr != pUIScene)
	{
		pUIScene->Clear_Last(iUIType);
	}
}

void CUI_Manager::Clear_ALL()
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Safe_Release(Pair.second);

		m_pScenes[i].clear();
	}

}

CUI_Scene* CUI_Manager::Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag)
{
	auto	iter = m_pScenes[iSceneIndex].find(strSceneTag);

	if (iter == m_pScenes[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

map<const _wstring, class CUI_Scene*>* CUI_Manager::Find_UIScene_ALL()
{
	return m_pScenes;
}

HRESULT CUI_Manager::Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag,_uint iUIType, void* pArg)
{
	/* UIScene에 저장할 UIObj 복제해오기 */
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CUI_Scene* pScene = Find_UIScene(iSceneIndex, strSceneTag);

	if (nullptr == pScene)
	{
		pScene = CUI_Scene::Create();

		pScene->Add_UIObject_Type(iUIType, pGameObject);
		m_pScenes[iSceneIndex].emplace(strSceneTag, pScene);
	}
	else
	{
		pScene->Add_UIObject_Type(iUIType, pGameObject); 
	}
	
	return S_OK;
}

HRESULT CUI_Manager::UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen)
{
	return pScene->UIScene_UIObject_Render_OnOff(bOpen);
}

_bool CUI_Manager::Get_Scene_Render_State(CUI_Scene* pScene)
{
	return pScene->Get_Scene_Render_State();
}

HRESULT CUI_Manager::Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen)
{
	return pScene->Set_All_UIObject_Condition_Open(bOpen);
}

void CUI_Manager::Find_TextBox_PlayerScreen(CUI_Scene* pScene, _uint iGroupID, _uint iCount)
{
	return pScene->Find_TextBox_PlayerScreen(iGroupID,iCount);
}

void CUI_Manager::Set_Condition(CUI_Scene* pScene, _int iCondition1, _int iCondition2)
{
	return pScene->Set_Condition(iCondition1, iCondition2);
}

_int CUI_Manager::Get_Condition(CUI_Scene* pScene, _uint iConditionNum)
{
	return pScene->Get_Condition(iConditionNum);
}

HRESULT CUI_Manager::LoadDataFile_UIObj_Info(HWND hWnd,_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);


	HANDLE		hFile = CreateFile(szLastPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szLastPath, TEXT("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  fPos = {};
	_float2  fSize = {};
	_float3  fRotation = {};
	_uint  iLen = {};
	_wstring szFontName = {};
	_wstring szContentText = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};
	_uint iTextureNum = {};
	_uint iGroupID = {};

	while (true)
	{
		ReadFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &fSize, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		if (iUIType == UI_TEXT || iUIType == UI_BUTTON)
		{
			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szFontName.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szContentText.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szContentText.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		}

		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		Desc.fNear = 0.f;
		Desc.fFar = 1.f;

		Desc.fX = fPos.x;
		Desc.fY = fPos.y;
		Desc.fZ = fPos.z;
		Desc.fSizeX = fSize.x;
		Desc.fSizeY = fSize.y;

		Desc.strFontName = szFontName;
		Desc.strContent = szContentText;

		Desc.strProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
		Desc.iTexNumber = iTextureNum;
		Desc.iGroupID = iGroupID;
		Desc.fRotation = fRotation;
 		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(iLevelIndex, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	//MessageBox(hWnd, L"Load 완료", TEXT("성공"), MB_OK);
	return S_OK;
}

HRESULT CUI_Manager::LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut)
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szPlusText[MAX_PATH] = "_Text";
	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szPlusText);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);

	HANDLE		hFile = CreateFile(szLastPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(hWnd, szLastPath, TEXT("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	UI_TextInfo TextInfo = {};
	_uint iLen = {};

	while (true)
	{

		ReadFile(hFile, &TextInfo.iTextID, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.strFontName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.strFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.srtTextContent.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.srtTextContent.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &TextInfo.fTextStartPos, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &TextInfo.fTextSize, sizeof(_float2), &dwByte, nullptr);


		if (0 == dwByte)
		{
			break;
		}


		pOut.push_back(TextInfo);

	}

	CloseHandle(hFile);

	return S_OK;
}

CUI_Manager* CUI_Manager::Create(_uint iNumScenes)
{
	CUI_Manager* pInstance = new CUI_Manager();

	if (FAILED(pInstance->Initialize(iNumScenes)))
	{
		MSG_BOX("Failed To Created : CUI_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Safe_Release(Pair.second);

		m_pScenes[i].clear();
	}

	Safe_Delete_Array(m_pScenes);
	Safe_Release(m_pGameInstance);
}
