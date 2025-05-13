#include "pch.h"
#include "UIGroup_Inventory.h"
#include "UI_Scene.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "UI_ItemBackground.h"
#include "UI_Item_Icon.h"
#include "UI_ButtonHighlight.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_UnderLine.h"
#include "UI_Arrow.h"
#include "UIGroup_PlayerScreen.h"

#include "Player.h"

CUIGroup_Inventory::CUIGroup_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Inventory::CUIGroup_Inventory(const CUIGroup_Inventory& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Inventory::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Inventory::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;


	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMyBaseScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase");
	m_pItemScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_Inven_ItemUse");

	// ���� ������ �ʿ��� ui�� ���ʿ��� off �ϵ��� ����
	m_pItemUsePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemUse_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemUsePopUp, false);

	m_pItemTypePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemType_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemTypePopUp, false);

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER");
	


	Ready_MiniView_ItemInfo(); // ������ ���� ���� �κ� ��� �ٲ�ϱ� ������ �����ؼ� ���
	Set_Item_Default_Info(); // ������ ui ���� ������ ���� ������ ���� ���� �����̳� �ϳ� �غ�

	return S_OK;
}

void CUIGroup_Inventory::Priority_Update(_float fTimeDelta)
{
	// �׷쿡�� ���� ������ �Ǵ� ���� ������ ��� �ٸ� �͵鵵 �������� => �ش� ������ ������Ʈ�� �Ҵٴ� ��
	
	if (m_bRenderOpen)
	{

		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp, true); // �ڵ����� uiobj�� ���� scene ������ ���µǵ��� �Լ��� ����
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, true);
		dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(true);

	}
	else if(!m_pGameInstance->UIGroup_Render_State(LEVEL_STATIC, TEXT("Layer_Mouse")))
	{
		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // ���콺 �̹��� ����
		m_bItemUsePopOpen = false; // ������ ���/������ �˾� off
		m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemUsePopUp, false);
		m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemTypePopUp, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, false);
		m_iMouseOnLastSlot = 0; // ���� ������ �� �ʱ�ȭ
		dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);
	}
}

void CUIGroup_Inventory::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		for (auto& EscapeButton : m_pMyBaseScene->Find_UI_Button())
		{
			if (1 == EscapeButton->Get_UI_GroupID())
			{
				if (EscapeButton->Get_Mouse_Select_OnOff())
				{
					EscapeButton->Set_Mouse_Select_OnOff(false);
					m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // ���콺 �̹��� ����
					m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerInventory"), false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMyBaseScene, false);
					m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevel, TEXT("Layer_PlayerScreen"), true);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

					dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevel, TEXT("Layer_Player"), "PLAYER"))->Set_UI_End(false);

				}
				break;
			}
		}
		Change_UI_Item_Tab(); // ������ �Ǿ��� ������ ���� ���� ���� �� ���� ���� ���� ����
		if (!m_bItemUsePopOpen)// ������ ���/ ������ �˾��� ���� ���� �ʴٸ�
		{
			Update_Get_ItemMgr(); //������ �Ŵ����� ���� �ִ� ���� ������ ����� ������
			Update_ItemInfo(); // ���� ������ Ÿ�� ������ ���� �������� �ΰ����� ����� ä���ֱ�

			if (!m_bItemTypePopOpen) // ������ Ÿ�� �˾��� ���� ���� �ʴٸ�
			{
				if (m_bCommonOpen) // ������ �� �������� ��
					Slot_Button_MouseOn_Check(m_InvenItemCommon); // ������ ���콺�� ����Ű����
				if (m_bSkillOpen) // ��ų �� ���� ���� ��
					Slot_Button_MouseOn_Check(m_InvenItemSkill);
				if (m_bStoryOpen) // ��ų �� ���� ���� ��
					Slot_Button_MouseOn_Check(m_InvenItemStory);
				if (m_bHerbOpen) // ��ų �� ���� ���� ��
					Slot_Button_MouseOn_Check(m_InvenItemHerb);
			}
			// ���콺 On ���� true �� �༮�� ã�� ���� ���� ����

			/* ������ Ÿ�� �˾��� ����̱⿡ ���⼭ ���� ��ư ������ ������ �ϰ� ����*/
			if (m_bCommonOpen)
				Slot_Button_Select_Check(m_InvenItemCommon); // ������ ���콺�� ��������
			if (m_bSkillOpen)
				Slot_Button_Select_Check(m_InvenItemSkill);
			if (m_bStoryOpen)
				Slot_Button_Select_Check(m_InvenItemStory);
			if (m_bHerbOpen)
				Slot_Button_Select_Check(m_InvenItemHerb);
			
			if (m_bItemTypePopOpen)
				ItemType_PopUP_Button(); // ������ Ÿ�� �˾��� ���� �ִٸ� ��ư ������ Ȱ��ȭ
		}
		else
		{
			if (1 == m_iPopUpOpenNum)// ������ ��� �˾��� ���� �ִٸ� ������ ��� �Լ� üũ
				ItemUse_PopUP_Use_Button(); // ������ ������ �˾��� ���� �ִٸ� ������ ������ �Լ� üũ
			else if (2 == m_iPopUpOpenNum)
				ItemUse_PopUP_Drop_Button();
		}
	}
}

