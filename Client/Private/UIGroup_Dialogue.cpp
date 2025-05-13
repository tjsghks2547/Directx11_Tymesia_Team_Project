#include "pch.h"
#include "GameInstance.h"
#include "UI_Scene.h"
#include "UIGroup_Dialogue.h"

#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "Player.h"

CUIGroup_Dialogue::CUIGroup_Dialogue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Dialogue::CUIGroup_Dialogue(const CUIGroup_Dialogue& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Dialogue::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Dialogue::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pTalkScene = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemy");
	for (auto& TextBox : m_pTalkScene->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID())
			TextBox->Set_TextDrawType(TextDrawEffect::TEXT_ALPHA);
	}
	m_pPopScene = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemyPop");

	
	m_pTalkScene_Boss = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemy_1");
	for (auto& TextBox : m_pTalkScene_Boss->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID())
			TextBox->Set_TextDrawType(TextDrawEffect::TEXT_ALPHA);
	}
	m_pPopScene_Boss = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemyPop_1");

	m_pBossTalk = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_BossTalk");
	for (auto& BossTextBox : m_pBossTalk->Find_UI_TextBox())
		m_pBossTextBox = BossTextBox;
	m_pBossTextBox->Set_OnOff(false);
	dynamic_cast<CUI_TextBox*>(m_pBossTextBox)->Set_TextDrawType(TEXT_ALPHA_ANIM);


	m_pEndingTalk = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemyeEnding");
	for (auto& TextBox : m_pEndingTalk->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID())
			TextBox->Set_TextDrawType(TextDrawEffect::TEXT_ALPHA);
	}
	m_pEndingImage = m_pGameInstance->Find_UIScene(UISCENE_DIALOGUE, L"UIScene_AIsemyeEnding_Image");
	_int iNum = 0;
	for (auto& TextBox : m_pEndingImage->Find_UI_TextBox())
	{
			TextBox->Set_TextDrawType(TextDrawEffect::TEXT_ALPHA);
			TextBox->Set_OnOff(false);
			m_pLastText[iNum] = TextBox;
			iNum++;
	}
	return S_OK;
}

void CUIGroup_Dialogue::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_pGameInstance->Get_Scene_Render_State(m_pTalkScene))
		{
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(true);

			m_fDelayTime += fTimeDelta;

			if (m_pGameInstance->isAnyEnter() && m_fDelayTime > 1)
			{
				m_fDelayTime = 0;
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pTalkScene, false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene, true);
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);


			}
		}
		else if (m_pGameInstance->Get_Scene_Render_State(m_pTalkScene_Boss))
		{
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(true);
			m_fDelayTime += fTimeDelta;

			if (m_pGameInstance->isAnyEnter() && m_fDelayTime > 1)
			{
				m_fDelayTime = 0;
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pTalkScene_Boss, false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene_Boss, true);
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);

			}
		}
		else if (m_pGameInstance->Get_Scene_Render_State(m_pEndingTalk))
		{
			dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(true);
			m_fDelayTime += fTimeDelta;

			if (m_pGameInstance->isAnyEnter() && m_fDelayTime > 1)
			{
				m_fDelayTime = 0;
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 1.5f);;
				m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);

			}
		}
		_bool bCheck = false;
		if (m_pGameInstance->Get_Scene_Render_State(m_pEndingTalk) &&
			m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
		{
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEndingTalk, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEndingImage, true);
			bCheck = true;
		}
		if (m_pGameInstance->Get_Scene_Render_State(m_pPopScene))
		{
			AIsemy_Pop_Button();
		}
		else if (m_pGameInstance->Get_Scene_Render_State(m_pPopScene_Boss))
		{
			AIsemy_Pop_Boss_Button();
		}
		else if (m_pGameInstance->Get_Scene_Render_State(m_pEndingImage))
		{
			if(bCheck)
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);

		
		}

		if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_IN))
		{
			m_pLastText[0]->Set_OnOff(true);
			m_pLastText[1]->Set_OnOff(true);
			m_pLastText[2]->Set_OnOff(true);
		
		}
		
	}
}

