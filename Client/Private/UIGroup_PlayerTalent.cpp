#include "pch.h"
#include "UIGroup_PlayerTalent.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "Player.h"

CUIGroup_PlayerTalent::CUIGroup_PlayerTalent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerTalent::CUIGroup_PlayerTalent(const CUIGroup_PlayerTalent& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerTalent::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevelID = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pGroupLevelUp = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_PlayerLevelUP"), "PlayerLevelUp");

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER");

	m_pMySceneBase = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_0");

	m_pSlot_LightAttack = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_1");
	m_pSlot_Parry = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_2");
	m_pSlot_Dodge = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_3");
	m_pSlot_Claw = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_4");
	m_pSlot_Feather = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_5");
	m_pSlot_Misc = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_6");

	Slot_Setting();
	Talent_Info_Setting();

	CUI_Manager::UI_SAVE_TALENT stGetDate = { };

	if (m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Claw.empty())
	{
		/* 최초에 UIgroupID를 매니저 컨테이너에 저장한다*/
		for (auto& SlotUpdate : m_mapSlot_LightAttack)
			stGetDate.mapSlot_LightAttack.emplace(SlotUpdate.first, SlotUpdate.second.first);

		for (auto& SlotUpdate : m_mapSlot_Parry)
			stGetDate.mapSlot_Parry.emplace(SlotUpdate.first, SlotUpdate.second.first);

		for (auto& SlotUpdate : m_mapSlot_Dodge)
			stGetDate.mapSlot_Dodge.emplace(SlotUpdate.first, SlotUpdate.second.first);

		for (auto& SlotUpdate : m_mapSlot_Claw)
			stGetDate.mapSlot_Claw.emplace(SlotUpdate.first, SlotUpdate.second.first);

		for (auto& SlotUpdate : m_mapSlot_Feather)
			stGetDate.mapSlot_Feather.emplace(SlotUpdate.first, SlotUpdate.second.first);

		for (auto& SlotUpdate : m_mapSlot_Misc)
			stGetDate.mapSlot_Misc.emplace(SlotUpdate.first, SlotUpdate.second.first);
		
		m_pGameInstance->Set_UI_Talent_SaveData(stGetDate);

	}
	else // 컨테이너에서 저장된 값을 가져온다
	{
		stGetDate.mapSlot_LightAttack = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_LightAttack;
		stGetDate.mapSlot_Parry = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Parry;
		stGetDate.mapSlot_Dodge = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Dodge;
		stGetDate.mapSlot_Claw = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Claw;
		stGetDate.mapSlot_Feather = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Feather;
		stGetDate.mapSlot_Misc = m_pGameInstance->Get_UI_Talent_SaveData().mapSlot_Misc;
		
		for (auto& SlotUpdate : m_mapSlot_LightAttack)
		{
			for (auto& SlotData : stGetDate.mapSlot_LightAttack)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
		for (auto& SlotUpdate : m_mapSlot_Parry)
		{
			for (auto& SlotData : stGetDate.mapSlot_Parry)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
		for (auto& SlotUpdate : m_mapSlot_Dodge)
		{
			for (auto& SlotData : stGetDate.mapSlot_Dodge)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
		for (auto& SlotUpdate : m_mapSlot_Claw)
		{
			for (auto& SlotData : stGetDate.mapSlot_Claw)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
		for (auto& SlotUpdate : m_mapSlot_Feather)
		{
			for (auto& SlotData : stGetDate.mapSlot_Feather)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
		for (auto& SlotUpdate : m_mapSlot_Misc)
		{
			for (auto& SlotData : stGetDate.mapSlot_Misc)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if (SlotUpdate.second.first)
						dynamic_cast<CUI_Frame*>(SlotUpdate.second.second)->Set_Slot_State(SLOT_OPEN_OFF);
					break;
				}
			}
		}
	}
	return S_OK;
}

void CUIGroup_PlayerTalent::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		_tchar ChangeText[MAX_PATH] = {};
		const _tchar* CountText1 = L"미사용 특성 점수";
		const _tchar* CountText2 = L" %d";
		m_iTalentPoint = dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Get_TalentPoint_CurrentUnspent();
		wsprintf(ChangeText, CountText2, m_iTalentPoint);
		m_pText_TalentPoint->Set_Content(CountText1);
		dynamic_cast<CUI_Text*>(m_pText_TalentPoint)->Set_Content2(ChangeText);
		dynamic_cast<CUI_Text*>(m_pText_TalentPoint)->Set_Change_TextColor(FONT_WHITE);	
		dynamic_cast<CUI_Text*>(m_pText_TalentPoint)->Set_Change_TextColor2(FONT_GREEN);
		dynamic_cast<CUI_Text*>(m_pText_TalentPoint)->Set_TextDrawType(Engine::TEXT_TWOCOLOR);
	}
}

void CUIGroup_PlayerTalent::Update(_float fTimeDelta)
{
	Player_Talent_Update();

	if (m_bRenderOpen)
	{
		//Slot_Update_State();
		Talent_Tab_Change();
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (Button->Get_Mouse_Select_OnOff())
			{
				if (40 == Button->Get_UI_GroupID()) // ESC
				{
					Button->Set_Mouse_Select_OnOff(false);
					m_bEscape = true;

					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
					//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMySceneBase, false);
					//
					//m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerTalent"), false);

					//m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
					//m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), true);
				}
			}
		}

		if (m_pGameInstance->isKeyEnter(DIK_ESCAPE) || m_bEscape)
		{
			m_bEscape = false;

			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMySceneBase, false);

			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerTalent"), false);

			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), true);
		}

	}

}