void CUIGroup_Inventory::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		
	}
}

HRESULT CUIGroup_Inventory::Render()
{
	return S_OK;
}

HRESULT CUIGroup_Inventory::Ready_UIObject()
{
	//// ESC �޴� ���
	//if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_EscMenuBase")))
	//	return E_FAIL;

	//// �κ� ������ ���� ����
	//if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_Inven_ItemUse")))
	//	return E_FAIL;

	//// �κ� ������ ��� ������ ���� �˾�
	//if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemUse_PopUp")))
	//	return E_FAIL;

	////�κ� ������ Ÿ�Կ� ���� �˾�
	//if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemType_PopUp")))
	//	return E_FAIL;



	LoadData_UIObject(LEVEL_STATIC, UISCENE_INVEN, L"UIScene_EscMenuBase");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_INVEN, L"UIScene_Inven_ItemUse");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemUse_PopUp");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemType_PopUp");




	return S_OK;
}

HRESULT CUIGroup_Inventory::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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


	//MessageBox(g_hWnd, L"Load �Ϸ�", _T("����"), MB_OK);
	return S_OK;
}

void CUIGroup_Inventory::Change_UI_Item_Tab()
{
	if (m_bCommonOpen)
		Itme_View_Inventory(m_InvenItemCommon); // �Ϲ� �������� ���Կ� ����

	if (m_bSkillOpen)
		Itme_View_Inventory(m_InvenItemSkill);// ��ų �������� ���Կ� ����

	if (m_bStoryOpen)
		Itme_View_Inventory(m_InvenItemStory);// �̾߱� �������� ���Կ� ����

	if (m_bHerbOpen)
		Itme_View_Inventory(m_InvenItemHerb);// ��� �������� ���Կ� ����


	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		CUI_UnderLine* pButton = dynamic_cast<CUI_UnderLine*>(Button);

		if (10 == Button->Get_UI_GroupID()) // �Ϲ� 
		{
			if (pButton->Get_Mouse_Select_OnOff()) // �Ϲ� ��ư�� ���� ��
			{
				m_bCommonOpen = true;
				m_bStoryOpen = false;
				m_bSkillOpen = false;
				m_bHerbOpen = false;
				pButton->Set_Open_Image(true);
			}

		}
		if (20 == Button->Get_UI_GroupID())// �̾߱�
		{
			if (pButton->Get_Mouse_Select_OnOff()) // �̾߱� ��ư�� ���� ��
			{
				m_bCommonOpen = false;
				m_bStoryOpen = true;
				m_bSkillOpen = false;
				m_bHerbOpen = false;
				pButton->Set_Open_Image(true);
			}

		}
		if (30 == Button->Get_UI_GroupID()) // ����� ����
		{
			if (pButton->Get_Mouse_Select_OnOff()) // ����� ���� ��ư�� ���� ��
			{
				m_bCommonOpen = false;
				m_bStoryOpen = false;
				m_bSkillOpen = true;
				m_bHerbOpen = false;
				pButton->Set_Open_Image(true);

			}
		}
		if (40 == Button->Get_UI_GroupID()) // ���
		{
			if (pButton->Get_Mouse_Select_OnOff()) // �̾߱� ��ư�� ���� ��
			{
				m_bCommonOpen = false;
				m_bStoryOpen = false;
				m_bSkillOpen = false;
				m_bHerbOpen = true;
				pButton->Set_Open_Image(true);
			}

		}
		if (!m_bCommonOpen)
		{
			if (10 == Button->Get_UI_GroupID()) // �Ϲ� 
			{
				pButton->Set_Open_Image(false);
			}
		}
		if (!m_bStoryOpen)
		{
			if (20 == Button->Get_UI_GroupID()) // �̾߱� 
			{
				pButton->Set_Open_Image(false);
			}
		}
		if (!m_bSkillOpen)
		{
			if (30 == Button->Get_UI_GroupID()) // ����� ����
			{
				pButton->Set_Open_Image(false);
			}
		}
		if (!m_bHerbOpen)
		{
			if (40 == Button->Get_UI_GroupID()) // ��� 
			{
				pButton->Set_Open_Image(false);
			}
		}

	}
}

