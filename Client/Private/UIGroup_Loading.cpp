#include "pch.h"
#include "UIGroup_Loading.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "UI_Scene.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_LoadingScreen.h"


#include "UI_ButtonHighlight.h"

CUIGroup_Loading::CUIGroup_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Loading::CUIGroup_Loading(const CUIGroup_Loading& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Loading::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Loading::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");

	
	return S_OK;
}

void CUIGroup_Loading::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		for (auto& Image : m_pMyScene->Find_UI_Image())
		{
			if (0 == Image->Get_UI_GroupID())
			{
				m_iTexNumber =  dynamic_cast<CUI_LoadingScreen*>(Image)->Get_ImageTex();
			}


		}
	}
}

void CUIGroup_Loading::Update(_float fTimeDelta)
{

	if (m_bRenderOpen)
	{
		Texture_Num_Check();
	}
}

void CUIGroup_Loading::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{

	}
}

HRESULT CUIGroup_Loading::Render()
{
	
	return S_OK;
}

void CUIGroup_Loading::Texture_Num_Check()
{
	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID()) // 제목	
		{
			for (auto& Text : m_TextInfo)
			{
				if (m_iTexNumber == Text.iTextID)
				{
					TextBox->Set_FontName(Text.strFontName.c_str());
					TextBox->Set_Content(Text.srtTextContent.c_str());
					break;
				}
				else
				{
					TextBox->Set_FontName(L"");
					TextBox->Set_Content(L"");
				}
			}
		}

		if (10 == TextBox->Get_UI_GroupID()) // 내용
		{
			for (auto& Text : m_TextInfo)
			{
				if ((m_iTexNumber + 10) == Text.iTextID)
				{
					TextBox->Set_FontName(Text.strFontName.c_str());
					TextBox->Set_Content(Text.srtTextContent.c_str());
					break;
				}
				else
				{
					TextBox->Set_FontName(L"");
					TextBox->Set_Content(L"");
				}
			}
		}
	}
}

HRESULT CUIGroup_Loading::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_LOADING, L"UIScene_Loading");
	LoadData_UIText_Info(L"UIScene_Loading");
	return S_OK;
}

HRESULT CUIGroup_Loading::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_Loading::LoadData_UIText_Info(const _tchar* szSceneName)
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

CUIGroup_Loading* CUIGroup_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Loading* pInstance = new CUIGroup_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Loading::Clone(void* pArg)
{
	CUIGroup_Loading* pInstance = new CUIGroup_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Loading::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_LOADING);
}