void CUIGroup_PlayerTalent::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{

	}
}

HRESULT CUIGroup_PlayerTalent::Render()
{
	if (m_bRenderOpen)
	{
	}
	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_0");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_1");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_2");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_3");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_4");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_5");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_6");


	LoadData_UIText_Info(L"UIScene_PlayerTalent_1", m_LightAttack_Text);
	LoadData_UIText_Info(L"UIScene_PlayerTalent_2", m_ParryText);
	LoadData_UIText_Info(L"UIScene_PlayerTalent_3", m_DodgeText);
	LoadData_UIText_Info(L"UIScene_PlayerTalent_4", m_ClawText);
	LoadData_UIText_Info(L"UIScene_PlayerTalent_5", m_FeatherText);
	LoadData_UIText_Info(L"UIScene_PlayerTalent_6", m_MiscText);

	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_PlayerTalent::LoadData_UIText_Info(const _tchar* szSceneName, vector<UI_TextInfo>& vecText)
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


		vecText.push_back(TextInfo);

	}

	CloseHandle(hFile);

	return S_OK;
}

void CUIGroup_PlayerTalent::Talent_Tab_Change()
{
	TABSTATE eBefore = m_eTabCheck;
	for (auto& TabButton : m_pMySceneBase->Find_UI_Button())
	{
		if (TabButton->Get_Mouse_Select_OnOff())
		{
			if (110 == TabButton->Get_UI_GroupID()) // 검
			{
				m_eTabCheck = TAB_LIGHTATTACK;
				break;
			}
			if (111 == TabButton->Get_UI_GroupID()) // 튕겨내기
			{
				m_eTabCheck = TAB_PARRY;
				break;
			}
			if (112 == TabButton->Get_UI_GroupID()) // 회피
			{
				m_eTabCheck = TAB_DODGE;
				break;
			}
			if (113 == TabButton->Get_UI_GroupID()) // 발톱
			{
				m_eTabCheck = TAB_CLAW;
				break;
			}
			if (114 == TabButton->Get_UI_GroupID()) // 깃털
			{
				m_eTabCheck = TAB_FEATHER;
				break;
			}
			if (115 == TabButton->Get_UI_GroupID()) // 전략
			{
				m_eTabCheck = TAB_MISC;
				break;
			}

		}
	}

	switch (m_eTabCheck)
	{
	case Client::CUIGroup_PlayerTalent::TAB_LIGHTATTACK:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(true);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(false);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(false);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(false);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(false);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(false);
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_PARRY:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(false);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(true);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(false);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(false);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(false);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(false);
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_DODGE:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(false);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(false);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(true);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(false);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(false);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(false);
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_CLAW:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(false);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(false);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(false);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(true);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(false);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(false);
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_FEATHER:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(false);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(false);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(false);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(false);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(true);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(false);
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_MISC:
		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (110 == Button->Get_UI_GroupID()) // 검
				Button->Set_Open_Image(false);
			if (111 == Button->Get_UI_GroupID()) // 튕겨내기
				Button->Set_Open_Image(false);
			if (112 == Button->Get_UI_GroupID()) // 회피
				Button->Set_Open_Image(false);
			if (113 == Button->Get_UI_GroupID()) // 발톱
				Button->Set_Open_Image(false);
			if (114 == Button->Get_UI_GroupID()) // 깃털
				Button->Set_Open_Image(false);
			if (115 == Button->Get_UI_GroupID()) // 전략
				Button->Set_Open_Image(true);
		}
		break;
	}

	if (eBefore != m_eTabCheck) // 탭 상태가 변경되었다면
	{
		m_pText_TalentName->Set_OnOff(false);
		m_pText_TalentDesc->Set_OnOff(false);
		m_pText_TalentOpen->Set_OnOff(false);
		//m_pImage_Frame->Set_OnOff(false);
		m_pImage_Desc->Set_OnOff(false);
	}

	Slot_Update_State(m_eTabCheck);
	Find_Text_Info(m_eTabCheck);
	Talent_Mouse_Check(m_eTabCheck);

}