void CUIGroup_Inventory::Ready_MiniView_ItemInfo()
{
	// ���� ���� �⺻ ������ü �̸� �����ͼ� �����ϱ�
	for (auto& TextBox : m_pItemScene->Find_UI_TextBox())
	{
		_tchar* pTemp = { L"" };
		if (200 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxName = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
		}
		if (210 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxType = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
		}
		if (220 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCount = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
		}
		if (221 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCountNum = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
		}

		if (230 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxDesc = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
		}
		if (nullptr != m_pItemTextBoxName &&
			nullptr != m_pItemTextBoxDesc)
		{
			break;
		}
	}

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}
}

void CUIGroup_Inventory::Itme_View_Inventory(vector<UI_Item>& vecContainer)
{
	// ������ ������ ������ �����Ѵ�
	sort(vecContainer.begin(), vecContainer.end(), [](UI_Item a, UI_Item b) {return a.ItemType < b.ItemType; }); // ������ Ÿ�� ���� �������� ����


	// �κ��丮 ���� ���빰 �ʱ�ȭ
	_int iOpenSlotCount = 100; // ���� ��ȣ
	for (auto& Slot : m_pItemScene->Find_UI_Button()) // ���� ��������
	{
		if (iOpenSlotCount == Slot->Get_UI_GroupID()) // ù ��° ���Ժ��� ���� ä���
		{
			_tchar ChangeText[MAX_PATH] = { L"" };

			dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(false);
			dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
			iOpenSlotCount++;

		}
	}
	iOpenSlotCount = 100;

	// �κ��丮�� ȹ���� �����۵��� �����ش�
	for (auto& InvenItem : vecContainer) // ������ ���� ���鼭 ���Կ� �ֱ�
	{
		for (auto& Slot : m_pItemScene->Find_UI_Button()) // ���� ��������
		{
			if (iOpenSlotCount == Slot->Get_UI_GroupID()) // ù ��° ���Ժ��� ���� ä���
			{
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon(InvenItem.ItemIconNum);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(true);

				_tchar ChangeText[MAX_PATH] = {};
				const _tchar* Text = { L"x%d" };

				wsprintf(ChangeText, Text, InvenItem.ItemCount);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
				iOpenSlotCount++;
				break;
			}
		}
	}
}

void CUIGroup_Inventory::Slot_Button_Select_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		if (100 <= Button->Get_UI_GroupID() && 125 > Button->Get_UI_GroupID()) // slotbutton
		{
			CUI_ItemBackground* pButton = dynamic_cast<CUI_ItemBackground*>(Button);
			if (pButton->Get_Mouse_Select_OnOff())
			{
				// ��ư�� �� ���콺�� ������ �༮�� �ִ�!
				pButton->Set_Mouse_Select_OnOff(false); // 1ȸ üũ�� �� ���̱⿡ ���� ����

				for (auto& Item : vecContainer)
				{
					if (Item.ItemIconNum == pButton->Get_Item_Icon())
					{
						m_CurrentItemInfo = Item; // ������ ������ ����
						_float3 ButtonPos = dynamic_cast<CTransform*>(pButton->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
						ItemType_PopUP_Open(Item.ItemType, ButtonPos);
						break;
					}
				}
			}
		}
	}

	
}