void CUIGroup_Dialogue::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_pBossTextBox->Get_OnOff())
		{
			if (m_pGameInstance->Get_Condition(m_pBossTalk, 0) == 1) // 보스 등장
			{
				if (m_fBossTalkTime < 3.f)
				{
					for (auto& BossTalk : m_BossTextInfo)
					{
						if (m_pGameInstance->Get_Condition(m_pBossTalk, 1) == BossTalk.iTextID)
						{
							m_pBossTextBox->Set_Content(BossTalk.srtTextContent.c_str());
							break;
						}
					}
				}
				else if (m_fBossTalkTime < 6.f)
				{
					for (auto& BossTalk : m_BossTextInfo)
					{
						if (m_pGameInstance->Get_Condition(m_pBossTalk, 1) + 1 == BossTalk.iTextID)
						{
							m_pBossTextBox->Set_Content(BossTalk.srtTextContent.c_str());
							break;
						}
					}
				}
				else if (m_fBossTalkTime >= 6.0f)
				{
					m_pBossTextBox->Set_OnOff(false);
					m_fBossTalkTime = 0.0f;
				}
				m_fBossTalkTime += fTimeDelta;

			}
			else if (m_pGameInstance->Get_Condition(m_pBossTalk, 0) == 2) // 보스 죽음
			{
				if (m_fBossTalkTime < 3.f)
				{
					for (auto& BossTalk : m_BossTextInfo)
					{
						if (m_pGameInstance->Get_Condition(m_pBossTalk, 1) == BossTalk.iTextID)
						{
							m_pBossTextBox->Set_Content(BossTalk.srtTextContent.c_str());
							break;
						}
					}
				}
				else if (m_fBossTalkTime >= 3.0f)
				{
					m_pBossTextBox->Set_OnOff(false);
					m_fBossTalkTime = 0.0f;
				}
				m_fBossTalkTime += fTimeDelta;

			}
			
		}
		else
		{
			m_fBossTalkTime = 0.0f;

		}


	}
}

void CUIGroup_Dialogue::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

HRESULT CUIGroup_Dialogue::Render()
{

	return S_OK;
}

void CUIGroup_Dialogue::AIsemy_Pop_Button()
{
	for (auto& Button : m_pPopScene->Find_UI_Button())
	{
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (1 == Button->Get_UI_GroupID()) // 맵 체인지
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_Dialogue"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene, false);

				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_MapChange"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MAP, L"UIScene_MapChange")), true);

			}
			if (2 == Button->Get_UI_GroupID()) // 플레이어 메뉴
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_Dialogue"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene, false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerMenu"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
			}
			if (3 == Button->Get_UI_GroupID()) // 떠나기?
			{
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_Dialogue"), false);
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene, false);
				dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);

			}
		}
	}
}

void CUIGroup_Dialogue::AIsemy_Pop_Boss_Button()
{
	for (auto& Button : m_pPopScene_Boss->Find_UI_Button())
	{
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (1 == Button->Get_UI_GroupID())
			{
				Button->Set_Mouse_Select_OnOff(false);

				m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.9f);
			}
			if (2 == Button->Get_UI_GroupID()) // 할일이 남아 있어!
			{
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false);
				m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_Dialogue"), false);
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene_Boss, false);
				dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
			}
		}
	}
	if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
	{
		m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_Dialogue"), false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPopScene_Boss, false);

		m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
		m_pGameInstance->Set_NextLevel_Open(true, LEVEL_HILL);
	}
}

HRESULT CUIGroup_Dialogue::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemy");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemyPop");
	
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemy_1");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemyPop_1");
	
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemyeEnding");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_AIsemyeEnding_Image");
	
	LoadData_UIObject(LEVEL_STATIC, UISCENE_DIALOGUE, L"UIScene_BossTalk");
	LoadData_UIText_Info(L"UIScene_BossTalk");


	return S_OK;
}

HRESULT CUIGroup_Dialogue::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_Dialogue::LoadData_UIText_Info(const _tchar* szSceneName)
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


		m_BossTextInfo.push_back(TextInfo);

	}

	CloseHandle(hFile);

	return S_OK;
}

CUIGroup_Dialogue* CUIGroup_Dialogue::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Dialogue* pInstance = new CUIGroup_Dialogue(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Dialogue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Dialogue::Clone(void* pArg)
{
	CUIGroup_Dialogue* pInstance = new CUIGroup_Dialogue(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Dialogue");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Dialogue::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_DIALOGUE);
}