void CUIGroup_PlayerTalent::Talent_Info_Setting()
{
	for (auto& TextBox : m_pMySceneBase->Find_UI_TextBox())
	{
		if (16 == TextBox->Get_UI_GroupID()) // 미사용 특성 점수
		{
			m_pText_TalentPoint = TextBox;
		}
		if (17 == TextBox->Get_UI_GroupID()) // 특성 제목
		{
			TextBox->Set_OnOff(false);
			m_pText_TalentName = TextBox;

		}
		if (18 == TextBox->Get_UI_GroupID()) //특성 설명
		{
			TextBox->Set_OnOff(false);
			m_pText_TalentDesc = TextBox;

		}
		if (19 == TextBox->Get_UI_GroupID()) // 해제 여부
		{
			TextBox->Set_OnOff(false);
			m_pText_TalentOpen = TextBox;

		}

	}
	for (auto& Image : m_pMySceneBase->Find_UI_Image())
	{
	/*	if (1 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(false);
			m_pImage_Frame = Image;
		}*/
		if (2 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(false);
			m_pImage_Desc = Image;
		}
	}
}

void CUIGroup_PlayerTalent::Find_Text_Info(TABSTATE eTab)
{
	if (nullptr != m_pCurrentTalent)
	{
		_uint iId = m_pCurrentTalent->Get_UI_GroupID();
		switch (eTab)
		{
		case Client::CUIGroup_PlayerTalent::TAB_LIGHTATTACK:
			for (auto& TextInfo : m_LightAttack_Text)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		case Client::CUIGroup_PlayerTalent::TAB_PARRY:
			for (auto& TextInfo : m_ParryText)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		case Client::CUIGroup_PlayerTalent::TAB_DODGE:
			for (auto& TextInfo : m_DodgeText)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		case Client::CUIGroup_PlayerTalent::TAB_CLAW:
			for (auto& TextInfo : m_ClawText)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		case Client::CUIGroup_PlayerTalent::TAB_FEATHER:
			for (auto& TextInfo : m_FeatherText)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		case Client::CUIGroup_PlayerTalent::TAB_MISC:
			for (auto& TextInfo : m_MiscText)
			{
				if (iId == TextInfo.iTextID)
					m_pText_TalentName->Set_Content(TextInfo.srtTextContent.c_str());
				if (iId + 1000 == TextInfo.iTextID)
					m_pText_TalentDesc->Set_Content(TextInfo.srtTextContent.c_str());
			}
			break;
		}
	}
	
}

void CUIGroup_PlayerTalent::Talent_Mouse_Check(TABSTATE eTab)
{
	switch (eTab)
	{
	case Client::CUIGroup_PlayerTalent::TAB_LIGHTATTACK:
		for (auto& SlotEffect : m_mapSlot_LightAttack)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}
			
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_PARRY:
		for (auto& SlotEffect : m_mapSlot_Parry)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}

		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_DODGE:
		for (auto& SlotEffect : m_mapSlot_Dodge)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}

		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_CLAW:
		for (auto& SlotEffect : m_mapSlot_Claw)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}

		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_FEATHER:
		for (auto& SlotEffect : m_mapSlot_Feather)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_MISC:
		for (auto& SlotEffect : m_mapSlot_Misc)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

			if (pSlot->Get_Mouse_OnOff())
			{
				if (0 != pSlot->Get_TexNumber())
				{
					m_pText_TalentName->Set_OnOff(true);
					m_pText_TalentDesc->Set_OnOff(true);
					m_pText_TalentOpen->Set_OnOff(true);
					//m_pImage_Frame->Set_OnOff(true);
					m_pImage_Desc->Set_OnOff(true);
				}
				break;
			}
			else
			{
				m_pText_TalentName->Set_OnOff(false);
				m_pText_TalentDesc->Set_OnOff(false);
				m_pText_TalentOpen->Set_OnOff(false);
				//m_pImage_Frame->Set_OnOff(false);
				m_pImage_Desc->Set_OnOff(false);
			}
		}
		break;
	}

}