void CUIGroup_Inventory::Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Slot : m_pItemScene->Find_UI_Button())
	{
		CUI_ItemBackground* pSlot = dynamic_cast<CUI_ItemBackground*>(Slot);

		if (100 <= Slot->Get_UI_GroupID() && 125 > Slot->Get_UI_GroupID())
		{
			if (pSlot->Get_Slot_Mouse_OnOff()) // �ش� ���Կ� ���콺�� �÷��� �ִ�
			{
				for (auto& Image : m_pItemScene->Find_UI_Image())
				{
					if (100 == Image->Get_UI_GroupID()) // ȣ�� �̹����� ã�Ƽ�
					{
						_float3 fPos = dynamic_cast<CTransform*>(Slot->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);

						CUI_SquareFrame_Hover* pImage = dynamic_cast<CUI_SquareFrame_Hover*>(Image);
						dynamic_cast<CTransform*>(pImage->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fPos.x,fPos.y });
						pImage->Set_Hover_ImageOn(true);
						m_iMouseOnLastSlot = pSlot->Get_UI_GroupID();
					}
				}

				if (pSlot->Get_Item_Icon_OnOff()) // ���Կ� �������� �������� �ִ���
				{
					for (auto& Item : vecContainer)
					{
						if (Item.ItemIconNum == pSlot->Get_Item_Icon()) // ���Կ� �ִ� �������� �̹����� ���� ������ ���� ��������
							Connect_MiniView_ItemInfo(Item); // ui�� ����
					}
				}
				else
				{
					DisConnect_MiniView_ItemInfo();
				}

			}

		}

	}


	for (auto& Slot : m_pItemScene->Find_UI_Button())
	{
		if (m_iMouseOnLastSlot == Slot->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon_OnOff()) // ���Կ� �������� �������� �ִ���
			{
				for (auto& Item : vecContainer)
				{
					if (Item.ItemIconNum == dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon()) // ���Կ� �ִ� �������� �̹����� ���� ������ ���� ��������
						Connect_MiniView_ItemInfo(Item); // ui�� ����
				}
			}
			else
			{
				DisConnect_MiniView_ItemInfo();
			}
		}
	}
}

void CUIGroup_Inventory::ItemType_PopUP_Open(ITEM_TYPE eItemType, _float3 fSetPos)
{
	_float3 fImagePos = {};

	switch (eItemType)
	{
	case ITEM_TYPE::ITEM_KEY1: // �ƹ��͵� �� ��
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_KEY2: // �ƹ��͵� �� ��
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_MEMORY:// ��� ������ ���� ��
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y + 40.f });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:// �ƹ��͵� �� ��
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:// ������, ���� ��
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				Button->Set_OnOff(false);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	default: //�󽽷�
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
		//case 3: //ITEM_ONLYUSE // ����ϱ�, ���� �� // �� ��ȣ��� ���� Į ������
		//	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		//	{
		//		if (300 == Image->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
		//			fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		//			Image->Set_OnOff(true);
		//		}
		//		if (400 == Image->Get_UI_GroupID())
		//		{
		//			Image->Set_OnOff(true);
		//		}
		//	}
		//	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		//	{
		//		if (310 == Button->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
		//			Button->Set_OnOff(true);
		//		}
		//		if (320 == Button->Get_UI_GroupID())
		//		{
		//			Button->Set_OnOff(false);
		//		}
		//		if (330 == Button->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
		//			Button->Set_OnOff(true);
		//		}
		//	}
		//	m_bItemTypePopOpen = true;
		//	break;
	}


	
}

void CUIGroup_Inventory::ItemType_PopUP_State(_bool bOpen)
{

	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
		if (400 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
	}
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (310 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (320 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (330 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
	}
}

void CUIGroup_Inventory::ItemType_PopUP_Button()
{
	// �˾� ���� ���� => �̹��� �ٱ� ����
	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_SquareFrame*>(Image)->Get_Mouse_Side_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
				m_CurrentItemInfo = {};
			}
		}
	}
	// �˾� ���� ���� => ���� ��ư Ŭ��
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (330 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
				m_CurrentItemInfo = {};
			}
		}
	}

	// ��� ��ư Ŭ��
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (310 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iPopUpOpenNum = 1;
				ItemUse_PopUP_Open();

			}
		}
	}
	// ������ ��ư Ŭ��
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (320 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iPopUpOpenNum = 2;
				ItemUse_PopUP_Open();




			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_PopUP_Open()
{
	_float3 fImagePos = {};


	for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
	{
		Image->Set_OnOff(true);
	}

	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		Button->Set_OnOff(true);
	}

	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		TextBox->Set_OnOff(true);
	}


	m_bItemUsePopOpen = true;
}

