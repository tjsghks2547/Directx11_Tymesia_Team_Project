#include "pch.h"
#include "GameInstance.h"
#include "UI_Scene.h"
#include "UIGroup_Skill.h"

#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_TextBox.h"
#include "UI_Text.h"
#include "UI_Skill_Slot.h"
#include "PlayerSkillMgr.h"
#include "Player.h"

CUIGroup_Skill::CUIGroup_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Skill::CUIGroup_Skill(const CUIGroup_Skill& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Skill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Skill::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	m_eMyLevelID = static_cast<LEVELID>(pDesc->iCurLevel);
	
	m_pBaseScene = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill");
	m_pEquipCondition = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill_1Condition");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pEquipCondition, false);

	m_pPlayerSkillMgr = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER"))->Get_PlayerSkillMgr();

	Slot_Setting();


	CUI_Manager::UI_SAVE_SKILL stGetDate = { };
	if (m_pGameInstance->Get_UI_Skill_SaveData().mapSlotInfo.empty())
	{
		/* 최초에 UIgroupID를 매니저 컨테이너에 저장한다*/
		for (auto& SlotUpdate : m_mapSlotInfo)
			stGetDate.mapSlotInfo.emplace(SlotUpdate.first, SlotUpdate.second.first);
		m_pGameInstance->Set_UI_Skill_SaveData(stGetDate);

	}
	else
	{
		stGetDate.mapSlotInfo = m_pGameInstance->Get_UI_Skill_SaveData().mapSlotInfo;

		for (auto& SlotUpdate : m_mapSlotInfo)
		{
			for (auto& SlotData : stGetDate.mapSlotInfo)
			{
				if (SlotUpdate.first == SlotData.first)
				{
					SlotUpdate.second.first = SlotData.second;
					if(SlotUpdate.second.first)
						dynamic_cast<CUI_Skill_Slot*>(SlotUpdate.second.second)->Set_Slot_State(SKILL_OPEN_OFF);
					break;
				}
			}
		}

	}


	//m_pEquipWeapon = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip");
	//m_pEquipWeapon_2 = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip2");
	//m_pGameInstance->Set_All_UIObject_Condition_Open(m_pEquipWeapon_2, false);
	//Skill_Equip_Weapon();

	return S_OK; 
}

void CUIGroup_Skill::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipWeapon, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipCondition, true);

	}
}

void CUIGroup_Skill::Update(_float fTimeDelta)
{
	Slot_Update_State();
	if (m_bRenderOpen)
	{
		if (m_pGameInstance->isKeyEnter(DIK_ESCAPE) || m_bEscape)
		{
			m_bEscape = false;
			m_pGameInstance->Set_All_UIObject_Condition_Open(m_pEquipCondition, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pBaseScene, false);
			//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipWeapon, false);
			//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipWeapon_2, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipCondition, false);
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerSkill"), false);

			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
		}

		Slot_Contion_Check();

		for (auto& EscapeButton : m_pBaseScene->Find_UI_Button())
		{
			if (30 == EscapeButton->Get_UI_GroupID())
			{
				if (EscapeButton->Get_Mouse_Select_OnOff())
				{
					m_bEscape = true;
					EscapeButton->Set_Mouse_Select_OnOff(false);

				}
				break;
			}

		}
		
	}

}

void CUIGroup_Skill::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

HRESULT CUIGroup_Skill::Render()
{

	return S_OK;
}

void CUIGroup_Skill::Slot_Setting()
{
	for (auto& Button : m_pBaseScene->Find_UI_Button())
	{
		CUI_Skill_Slot* pSlot = dynamic_cast<CUI_Skill_Slot*>(Button);

		if (0 < Button->Get_UI_GroupID() &&
			20 > Button->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SKILL_CLOSE_OFF);
			m_mapSlotInfo.emplace(Button->Get_UI_GroupID(), make_pair(false, pSlot)); // 
			switch (Button->Get_UI_GroupID()) // 어떤 스킬로 해금되는지 설정
			{
			case 1:
				pSlot->Set_IconChange(1);
				pSlot->Set_Content(TEXT("도끼"));
				pSlot->Set_MySkill(PLAYER_SKILL_AXE);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_AXE);
				break;
			case 2:
				pSlot->Set_IconChange(11);
				pSlot->Set_Content(TEXT("핼버드"));
				pSlot->Set_MySkill(PLAYER_SKILL_HALBERD);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_HALBERD);
				break;
			case 3:
				pSlot->Set_IconChange(17);
				pSlot->Set_Content(TEXT("낫"));
				pSlot->Set_MySkill(PLAYER_SKILL_SCYTHE);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_SCYTHE);
				break;
			case 4:
				pSlot->Set_IconChange(14);
				pSlot->Set_Content(TEXT("어둠"));
				pSlot->Set_MySkill(PLAYER_SKILL_CANESWORD);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_CANESWORD);
				break;
			case 5:
				pSlot->Set_IconChange(10);
				pSlot->Set_Content(TEXT("대검"));
				pSlot->Set_MySkill(PLAYER_SKILL_GREADSWORD);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_GREADSWORD);
				break; 
			case 6:
				pSlot->Set_IconChange(20);
				pSlot->Set_Content(TEXT("투창검"));
				pSlot->Set_MySkill(PLAYER_SKILL_JAVELINSWORD);
				static_cast<CPlayerSkillMgr*>(m_pPlayerSkillMgr)->UnLockSkill(PLAYER_SKILL_JAVELINSWORD);
				break;
			default:
				pSlot->Set_Content(TEXT(""));
				break;
			}


		}
	
	}
}