void CUIGroup_PlayerTalent::Player_Talent_Update()
{
	for (auto& AttackSlot : m_mapSlot_LightAttack)
	{
		switch (AttackSlot.first)
		{
		case 201:// 검 공격 2레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_HIT_COMBO);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_HIT_COMBO);
			break;
		case 203:// 치유의 처형 1레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_EXECUTION_HP_MP);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_EXECUTION_HP_MP);
			break;
			//case 303:// 치유의 처형 2레벨
		case 204:// 날카로운 무기 1레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_SWORD_ATTACK_DAMAGE);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_SWORD_ATTACK_DAMAGE);
			break;
			//case 304:// 날카로운 무기 2레벨
		case 205:// 에너지가 깃든 무기 1레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_SWORD_MP);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_SWORD_MP);
			break;
			//case 305:// 에너지가 깃든 무기 2레벨
		}
	}

	for (auto& AttackSlot : m_mapSlot_Parry)
	{
		switch (AttackSlot.first)
		{
		case 203:// 치유의 처형 1레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALNET_PARRY_MP);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALNET_PARRY_MP);
			break;
			//case 303:// 날카로운 무기 1레벨
		}
	}

	
	for (auto& AttackSlot : m_mapSlot_Claw)
	{
		switch (AttackSlot.first)
		{
		case 201:// 긴 발톱 2레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_DOUBLE_CLAW);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_DOUBLE_CLAW);
			break;
		case 103:// 에너지가 깃든 발톱 1레벨
			if (AttackSlot.second.first)
				dynamic_cast<CPlayer*>(m_pPlayer)->Set_TalentState(CPlayer::TALENT_CRAW_MP);
			else
				dynamic_cast<CPlayer*>(m_pPlayer)->Sub_TalentState(CPlayer::TALENT_CRAW_MP);
			break;
		}

	}

}

void CUIGroup_PlayerTalent::Slot_Setting()
{
	/* 슬롯 정보를 따로 컨테이너에 저장한다*/
	for (auto& Slot : m_pSlot_LightAttack->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);

		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_LightAttack.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_LightAttack.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}

		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID()/100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Slot : m_pSlot_Parry->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_Parry.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_Parry.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() / 100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Slot : m_pSlot_Dodge->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_Dodge.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_Dodge.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() / 100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Slot : m_pSlot_Claw->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_Claw.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_Claw.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() / 100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Slot : m_pSlot_Feather->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_Feather.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_Feather.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() / 100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (auto& Slot : m_pSlot_Misc->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			m_mapSlot_Misc.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			m_mapSlot_Misc.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
		//pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() / 100) % 10); // 그룹 아이디 백의 자리의 -1 만큼 필요 포인트 설정
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void CUIGroup_PlayerTalent::Slot_Update_State(TABSTATE eTab)
{
	/* map에 저장한 button의 정보에 따라 이미지를 반응 설정*/
	switch (eTab)
	{
	case Client::CUIGroup_PlayerTalent::TAB_LIGHTATTACK:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
		for (auto& MySlot : m_mapSlot_LightAttack)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}
			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_LightAttack)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID && // 조건인 슬롯이 없는 경우
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_PARRY:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
		for (auto& MySlot : m_mapSlot_Parry)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}

			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_Parry)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID &&
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}
		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_DODGE:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
		for (auto& MySlot : m_mapSlot_Dodge)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}

			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_Dodge)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID &&
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}


		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_CLAW:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
		for (auto& MySlot : m_mapSlot_Claw)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			
			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}

			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_Claw)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID &&
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}


		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_FEATHER:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, false);
		for (auto& MySlot : m_mapSlot_Feather)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			
			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}

			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_Feather)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID &&
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}

		}
		break;
	case Client::CUIGroup_PlayerTalent::TAB_MISC:
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_LightAttack, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Parry, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Dodge, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Claw, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Feather, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pSlot_Misc, true);
		for (auto& MySlot : m_mapSlot_Misc)
		{
			CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(MySlot.second.second);

			Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

			
			if (pSlot->Get_Mouse_OnOff())
			{
				m_pCurrentTalent = pSlot;
			}

			/* 포인트 조건*/
			/* 상위 특성 해금 조건*/
			if (pSlot->Get_Mouse_Select_OnOff() && 0 != pSlot->Get_TexNumber())
			{
				if (200 <= MySlot.first &&
					pSlot->Get_Slot_State() == SLOT_OPEN_ON)
				{
					pSlot->Set_Slot_State(SLOT_CLOSE_ON);
					MySlot.second.first = false;
					m_iTalentPoint++;
					dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
				}
				else
				{
					for (auto& FindSlot : m_mapSlot_Misc)
					{
						m_bNotFindConditionID = true;
						if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
							pSlot->Get_ConditionID() == FindSlot.first)
						{
							if (FindSlot.second.first &&
								pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
							{
								pSlot->Set_Slot_State(SLOT_OPEN_ON);
								MySlot.second.first = true;
								m_iTalentPoint--;
								dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
							}
							m_bNotFindConditionID = false;
							break;

						}
						/*	if (SlotEffect.second.first)
								pSlot->Set_Talent_Active(true);
							else
								pSlot->Set_Talent_Active(false);
							break;*/
						else if (1 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 10 == FindSlot.first)
							{
								if (FindSlot.second.first && // 선제 조건 찾고 
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									// 선제조건 만족
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								// 선제조건 불만족
								m_bNotFindConditionID = false;
								break;
							}
						}
						else if (2 == (MySlot.first / 10) % 10)
						{
							if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
								pSlot->Get_ConditionID() - 20 == FindSlot.first)
							{
								if (FindSlot.second.first &&
									pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
								{
									pSlot->Set_Slot_State(SLOT_OPEN_ON);
									MySlot.second.first = true;
									m_iTalentPoint--;
									dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
								}
								m_bNotFindConditionID = false;
								break;
							}
						}

					}

					if (m_iTalentPoint >= pSlot->Get_TalentPoint() &&
						m_bNotFindConditionID &&
						pSlot->Get_Slot_State() == SLOT_CLOSE_ON)
					{
						pSlot->Set_Slot_State(SLOT_OPEN_ON);
						MySlot.second.first = true;
						m_iTalentPoint--;
						dynamic_cast<CUIGroup_PlayerLevelUP*>(m_pGroupLevelUp)->Set_TalentPoint_ALlUnspent(m_iTalentPoint);
					}
				}
			}
		}
		break;
	}
}