void CUIGroup_Inventory::ItemUse_PopUP_Use_Button()
{
	m_iItemMaxCount = m_CurrentItemInfo.ItemCount;

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%s";
	const _tchar* CountNum = L"%d";
	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		if (0 == TextBox->Get_UI_GroupID())
		{
			TextBox->Set_Content(L"���");
		}
		if (1 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountText, m_CurrentItemInfo.ItemName);
			TextBox->Set_Content(ChangeText);
			TextBox->Set_Change_TextColor(FONT_GREEN);
		}
		if (2 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountNum, m_iItemStatCount);
			TextBox->Set_Content(ChangeText);

		}
	}
	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		if (10 == Button->Get_UI_GroupID()) // ���� ����
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount -= 1;
				if (m_iItemStatCount == 0)
				{
					m_iItemStatCount = m_iItemMaxCount;
				}

			}

		}
		if (20 == Button->Get_UI_GroupID()) // ���� ����
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_iItemMaxCount > m_iItemStatCount)
				{
					m_iItemStatCount += 1;
				}
			}
		}


		if (30 == Button->Get_UI_GroupID()) // ������ ���
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_pGameInstance->Use_Item(m_CurrentItemInfo.ItemType, m_iItemStatCount))
				{
					for (auto It = m_InvenItemCommon.begin(); It != m_InvenItemCommon.end();)
					{
						if (It->ItemType == m_CurrentItemInfo.ItemType)
							It = m_InvenItemCommon.erase(It);
						else
							++It;
					}
				}
				Item_Use_Set(m_CurrentItemInfo.ItemType, m_iItemStatCount);
				ItemUse_Update();
				ItemType_PopUP_State(false);
				m_iItemStatCount = 1;
				m_bItemTypePopOpen = false;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
		if (40 == Button->Get_UI_GroupID())// â ����
		{

			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount = 1;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_PopUP_Drop_Button()
{
	m_iItemMaxCount = m_CurrentItemInfo.ItemCount;

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%s";
	const _tchar* CountNum = L"%d";
	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		if (0 == TextBox->Get_UI_GroupID())
		{
			TextBox->Set_Content(L"������");
		}
		if (1 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountText, m_CurrentItemInfo.ItemName);
			TextBox->Set_Content(ChangeText);
			TextBox->Set_Change_TextColor(FONT_GREEN);
		}
		if (2 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountNum, m_iItemStatCount);
			TextBox->Set_Content(ChangeText);

		}
	}
	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		if (10 == Button->Get_UI_GroupID()) // ���� ����
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount -= 1;
				if (m_iItemStatCount == 0)
				{
					m_iItemStatCount = m_iItemMaxCount;
				}

			}

		}
		if (20 == Button->Get_UI_GroupID()) // ���� ����
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_iItemMaxCount > m_iItemStatCount)
				{
					m_iItemStatCount += 1;
				}
			}
		}


		if (30 == Button->Get_UI_GroupID()) // ������ ������
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_pGameInstance->Use_Item(m_CurrentItemInfo.ItemType, m_iItemStatCount)) // �������� ����Ǿ� �ִ� �����̳ʿ��� Item_type�� �´� �༮�� ������ ����
				{
					Update_Get_ItemMgr(); // �����̳ʿ��� ���� �����ϱ� �κ��丮 ���� ���� �ٽ� ������Ʈ
				}
				m_pGameInstance->Drop_Item(m_CurrentItemInfo.ItemType, dynamic_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION), m_pPlayer, m_iItemStatCount);

				ItemUse_Update();
				ItemType_PopUP_State(false);
				m_iItemStatCount = 1;
				m_bItemTypePopOpen = false;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
		if (40 == Button->Get_UI_GroupID())// â ����
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount = 1;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_Update()
{
	_int iOpenSlotCount = 100; // ���� ��ȣ
	for (auto& Slot : m_pItemScene->Find_UI_Button()) // ���� ��������
	{
		if (iOpenSlotCount == Slot->Get_UI_GroupID())
		{
			const _tchar* Text = { L"x%d" };

			if (lstrcmp(Slot->Get_Content().c_str(), L"x0"))
			{
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(false);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(L"");
				iOpenSlotCount++;
			}
		}
	}
	if (m_bCommonOpen)
		Itme_View_Inventory(m_InvenItemCommon); // �Ϲ� �������� ���Կ� ����

	if (m_bSkillOpen)
		Itme_View_Inventory(m_InvenItemSkill);// ��ų �������� ���Կ� ����

	if (m_bStoryOpen)
		Itme_View_Inventory(m_InvenItemStory);// �̾߱� �������� ���Կ� ����

	if (m_bHerbOpen)
		Itme_View_Inventory(m_InvenItemHerb);// ��� �������� ���Կ� ����


}