void CUIGroup_Skill::Slot_Update_State()
{
	for (auto& SlotButton : m_mapSlotInfo)
	{
		CUI_Skill_Slot* pSlot = dynamic_cast<CUI_Skill_Slot*>(SlotButton.second.second);

		Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

		if (1 >= pSlot->Get_Slot_State())
		{
			SlotButton.second.first = true; // 스킬 해금이 되었으면 true
			if (m_deqOpenSkill.empty())
				m_deqOpenSkill.push_back(pSlot->Get_MySkill()); // 리볼빙을 위한 해금된 스킬 푸시백
			else
			{
				_bool bCheck = false;
				for (auto& SkillList : m_deqOpenSkill)
				{
					if (SkillList == pSlot->Get_MySkill())
						bCheck = true;
				}
				if (!bCheck)
				{
					m_deqOpenSkill.push_back(pSlot->Get_MySkill());
				}
			}
		}
		else
			SlotButton.second.first = false;

		if (pSlot->Get_Mouse_OnOff()) 
		{
			m_pGameInstance->Set_All_UIObject_Condition_Open(m_pEquipCondition, true);

			for (auto& Image : m_pBaseScene->Find_UI_Image())
			{
				if (1 == Image->Get_UI_GroupID())
				{
					Image->Set_Content(pSlot->Get_Content().c_str());
					break;
				}
			}


			Condition_Text_Update(pSlot->Get_NeedItem(), SlotButton.second.first); // 스킬이 진짜 해금 되었는지에 따라  텍스트 설명 내용 변경

			if (!lstrcmp(pSlot->Get_Content().c_str(), L""))
			{
				m_pGameInstance->Set_All_UIObject_Condition_Open(m_pEquipCondition, false);
				Condition_Text_Update(pSlot->Get_NeedItem(), true);

			}
		}


	}
}

void CUIGroup_Skill::Slot_Update_State_Value(SkILL_SLOTSTATE eSteteNum, CUI_Skill_Slot* pSlotUIObj)
{
	switch (eSteteNum)
	{
	case Client::SKILL_OPEN_ON:
		pSlotUIObj->Set_TexIconOff(false); 
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(2);
		break;
	case Client::SKILL_OPEN_OFF:
		pSlotUIObj->Set_TexIconOff(true);
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(0);
		break;
	case Client::SKILL_CLOSE_ON:
		pSlotUIObj->Set_TexIconOff(false); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);   // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1); // 이미지 랜더를 꺼야 함
		break;
	case Client::SKILL_CLOSE_OFF:
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(1);  // 이미지 랜더를 꺼야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	case Client::SKILL_OPEN_IDLE:
		pSlotUIObj->Set_TexIconOff(false); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(2);  // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(2);// 이미지 랜더를 꺼야 함
		break;

	}

}

void CUIGroup_Skill::Slot_Contion_Check()
{
	for (auto& SlotButton : m_mapSlotInfo)
	{
		CUI_Skill_Slot* pSlot = dynamic_cast<CUI_Skill_Slot*>(SlotButton.second.second);

		if (lstrcmp(pSlot->Get_Content().c_str(), L"") && 1 < pSlot->Get_Slot_State() && 3 <= m_pGameInstance->Get_Item_Count(pSlot->Get_NeedItem()))
			pSlot->Set_OpenContion(true); // 클릭 했을 때 스킬을 해금 할 수 있는지 여부에 대해서 체크 하는 값
		else
			pSlot->Set_OpenContion(false);

		SKill_Current_Slot(pSlot, SlotButton.second.first);

	}
}