void CUIGroup_PlayerTalent::Slot_Update_State_Value(SLOTSTATE eSteteNum, CUI_Frame* pSlotUIObj)
{
	switch (eSteteNum)
	{
	case Client::SLOT_OPEN_ON:
		pSlotUIObj->Set_TexSlot(3);
		pSlotUIObj->Set_TexIconOff(false);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(1);
		break;
	case Client::SLOT_OPEN_OFF:
		pSlotUIObj->Set_TexSlot(3);
		pSlotUIObj->Set_TexIconOff(false);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(2);
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(0);
		break;
	case Client::SLOT_CLOSE_ON:	
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);   // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1); // 이미지 랜더를 꺼야 함
		break;
	case Client::SLOT_CLOSE_OFF:
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(2);  // 이미지 랜더를 꺼야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	case Client::SLOT_CLOSE_CONDITION:
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);  // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	
	}

}

CUIGroup_PlayerTalent* CUIGroup_PlayerTalent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerTalent* pInstance = new CUIGroup_PlayerTalent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerTalent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerTalent::Clone(void* pArg)
{
	CUIGroup_PlayerTalent* pInstance = new CUIGroup_PlayerTalent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerTalent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerTalent::Free()
{
	CUI_Manager::UI_SAVE_TALENT stSatDate = { };

	for (auto& SlotUpdate : m_mapSlot_LightAttack)
		stSatDate.mapSlot_LightAttack.emplace(SlotUpdate.first, SlotUpdate.second.first);

	for (auto& SlotUpdate : m_mapSlot_Parry)
		stSatDate.mapSlot_Parry.emplace(SlotUpdate.first, SlotUpdate.second.first);

	for (auto& SlotUpdate : m_mapSlot_Dodge)
		stSatDate.mapSlot_Dodge.emplace(SlotUpdate.first, SlotUpdate.second.first);

	for (auto& SlotUpdate : m_mapSlot_Claw)
		stSatDate.mapSlot_Claw.emplace(SlotUpdate.first, SlotUpdate.second.first);

	for (auto& SlotUpdate : m_mapSlot_Feather)
		stSatDate.mapSlot_Feather.emplace(SlotUpdate.first, SlotUpdate.second.first);

	for (auto& SlotUpdate : m_mapSlot_Misc)
		stSatDate.mapSlot_Misc.emplace(SlotUpdate.first, SlotUpdate.second.first);

	m_pGameInstance->Set_UI_Talent_SaveData(stSatDate);


	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_TALENT);
}