void CUIGroup_Inventory::Item_Use_Set(ITEM_TYPE etype, _uint iCount)
{
	switch (etype)
	{
	case Engine::ITEM_TYPE::ITEM_KEY1:
		break;
	case Engine::ITEM_TYPE::ITEM_KEY2:
		break;
	case Engine::ITEM_TYPE::ITEM_MEMORY:
		dynamic_cast<CPlayer*>(m_pPlayer)->Increase_MemoryFragment(iCount * 1000);
		break;
	case Engine::ITEM_TYPE::ITEM_FORGIVEN:
		break;
	case Engine::ITEM_TYPE::ITEM_SKILLPIECE:
		break;
	case Engine::ITEM_TYPE::ITEM_END:
		break;
	default:
		break;
	}


}

void CUIGroup_Inventory::Update_Get_ItemMgr()
{
	UI_Item ItemBox = {};
	m_InvenItemCommon.clear();
	m_InvenItemSkill.clear();
	m_InvenItemStory.clear();
	m_InvenItemHerb.clear();

	m_ItemMgrContainerRef = m_pGameInstance->Get_Item_Info(); // ������ �Ŵ������� ���� ��������

	for (auto& ItemInfo : m_ItemMgrContainerRef)
	{
		ItemBox.ItemType = ItemInfo.first;
		ItemBox.ItemCount = ItemInfo.second.first;

		if (0 != ItemBox.ItemCount)
		{
			switch (ItemBox.ItemType)
			{
			case ITEM_TYPE::ITEM_KEY1:
				m_InvenItemCommon.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_KEY2:
				m_InvenItemCommon.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_MEMORY:
				m_InvenItemCommon.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_SKILLPIECE:
				m_InvenItemSkill.push_back(ItemBox);
				break;

			case ITEM_TYPE::ITEM_FIELDITEM:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_BADZOKER:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_BEOMSEUNG:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_FAKER:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_UIN:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_PLAYERMASTER:
				m_InvenItemStory.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_YUBHIN:
				m_InvenItemStory.push_back(ItemBox);
				break;

			case ITEM_TYPE::ITEM_HERB_1:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_2:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_3:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_4:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_5:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_6:
				m_InvenItemHerb.push_back(ItemBox);
				break;
			case ITEM_TYPE::ITEM_HERB_7:
				m_InvenItemHerb.push_back(ItemBox);
				break;

			}
		}
	}
}

void CUIGroup_Inventory::Update_ItemInfo()
{
	UI_Item eItemInfo = {};
	
	
	for (auto& InvenInfo : m_InvenItemCommon)
	{
		auto iter = find_if(m_vecItemDefaultInfo.begin(), m_vecItemDefaultInfo.end(), [&](UI_Item stItemInfo){
			return stItemInfo.ItemType == InvenInfo.ItemType;
			});
		
		switch (InvenInfo.ItemType)
		{
		case ITEM_TYPE::ITEM_KEY1:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		case ITEM_TYPE::ITEM_KEY2:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		case ITEM_TYPE::ITEM_MEMORY:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		case ITEM_TYPE::ITEM_FORGIVEN:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName =  (*iter).ItemName;
			InvenInfo.ItemDesc =  (*iter).ItemDesc;
			break;
		default:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName =  (*iter).ItemName;
			InvenInfo.ItemDesc =  (*iter).ItemDesc;
			break;

		}
	}
	for (auto& InvenInfo : m_InvenItemSkill)
	{
		auto iter = find_if(m_vecItemDefaultInfo.begin(), m_vecItemDefaultInfo.end(), [&](UI_Item stItemInfo) {
			return stItemInfo.ItemType == InvenInfo.ItemType;
			});
		switch (InvenInfo.ItemType)
		{
		case ITEM_TYPE::ITEM_SKILLPIECE:
			InvenInfo.ItemIconNum = 7;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		}
	}
	for (auto& InvenInfo : m_InvenItemStory)
	{
		auto iter = find_if(m_vecItemDefaultInfo.begin(), m_vecItemDefaultInfo.end(), [&](UI_Item stItemInfo) {
			return stItemInfo.ItemType == InvenInfo.ItemType;
			});
		switch (InvenInfo.ItemType)
		{
		case ITEM_TYPE::ITEM_BADZOKER:
			InvenInfo.ItemIconNum = 8;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_BEOMSEUNG:
			InvenInfo.ItemIconNum = 9;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_FAKER:
			InvenInfo.ItemIconNum = 10;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_UIN:
			InvenInfo.ItemIconNum =11;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_PLAYERMASTER:
			InvenInfo.ItemIconNum = 12;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_YUBHIN:
			InvenInfo.ItemIconNum = 13;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;	
		case ITEM_TYPE::ITEM_FIELDITEM:
			InvenInfo.ItemIconNum = 8;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		}
	}

	for (auto& InvenInfo : m_InvenItemHerb)
	{
		auto iter = find_if(m_vecItemDefaultInfo.begin(), m_vecItemDefaultInfo.end(), [&](UI_Item stItemInfo) {
			return stItemInfo.ItemType == InvenInfo.ItemType;
			});
		switch (InvenInfo.ItemType)
		{

		case ITEM_TYPE::ITEM_HERB_1:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			
			break;
			
		case ITEM_TYPE::ITEM_HERB_2:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
			
		case ITEM_TYPE::ITEM_HERB_3:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
			
		case ITEM_TYPE::ITEM_HERB_4:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
			
		case ITEM_TYPE::ITEM_HERB_5:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
			
		case ITEM_TYPE::ITEM_HERB_6:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
			
		case ITEM_TYPE::ITEM_HERB_7:
			InvenInfo.ItemIconNum = (*iter).ItemIconNum;
			InvenInfo.ItemName = (*iter).ItemName;
			InvenInfo.ItemDesc = (*iter).ItemDesc;
			break;
		}
	}

}