void CUIGroup_Skill::Skill_Equip_Weapon()
{
	//for (auto& Button : m_pEquipWeapon->Find_UI_Button())
	//{
	//	if (1 == Button->Get_UI_GroupID())
	//	{
	//		CUI_Skill_Slot* pButton = dynamic_cast<CUI_Skill_Slot*>(Button);

	//		pButton->Set_TexIconOpen(true);
	//		pButton->Set_TexIconOff(true); // 이미지 흐리게 처리
	//		pButton->Set_TexSlot(2);
	//		pButton->Set_TexEdgeOff(true);
	//		pButton->Set_TexEdge(2);  // 이미지 랜더를 켜야 함
	//		pButton->Set_TexEffectOff(true);
	//		pButton->Set_TexEffect(2);// 이미지 랜더를 꺼야 함
	//		m_pEquipSkill_1 = Button;
	//	}
	//	if (10 == Button->Get_UI_GroupID())
	//	{
	//		Button->Set_OnOff(false);
	//	}
	//}
	//for (auto& Image : m_pEquipWeapon->Find_UI_Image())
	//{
	//	if (10 == Image->Get_UI_GroupID())
	//	{
	//		Image->Set_OnOff(false);
	//	}
	//}
	//for (auto& TextBox : m_pEquipWeapon->Find_UI_TextBox())
	//{
	//	if (10 == TextBox->Get_UI_GroupID())
	//	{
	//		TextBox->Set_OnOff(false);
	//	}
	//}
}

void CUIGroup_Skill::SKill_Current_Slot(CUI_Skill_Slot* pSkill, _bool bOpen)
{
	if (bOpen)
	{
		if (pSkill->Get_Mouse_Select_OnOff())
			m_pCurrentSkill = pSkill;
	}
	
}

void CUIGroup_Skill::Condition_Text_Update(ITEM_TYPE eItemtype, _bool bCheck)
{
	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* Text = { L"%d" };
	_uint iCount = m_pGameInstance->Get_Item_Count(eItemtype); // 보유하고 있는 아이템 개수

	for (auto& TextBox : m_pEquipCondition->Find_UI_TextBox())
	{
		/*if (1 == TextBox->Get_UI_GroupID())
		{
			if (bCheck)
			{
				TextBox->Set_OnOff(false);
			}
			else
			{
				TextBox->Set_OnOff(true);
				wsprintf(ChangeText, Text, iCount);

				TextBox->Set_Content(ChangeText);
				if (3 <= iCount)
					TextBox->Set_Change_TextColor(FONT_GREEN);
				else
					TextBox->Set_Change_TextColor(FONT_RED);
			}

		}*/
		if (2 == TextBox->Get_UI_GroupID())
		{
			if (bCheck)
				TextBox->Set_OnOff(false);
			else
			{
				TextBox->Set_OnOff(true);
				wsprintf(ChangeText, Text, iCount);
				TextBox->Set_TextDrawType(Engine::TEXT_TWOCOLOR);
				TextBox->Set_Content(ChangeText);
				TextBox->Set_Content2(TEXT(" / 3")); 
				TextBox->Set_Change_TextColor2(FONT_WHITE);
				if (3 <= iCount)
					TextBox->Set_Change_TextColor(FONT_GREEN);
				else
					TextBox->Set_Change_TextColor(FONT_RED);
			}
		}
		if (3 == TextBox->Get_UI_GroupID())
		{
			if (bCheck)
			{
				dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { 1000.f,450.f });
				TextBox->Set_Content(TEXT("스킬 해금이 완료되었습니다"));
				TextBox->Set_Change_TextColor(FONT_WHITE);
			}
			else
			{
				dynamic_cast<CTransform*>(TextBox->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { 1060.f,450.f });
				if (3 <= iCount)
				{
					TextBox->Set_Content(TEXT("스킬을 해금할 수 있습니다"));
					TextBox->Set_Change_TextColor(FONT_GREEN);
				}
				else
				{
					TextBox->Set_Content(TEXT("기술의 파편이 부족합니다"));
					TextBox->Set_Change_TextColor(FONT_RED);
				}
			}
		}

	}
}

HRESULT CUIGroup_Skill::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip2");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill_1Condition");
	return S_OK;
}

HRESULT CUIGroup_Skill::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_Skill::LoadData_UIText_Info(const _tchar* szSceneName)
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

CUIGroup_Skill* CUIGroup_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Skill* pInstance = new CUIGroup_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Skill::Clone(void* pArg)
{
	CUIGroup_Skill* pInstance = new CUIGroup_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Skill::Free()
{
	CUI_Manager::UI_SAVE_SKILL stSatDate = { };

	for (auto& SlotUpdate : m_mapSlotInfo)
		stSatDate.mapSlotInfo.emplace(SlotUpdate.first, SlotUpdate.second.first);

	m_pGameInstance->Set_UI_Skill_SaveData(stSatDate);

	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_SKILL);
}
