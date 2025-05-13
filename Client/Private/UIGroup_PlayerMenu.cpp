#include "pch.h"
#include "UIGroup_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_ButtonHighlight.h"
#include "UI_KeyBox_Long.h"
#include "Player.h"
#include "UI_Image.h"
#include "UI_Text.h"

CUIGroup_PlayerMenu::CUIGroup_PlayerMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerMenu::CUIGroup_PlayerMenu(const CUIGroup_PlayerMenu& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerMenu::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevelID = static_cast<LEVELID>(pDesc->iCurLevel);


	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu");
	m_pSceneChangePop = m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu_1ChangePop");

	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (1 == Image->Get_UI_GroupID())
		{
			m_pMapImage = Image;
		}
	}
	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (107 == TextBox->Get_UI_GroupID())
		{
			m_pMapName = TextBox;
		}
	}

	return S_OK;
}

void CUIGroup_PlayerMenu::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")))
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(true);

		switch (m_eMyLevelID)
		{
		case Client::LEVEL_TUTORIAL:
			m_pMapImage->Set_TexNumber(2);
			m_pMapName->Set_Content(L"헤르메스 요새");
			break;
		case Client::LEVEL_SEAOFTREES:
			m_pMapImage->Set_TexNumber(0);
			m_pMapName->Set_Content(L"나무바다");
			break;
		case Client::LEVEL_ROYALGARDEN:
			m_pMapImage->Set_TexNumber(1);
			m_pMapName->Set_Content(L"왕실 정원");
			break;
		case Client::LEVEL_OCEAN:
			m_pMapImage->Set_TexNumber(3);
			m_pMapName->Set_Content(L"기억의 대양");
			break;
		case Client::LEVEL_HILL:
			m_pMapImage->Set_TexNumber(4);
			m_pMapName->Set_Content(L"철학자의 언덕");
			break;
		}

	}

}

void CUIGroup_PlayerMenu::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)	
	{
		if (!m_bChangePopOpen)
		{
			MenuButton_Check();
		}
		else
		{
			ChangePopButton_Check();
		}

		if (m_pGameInstance->isKeyEnter(DIK_ESCAPE))
		{
			m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerScreen"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
		}
	}
}

void CUIGroup_PlayerMenu::Late_Update(_float fTimeDelta)
{
}

HRESULT CUIGroup_PlayerMenu::Render()
{
	return S_OK;
}

void CUIGroup_PlayerMenu::MenuButton_Check()
{

	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (100 == Button->Get_UI_GroupID()) // 레벨업
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerLevelUP"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
			}
			if (101 == Button->Get_UI_GroupID()) // 특성 해제
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerTalent"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_0")), true);
			}
			if (102 == Button->Get_UI_GroupID()) // 역병무기
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerSkill"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill")), true);
			}
			if (103 == Button->Get_UI_GroupID()) // 물약
			{
			}
			if (104 == Button->Get_UI_GroupID()) // 잊혀진 깃털 사용
			{
			}
			if (105 == Button->Get_UI_GroupID()) //기억 되살리기 중단	
			{
				if (m_eMyLevelID != LEVEL_HILL)
				{
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu_1ChangePop")), true);
					m_bChangePopOpen = true;

				}

			}
			if (106 == Button->Get_UI_GroupID()) // 게임 재개하기
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);


				//dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_PlayerState(CPlayer::STATE_ARCHIVE_SIT_GETUP);

			}
			if (120 == Button->Get_UI_GroupID()) // 키보드 esc 버튼
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
				//dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Set_PlayerState(CPlayer::STATE_ARCHIVE_SIT_GETUP);

			}

		}
	}
}

void CUIGroup_PlayerMenu::ChangePopButton_Check()
{
	for (auto& Button : m_pSceneChangePop->Find_UI_Button())
	{
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (1 == Button->Get_UI_GroupID()) // 철학자의 집으로 이동
			{
				m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.9f);
				
			}
			if (2 == Button->Get_UI_GroupID()) // 팝업 종료
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu_1ChangePop")), false);
				m_bChangePopOpen = false;
				break;
			}

		}
	}
	if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu_1ChangePop")), false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);

		m_pGameInstance->Set_NextLevel_Open(true, LEVEL_HILL); //철학자의 집 이동
	}
}

HRESULT CUIGroup_PlayerMenu::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_MENU, L"UIScene_PlayerMenu");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_MENU, L"UIScene_PlayerMenu_1ChangePop");
	return S_OK;
}

HRESULT CUIGroup_PlayerMenu::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

CUIGroup_PlayerMenu* CUIGroup_PlayerMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerMenu* pInstance = new CUIGroup_PlayerMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerMenu::Clone(void* pArg)
{
	CUIGroup_PlayerMenu* pInstance = new CUIGroup_PlayerMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerMenu::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_MENU);
}