void CUIGroup_Inventory::Connect_MiniView_ItemInfo(UI_Item ItemInfo)
{
	_tchar* pTemp = {};

	// ���콺�� �ö� ���� �� �ش� �������� ������ ��������
	// �̸� ������ ����

	//�ؽ�Ʈ �ڽ� n���� ������ �̸� ���
	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(ItemInfo.ItemName);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(ItemInfo.ItemDesc);

	_tchar* pSor = { L"%d" };
	_tchar pDust[MAX_PATH] = {};
	wsprintf(pDust, pSor, ItemInfo.ItemCount);
	pTemp = L"����";
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pDust);

	switch (ItemInfo.ItemType)
	{
	case ITEM_TYPE::ITEM_KEY1:
		pTemp = L"�Ϲ�";
		break;
	case ITEM_TYPE::ITEM_KEY2:
		pTemp = L"�Ϲ�";
		break;
	case ITEM_TYPE::ITEM_MEMORY:
		pTemp = L"�Һ� ������";
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:
		pTemp = L"�Һ� ������";
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:
		pTemp = L"����� ����";
		break;
	case ITEM_TYPE::ITEM_FIELDITEM:
		pTemp = L"Ư���� ������";
		break;
	case ITEM_TYPE::ITEM_BADZOKER:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_BEOMSEUNG:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_FAKER:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_UIN:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_PLAYERMASTER:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_YUBHIN:
		pTemp = L"��� �̾߱�";
		break;
	case ITEM_TYPE::ITEM_HERB_1:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_2:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_3:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_4:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_5:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_6:
		pTemp = L"���";
		break;
	case ITEM_TYPE::ITEM_HERB_7:
		pTemp = L"���";
		break;
	default:
		pTemp = L"ä���ֱ�";
		break;

	}
	

	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(true);
		}
		if (250 == image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_TexNumber(ItemInfo.ItemIconNum);
		}
	}

}

void CUIGroup_Inventory::DisConnect_MiniView_ItemInfo()
{
	_tchar* pTemp = { L"" };

	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}

}

CUIGroup_Inventory* CUIGroup_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Inventory* pInstance = new CUIGroup_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Inventory::Clone(void* pArg)
{
	CUIGroup_Inventory* pInstance = new CUIGroup_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Inventory::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_INVEN);

}

