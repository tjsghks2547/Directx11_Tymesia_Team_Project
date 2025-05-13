#include "pch.h"
#include "UIGroup_Loading.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "UI_Scene.h"
#include "UI_Image.h"
#include "UI_LandingScreen.h"


#include "UIGroup_Landing.h"

CUIGroup_Landing::CUIGroup_Landing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Landing::CUIGroup_Landing(const CUIGroup_Landing& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Landing::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Landing::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMessage_Dead = m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, L"UIScene_Landing_1Dead");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Dead, false);

	m_pMessage_Beacon = m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, L"UIScene_Landing_2Beacon");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Beacon, false);

	m_pMessage_Recall = m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, L"UIScene_Landing_3Recall");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Recall, false);

	m_pMessage_Memories = m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, L"UIScene_Landing_4Memories");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Memories, false);

	m_pMessage_MapName = m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, L"UIScene_Landing_5MapName");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_MapName, false);

	
	return S_OK;
}

void CUIGroup_Landing::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

void CUIGroup_Landing::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		Map_Name();

		if (
			dynamic_cast<CUI_LandingScreen*>(m_pMessage_Beacon->Find_UI_Image().front())->Get_OnOff() ||
			dynamic_cast<CUI_LandingScreen*>(m_pMessage_Recall->Find_UI_Image().front())->Get_OnOff() ||
			dynamic_cast<CUI_LandingScreen*>(m_pMessage_Memories->Find_UI_Image().front())->Get_OnOff() ||
			dynamic_cast<CUI_LandingScreen*>(m_pMessage_MapName->Find_UI_Image().front())->Get_OnOff())
		{
			if (m_fRandingTime > 2.f)
			{
				m_fRandingTime = 0.0f;
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Dead, false);
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Beacon, false);
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Recall, false);
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Memories, false);
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_MapName, false);
			}
			else
			{
				m_fRandingTime += fTimeDelta / 3;
			}

		}

		if (dynamic_cast<CUI_LandingScreen*>(m_pMessage_Dead->Find_UI_Image().front())->Get_OnOff())

		{
			if (m_fDeadTime > 1.f)
			{
				m_fDeadTime = 0.0f;
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pMessage_Dead, false);
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				m_pGameInstance->StopAll();
			}
			else
			{
				m_fDeadTime += fTimeDelta / 3;
			}
		}

	}
}

void CUIGroup_Landing::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{

	}
}

HRESULT CUIGroup_Landing::Render()
{

	return S_OK;
}

void CUIGroup_Landing::Map_Name()
{

	for (auto& Image : m_pMessage_MapName->Find_UI_Image())
	{
		switch (m_eMyLevel)
		{
		case LEVEL_HILL:
			Image->Set_TexNumber(6);
			break;
		case LEVEL_SEAOFTREES:
			Image->Set_TexNumber(7);
			break;
		case LEVEL_ROYALGARDEN:
			Image->Set_TexNumber(8);
			break;
		case LEVEL_OCEAN:
			Image->Set_TexNumber(10);
			break;
		}
	}

}

HRESULT CUIGroup_Landing::Ready_UIObject()
{
	
	LoadData_UIObject(LEVEL_STATIC, UISCNEN_MESSAGE, L"UIScene_Landing_1Dead");
	LoadData_UIObject(LEVEL_STATIC, UISCNEN_MESSAGE, L"UIScene_Landing_2Beacon");
	LoadData_UIObject(LEVEL_STATIC, UISCNEN_MESSAGE, L"UIScene_Landing_3Recall");
	LoadData_UIObject(LEVEL_STATIC, UISCNEN_MESSAGE, L"UIScene_Landing_4Memories");
	LoadData_UIObject(LEVEL_STATIC, UISCNEN_MESSAGE, L"UIScene_Landing_5MapName");
	return S_OK;
}

HRESULT CUIGroup_Landing::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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
		MessageBox(g_hWnd, szLastPath, TEXT("Fail"), MB_OK);
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
	_uint iTextureNum = { 0 };
	_uint iGroupID = {};

	while (true)
	{
		_uint iTextSort = { static_cast<CUIObject::TEXTSORT>(CUIObject::TEXTSORT::TEXT_LEFT) };

		ReadFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &fSize, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szFontName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szContentText.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szContentText.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);
		if (iUIType == UI_TEXT)
		{
			ReadFile(hFile, &iTextSort, sizeof(_uint), &dwByte, nullptr);

		}
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
		Desc.eTextSort = static_cast<CUIObject::TEXTSORT>(iTextSort);

		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(iLevelIndex, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);


	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

HRESULT CUIGroup_Landing::LoadData_UIText_Info(const _tchar* szSceneName)
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
		MessageBox(g_hWnd, szLastPath, TEXT("Fail"), MB_OK);
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


		m_TextInfo.push_back(TextInfo);

	}

	CloseHandle(hFile);

	return S_OK;
}

CUIGroup_Landing* CUIGroup_Landing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Landing* pInstance = new CUIGroup_Landing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Landing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Landing::Clone(void* pArg)
{
	CUIGroup_Landing* pInstance = new CUIGroup_Landing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Landing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Landing::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCNEN_MESSAGE);
}
