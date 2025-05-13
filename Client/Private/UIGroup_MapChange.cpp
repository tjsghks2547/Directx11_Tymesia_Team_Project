#include "pch.h"
#include "GameInstance.h"
#include "UI_Scene.h"
#include "UIGroup_MapChange.h"

#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_TextBox.h"
#include "Player.h"
CUIGroup_MapChange::CUIGroup_MapChange(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_MapChange::CUIGroup_MapChange(const CUIGroup_MapChange& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_MapChange::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_MapChange::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;


	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_MAP, L"UIScene_MapChange");
	m_pMapChangePop = m_pGameInstance->Find_UIScene(UISCENE_MAP, L"UIScene_MapChange_1Pop");

	Map_Obj_Setting();

	return S_OK;
}

void CUIGroup_MapChange::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, true);
	}
}

void CUIGroup_MapChange::Update(_float fTimeDelta)
{
	const _tchar* CountText = L"%s";

	if (m_bRenderOpen)
	{
		if (m_pGameInstance->isKeyEnter(DIK_ESCAPE))
		{
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel,TEXT("Layer_MapChange"),false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMyScene, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, false);
			m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
		}

		if (!m_bPopOpen && m_bRenderOpen)
		{
			Map_Tab_Mouse_On();

			for (auto& Button : m_pMyScene->Find_UI_Button())
			{
				if (Button->Get_Mouse_Select_OnOff() && 10 !=Button->Get_UI_GroupID())
				{
					Button->Set_Mouse_Select_OnOff(false);
					m_bPopOpen = true;
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, true);



					if (1 == Button->Get_UI_GroupID()) // 나무 바다
					{
						//wsprintf(m_pPoptitle, CountText, L"정말 나무바다 지도를 선택하시겠습니까?");
						m_pPoptitle = L"정말 나무바다 지도를 선택하시겠습니까?";

						m_eNextLevel = LEVEL_SEAOFTREES;
					}
					if (2 == Button->Get_UI_GroupID()) // 왕실 정원
					{
						m_pPoptitle = L"정말 왕실 정원 지도를 선택하시겠습니까?";
						m_eNextLevel = LEVEL_ROYALGARDEN;
					}
					//if (3 == Button->Get_UI_GroupID()) // 헤르메스 요새
					//{
					//	m_pPoptitle = L"정말 헤르메스 지도를 선택하시겠습니까?";
					//	m_eNextLevel = LEVEL_OCEAN;
					//}
					if (4 == Button->Get_UI_GroupID()) // 기억의 대양
					{
						m_pPoptitle = L"정말 기억의 대양 지도를 선택하시겠습니까?";
						m_eNextLevel = LEVEL_OCEAN;
					}
					for (auto TextBox : m_pMapChangePop->Find_UI_TextBox())
					{
						TextBox->Set_Content(m_pPoptitle);
					}
				}
				else if (Button->Get_Mouse_Select_OnOff() && 10 == Button->Get_UI_GroupID())
				{
					m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_MapChange"), false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMyScene, false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, false);
					m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
					m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), true);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
					dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
				}
			}
		}
		else
		{
			for (auto& Button : m_pMapChangePop->Find_UI_Button())
			{
				if (Button->Get_Mouse_Select_OnOff())
				{
					Button->Set_Mouse_Select_OnOff(false);
					if (1 == Button->Get_UI_GroupID())
					{
						m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.9f);
						m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);

					}
					if (2 == Button->Get_UI_GroupID())
					{
						m_bPopOpen = false;
						m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, false);
					}
				}

				if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
				{
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMyScene, false);
					m_pGameInstance->Set_NextLevel_Open(true, m_eNextLevel);
				}
			}
		}
	}



}

void CUIGroup_MapChange::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

HRESULT CUIGroup_MapChange::Render()
{
	return S_OK;
}

void CUIGroup_MapChange::Map_Obj_Setting()
{
	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID())
		{
			m_pMapName = TextBox;
			m_pMapName->Set_OnOff(false);
		}
		if (2 == TextBox->Get_UI_GroupID())
		{
			m_pMapDesc = TextBox;
			m_pMapDesc->Set_OnOff(false);
		}
	}
	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (1 == Image->Get_UI_GroupID())
		{
			m_pMapImageFrame = Image;
			m_pMapImageFrame->Set_OnOff(false);
		}
		if (2 == Image->Get_UI_GroupID())
		{
			m_pMapImage = Image;
			m_pMapImage->Set_OnOff(false);
		}
		if (3 == Image->Get_UI_GroupID())
		{
			m_pMapImageDiamond = Image;
			m_pMapImageDiamond->Set_OnOff(false);
		}
		
	}

}

void CUIGroup_MapChange::Map_Tab_Mouse_On()
{
	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (Button->Get_Mouse_OnOff())
		{
			m_pMapImageFrame->Set_OnOff(true);
			m_pMapImageDiamond->Set_OnOff(true);

			m_pMapImage->Set_OnOff(true);
			m_pMapImage->Set_TexNumber(Button->Get_UI_GroupID() - 1);

			for (auto& Text : m_TextInfo)
			{
				if (Button->Get_UI_GroupID() == Text.iTextID)
				{
					m_pMapName->Set_OnOff(true);
					m_pMapName->Set_FontName(Text.strFontName.c_str());
					m_pMapName->Set_Content(Text.srtTextContent.c_str());
				}
				if (Button->Get_UI_GroupID() + 10 == Text.iTextID)
				{
					m_pMapDesc->Set_OnOff(true);
					m_pMapDesc->Set_FontName(Text.strFontName.c_str());
					m_pMapDesc->Set_Content(Text.srtTextContent.c_str());
					break;
				}
			}

		}
	}
}

HRESULT CUIGroup_MapChange::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_MAP, L"UIScene_MapChange");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_MAP, L"UIScene_MapChange_1Pop");
	LoadData_UIText_Info(L"UIScene_MapChange");
	return S_OK;
}

HRESULT CUIGroup_MapChange::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_MapChange::LoadData_UIText_Info(const _tchar* szSceneName)
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

CUIGroup_MapChange* CUIGroup_MapChange::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_MapChange* pInstance = new CUIGroup_MapChange(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_MapChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_MapChange::Clone(void* pArg)
{
	CUIGroup_MapChange* pInstance = new CUIGroup_MapChange(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_MapChange");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_MapChange::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_MAP);
}