void CUIGroup_Inventory::Set_Item_Default_Info()
{
	/* �÷��̾� ��ũ�� => ������ �˸� �˾��� �������� ����*/
	UI_Item SaveData = {};
	SaveData.ItemType = ITEM_TYPE::ITEM_KEY1;
	SaveData.ItemIconNum = 1;
	SaveData.ItemName = L"����A";
	SaveData.ItemDesc = L"���� �Դϴ�.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_KEY2;
	SaveData.ItemIconNum = 3;
	SaveData.ItemName = L"����B";
	SaveData.ItemDesc = L"���� �Դϴ�.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_MEMORY;
	SaveData.ItemIconNum = 5;
	SaveData.ItemName = L"����� ������";
	SaveData.ItemDesc = L"�� �������� ����ϸ� ����� ������ ȹ���մϴ�.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_FORGIVEN;
	SaveData.ItemIconNum = 4;
	SaveData.ItemName = L"������ ����";
	SaveData.ItemDesc = L"������ ������ ����ϸ�, ���̼��̿� ��ȭ�ϰų� ��ȣ�⸦ ����Ͽ�\n�ڸ������� ������ ���¸� �ʱ�ȭ�� �� �ֽ��ϴ�.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_SKILLPIECE;
	SaveData.ItemIconNum = 7;
	SaveData.ItemName = L"������ ���";
	SaveData.ItemDesc = L"����� ������ ����� �����Ͽ� ��ȣ�⿡�� ���� ���⸦ �����ϰų�\n���׷��̵� �ϼ���";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_FIELDITEM;
	SaveData.ItemIconNum = 8;
	SaveData.ItemName = L"������ �̾߱�";
	SaveData.ItemDesc = L"���̼��̰� ã�� �ִ� å�� �Ϻ��̴�.\n �̾߱⸦ ��� �������ָ� ���� ���� ���� �� ����";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_BADZOKER;
	SaveData.ItemIconNum = 8;
	SaveData.ItemName = L"BadZoker";
	SaveData.ItemDesc = L"�츮�� ��� Ȱ¦ ���� �� ���̾� HAHAHA";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_BEOMSEUNG;
	SaveData.ItemIconNum = 9;
	SaveData.ItemName = L"Monster�� ��Ӵ�";
	SaveData.ItemDesc = L"Monster�� ź������� �������� �����Է�";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_FAKER;
	SaveData.ItemIconNum = 10;
	SaveData.ItemName = L"FAKER";
	SaveData.ItemDesc = L"��.��.��.\n??? : ���� ���ٷ�?";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_UIN;
	SaveData.ItemIconNum = 11;
	//SaveData.ItemName = L"���� ����";
	SaveData.ItemName = L"���ָ����θ���";
	SaveData.ItemDesc = L"��� ����߽��ϴ�.\n��������� ������!";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_PLAYERMASTER;
	SaveData.ItemIconNum = 12;
	SaveData.ItemName = L"����������";
	SaveData.ItemDesc = L"�پ�� ����!\n���� ���� ����!\n�ϴ�(���)�� ���� �;��";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_YUBHIN;
	SaveData.ItemIconNum = 13;
	SaveData.ItemName = L"�׳��� UI";
	SaveData.ItemDesc = L"Code\n�� ġ�� �;����! �������� ������ ����?";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_1;
	SaveData.ItemIconNum = 14;
	SaveData.ItemName = L"����";
	SaveData.ItemDesc = L"�ѱ����� ��������.";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_2;
	SaveData.ItemIconNum = 15;
	SaveData.ItemName = L"ȸ��";
	SaveData.ItemDesc = L"ó������ Ǯ�̿���..";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_3;
	SaveData.ItemIconNum = 16;
	SaveData.ItemName = L"����";
	SaveData.ItemDesc = L"��ũ��..���ɳ��� ���� �;��....";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_4;
	SaveData.ItemIconNum = 17;
	SaveData.ItemName = L"����";
	SaveData.ItemDesc = L"���� �ó����� �Ⱦ��.";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);
	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_5;
	SaveData.ItemIconNum = 18;
	SaveData.ItemName = L"������";
	SaveData.ItemDesc = L"��ŷ��� ��.��.��.\n���߰� ���� ������.";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_6;
	SaveData.ItemIconNum = 19;
	SaveData.ItemName = L"����";
	SaveData.ItemDesc = L"������ ��࿡�� ���� ��� �� �Ծ����.";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);

	
	SaveData.ItemType = ITEM_TYPE::ITEM_HERB_7;
	SaveData.ItemIconNum = 20;
	SaveData.ItemName = L"Ÿ��";
	SaveData.ItemDesc = L"������ ������ũ ���� �� ������ ����ؿ�.";
	SaveData.ItemCount = 0;
	m_vecItemDefaultInfo.push_back(SaveData);





}

