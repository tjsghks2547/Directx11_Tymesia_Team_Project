#include "pch.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "Player.h"

#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "UI_ButtonBackground.h"
#include "UI_ButtonHighlight.h"
#include "UI_KeyBox_Long.h"

CUIGroup_PlayerLevelUP::CUIGroup_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerLevelUP::CUIGroup_PlayerLevelUP(const CUIGroup_PlayerLevelUP& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerLevelUP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Initialize(void* pArg)
{

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevelID = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER");


	//���� ������ �ʿ��� ui�� ���ʿ��� off �ϵ��� ����
	m_pApplyPopUp = m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_2");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pApplyPopUp, false);

	m_pResetPopUp = m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_3");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pResetPopUp, false);

	//dynamic_cast<CPlayer*>(m_pPlayer)->Increase_MemoryFragment(3000);
	Setting_Default_Status_Value();



	return S_OK;
}

void CUIGroup_PlayerLevelUP::Priority_Update(_float fTimeDelta)
{
	if (m_iMemoryCurrentCount != dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment())
	{
		// ���� ���� �� �����ͼ� ������ �ٸ��� ���� ����
		m_iMemoryCurrentCount = dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment();
		m_iMemoryNextCount = m_iMemoryCurrentCount; // ������ ��ŭ ����
	}
	if (m_bRenderOpen)
	{
		if (m_pMyScene->Get_Scene_Render_State())
		{
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pApplyPopUp, true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pResetPopUp, true);
		}
		else
		{
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pApplyPopUp, false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pResetPopUp, false);
		}
	
		
	}
}

void CUIGroup_PlayerLevelUP::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_iCurrentMemoryNeed = (500 + (30 * (m_iNextLevel - 1)));
		m_iNextMemoryNeed = (500 + (30 * m_iNextLevel));

		Button_Input_Check();
		LevelUP_Apply_Button();

		Button_Render_Check();
		Button_Image_On_Check();
		Button_Render_Check();
		LevelUP_Memory_Check();
		LevelUP_Status_Check();
		LevelUP_Status_Info_Check();
		LevelUP_TalentPoint_Check();
		
		LevelUP_Reset_Button();

	}
}

void CUIGroup_PlayerLevelUP::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		if (m_pGameInstance->isKeyEnter(DIK_ESCAPE) || m_bEscapeCheck)
		{
			m_bEscapeCheck = true;
			m_bResetOn = true;

		} 
	}
}

HRESULT CUIGroup_PlayerLevelUP::Render()
{
	/*if (m_bRenderOpen)
	{
		vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{
			m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		}
	}*/
	return S_OK;
}

void CUIGroup_PlayerLevelUP::Setting_Default_Status_Value()
{

	if (0 == m_pGameInstance->Get_Player_State_SaveData().iLevel)
	{
		// ����
		m_iCurrentLevel = { 1 };
		m_iNextLevel = { 1 };


		// ĳ���� �ɷ�ġ ����
		m_iCurrentPower = { 1 }; // �� ��ġ
		m_iNextPower = { 1 }; // �� ��ġ

		m_iCurrentVitality = { 1 }; // Ȱ�� ��ġ
		m_iNextVitality = { 1 }; // Ȱ�� ��ġ

		m_iCurrentPlague = { 1 }; // ���� ��ġ
		m_iNextPlague = { 1 }; // ���� ��ġ

		// ĳ���� ���� ����
		m_fCurrentBonus_SkillDamage = *dynamic_cast<CPlayer*>(m_pPlayer)->Get_Bonus_SkillDamagePtr();
		m_fNextBonus_SkillDamage = m_fCurrentBonus_SkillDamage;


		m_iCurrentAttackPower = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower(); // ���� ����� 
		m_iNextAttackPower = m_iCurrentAttackPower; // ���� �����

		m_iCurrentAmountOfWounds = { 75 }; // ��ó ��
		m_iNextAmountOfWounds = { 75 }; // ��ó ��

		m_iCurrentClawAttackPower = dynamic_cast<CPlayer*>(m_pPlayer)->Get_ClawAttacPower(); // ��������
		m_iNextClawAttackPower = m_iCurrentClawAttackPower; // ��������

		m_iCurrentFullHp = dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp(); // ü��
		m_iNextFullHp = m_iCurrentFullHp; // ü��

		m_iCurrentFullMp = dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullMp();; // ������
		m_iNextFullMp = m_iCurrentFullMp; // ������


		m_iCurrentTalentPoint = { 0 };
		m_iNextTalentPoint = { 0 };

		m_iCurrentUnspent = { 0 };
		m_iNextUnspent = { 0 };

		m_iMemoryCurrentCount = dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment();
		m_iMemoryNextCount = m_iMemoryCurrentCount; // ������ ��ŭ ����

	}
	else
	{
		CUI_Manager::PLAYER_SAVE_STATE GetData = m_pGameInstance->Get_Player_State_SaveData();

		// ����
		m_iCurrentLevel = GetData.iLevel;
		m_iNextLevel = GetData.iLevel;


		// ĳ���� �ɷ�ġ ����
		m_iCurrentPower = GetData.iPower; // �� ��ġ
		m_iNextPower = GetData.iPower; // �� ��ġ

		m_iCurrentVitality = GetData.iVitality; // Ȱ�� ��ġ
		m_iNextVitality = GetData.iVitality; // Ȱ�� ��ġ

		m_iCurrentPlague = GetData.iPlague; // ���� ��ġ
		m_iNextPlague = GetData.iPlague; // ���� ��ġ

		// ĳ���� ���� ����

		m_fCurrentBonus_SkillDamage = GetData.fBonus_SkillDamage; // �߰� ��ų �����
		m_fNextBonus_SkillDamage = GetData.fBonus_SkillDamage;

		m_iCurrentAttackPower = GetData.iAttackPower; // ���� ����� 
		m_iNextAttackPower = GetData.iAttackPower; // ���� �����

		m_iCurrentAmountOfWounds = GetData.iAmountOfWounds; // ��ó ��
		m_iNextAmountOfWounds = GetData.iAmountOfWounds; // ��ó ��

		m_iCurrentClawAttackPower = GetData.iClawAttackPower; // ��������
		m_iNextClawAttackPower = GetData.iClawAttackPower; // ��������

		m_iCurrentFullHp = GetData.iFullHP; // ü��
		m_iNextFullHp = GetData.iFullHP; // ü��

		m_iCurrentFullMp = GetData.iFullMP; // ������
		m_iNextFullMp = GetData.iFullMP; // ������


		m_iCurrentTalentPoint = GetData.iTalentPoint;
		m_iNextTalentPoint = GetData.iTalentPoint;

		m_iCurrentUnspent = GetData.iUnspent;
		m_iNextUnspent = GetData.iUnspent;

		m_iMemoryCurrentCount = GetData.iMemoryFragment;
		m_iMemoryNextCount = m_iMemoryCurrentCount; // ������ ��ŭ ����
	}

	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (210 <= Image->Get_UI_GroupID() &&
			230 >= Image->Get_UI_GroupID())
			Image->Set_OnOff(false); // �⺻������ ��ư ���� ���� �׸���� ���ش�

		if (100 == Image->Get_UI_GroupID()) // Ư�� ��ü �̹���
			Image->Set_OnOff(false); // �⺻������ ��ư ���� ���� �׸���� ���ش�
		if (110 == Image->Get_UI_GroupID()) // Ư�� ��ü �̹���
			Image->Set_OnOff(false); // �⺻������ ��ư ���� ���� �׸���� ���ش�

	}


}

HRESULT CUIGroup_PlayerLevelUP::Button_Input_Check()
{
	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (200 == Button->Get_UI_GroupID() || 310 == Button->Get_UI_GroupID()) // ���� ��ư 
			{
				if (m_iCurrentPower != m_iNextPower ||
					m_iCurrentVitality != m_iNextVitality ||
					m_iCurrentPlague != m_iNextPlague)
				{
					m_pGameInstance->Set_All_UIObject_Condition_Open(m_pApplyPopUp, true);
					m_bApplyOn = true;
				}
				
			}
			if (300 == Button->Get_UI_GroupID()) // �ʱ�ȭ ��ư 
			{
				if (m_iCurrentPower != m_iNextPower ||
					m_iCurrentVitality != m_iNextVitality ||
					m_iCurrentPlague != m_iNextPlague)
				{
					m_pGameInstance->Set_All_UIObject_Condition_Open(m_pResetPopUp, true);
					m_bResetOn = true;

				}
			}

			if (211 == Button->Get_UI_GroupID()) // �� ���� 
			{
				m_iNextLevel -= 1; // �� ����
				m_iNextPower -= 1;
				m_iNextAttackPower -= 10;
				m_iNextAmountOfWounds -= 20;
				m_iNextTalentPoint -= 1;
				m_iNextUnspent -= 1;
				m_iMemoryNextCount += m_iCurrentMemoryNeed;
			}
			if (212 == Button->Get_UI_GroupID())
			{
				m_iNextLevel += 1; // �� ����
				m_iNextPower += 1;
				m_iNextAttackPower += 10;
				m_iNextAmountOfWounds += 20;
				m_iNextTalentPoint += 1;
				m_iNextUnspent += 1;
				m_iMemoryNextCount -= m_iNextMemoryNeed;
			}

			if (221 == Button->Get_UI_GroupID()) // Ȱ�� ���� 
			{
				m_iNextLevel -= 1; // Ȱ�� ����
				m_iNextVitality -= 1;
				m_iNextFullHp -= 140;
				m_iNextTalentPoint -= 1;
				m_iNextUnspent -= 1;
				m_iMemoryNextCount += m_iCurrentMemoryNeed;

			}
			if (222 == Button->Get_UI_GroupID())
			{
				m_iNextLevel += 1; // Ȱ�� ����
				m_iNextVitality += 1;
				m_iNextFullHp += 140;
				m_iNextTalentPoint += 1;
				m_iNextUnspent += 1;
				m_iMemoryNextCount -= m_iNextMemoryNeed;
			}
			if (231 == Button->Get_UI_GroupID()) // ���� ���� 
			{
				m_iNextLevel -= 1; // ���� ����
				m_iNextPlague -= 1;
				m_iNextClawAttackPower -= 100;
				m_iNextFullMp -= 30;
				m_fNextBonus_SkillDamage -= 5.f;
				m_iNextTalentPoint -= 1;
				m_iNextUnspent -= 1;
				m_iMemoryNextCount += m_iCurrentMemoryNeed;
			}
			if (232 == Button->Get_UI_GroupID())
			{
				m_iNextLevel += 1; // ���� ����
				m_iNextPlague += 1;
				m_iNextClawAttackPower += 100;
				m_iNextFullMp += 30;
				m_fNextBonus_SkillDamage += 5.f;
				m_iNextTalentPoint += 1;
				m_iNextUnspent += 1;
				m_iMemoryNextCount -= m_iNextMemoryNeed;
			}
			if (320 == Button->Get_UI_GroupID())
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_bEscapeCheck = true;
				//m_bResetOn = true;
				
			}
		}
	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Button_Render_Check()
{
	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (211 == Button->Get_UI_GroupID()) // �� ���� 
		{
			if (m_iNextPower > m_iCurrentPower)
				Button->Set_OnOff(true); // ���纸�� ������ ū ��� ( ���� ��ư�� ���� ����
			else
				Button->Set_OnOff(false); // ���� ��ư�� ������ �ʾ��� �� ���� ����
		}
		if (221 == Button->Get_UI_GroupID()) // Ȱ�� ����
		{
			if (m_iNextVitality > m_iCurrentVitality)
				Button->Set_OnOff(true); // ���纸�� ������ ū ��� ( ���� ��ư�� ���� ����
			else
				Button->Set_OnOff(false); // ���� ��ư�� ������ �ʾ��� �� ���� ����
		}
		if (231 == Button->Get_UI_GroupID()) // ���� ����
		{
			if (m_iNextPlague > m_iCurrentPlague)
				Button->Set_OnOff(true); // ���纸�� ������ ū ��� ( ���� ��ư�� ���� ����
			else
				Button->Set_OnOff(false); // ���� ��ư�� ������ �ʾ��� �� ���� ����
		}
		if (212 == Button->Get_UI_GroupID() ||
			222 == Button->Get_UI_GroupID() ||
			232 == Button->Get_UI_GroupID()) // ��, Ȱ��, ���� ���� ��ư ���� ����
		{
			if (m_iMemoryNextCount - m_iNextMemoryNeed > 0) //
				Button->Set_OnOff(true); // ���� ���� ������ ��쿡�� ���� ��ư ����
			else
				Button->Set_OnOff(false); // ���� ���� �Ұ����� ��� ���� ��ư ����
		}


	}
	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Button_Image_On_Check()
{
	_bool bPower = false;
	_bool bVitality = false;
	_bool bPlague = false;
	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (210 == Image->Get_UI_GroupID()) // �� Ȱ��ȭ �̹���                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
		{
			if (Image->Get_Open_Image()) // ���� �ִٸ� = > ���콺 on�� ����     
			{
				bPower = true;
			}

		}

		if (220 == Image->Get_UI_GroupID()) // Ȱ�� Ȱ��ȭ �̹���
		{
			if (Image->Get_Open_Image()) // ���� �ִٸ� = > ���콺 on�� ����     
			{
				bVitality = true;
			}

		}

		if (230 == Image->Get_UI_GroupID()) // ���� Ȱ��ȭ �̹���
		{
			if (Image->Get_Open_Image()) // ���� �ִٸ�
			{
				bPlague = true;

			}

		}
	}
	Button_Image_OnOff_Check(bPower, bVitality, bPlague);

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Button_Image_OnOff_Check(_bool bPower, _bool bVitality, _bool bPlaque)
{

	_bool bTalentOn = { false };

	if (true == bPower || true == bVitality || true == bPlaque)
		bTalentOn = true;

	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (210 == Image->Get_UI_GroupID()) // �� �̹���
		{
			Image->Set_OnOff(bPower); // ���� ������, ���� �����, ��ó ���� �̹����� on
		}
		if (220 == Image->Get_UI_GroupID()) // Ȱ�� �̹���
		{
			Image->Set_OnOff(bVitality); // Ȱ���� ������ ü�� �̹��� on
		}
		if (230 == Image->Get_UI_GroupID()) // ���� �̹���
		{
			Image->Set_OnOff(bPlaque); // ������ ������, ���� �����, ������ �̹����� on
		}
		if (100 == Image->Get_UI_GroupID()) // Ư�� ��ü �̹���
		{
			Image->Set_OnOff(bTalentOn);
		}
		if (110 == Image->Get_UI_GroupID()) // Ư�� �̻�� �̹���
		{
			Image->Set_OnOff(bTalentOn);
		}

	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Memory_Check()
{

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (10 == TextBox->Get_UI_GroupID()) // ���� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentLevel);
			TextBox->Set_Content(ChangeText);
		}
		if (11 == TextBox->Get_UI_GroupID()) // ���� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iNextLevel);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentLevel != m_iNextLevel)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);

		}
		if (12 == TextBox->Get_UI_GroupID()) // ������ �����ϰ� �ִ� ����� ���� �� 
		{
			wsprintf(ChangeText, CountText, m_iMemoryCurrentCount);
			TextBox->Set_Content(ChangeText);

		}
		if (13 == TextBox->Get_UI_GroupID()) //  ���� ���� ��� ���� - ���� ���� �Ҹ�Ǵ� ��� ���� = ��� �� ���
		{
			wsprintf(ChangeText, CountText, m_iMemoryNextCount);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentLevel != m_iNextLevel)
				TextBox->Set_Change_TextColor(FONT_RED);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);

		}
		if (14 == TextBox->Get_UI_GroupID()) //  ���� ������ ��� ��Ű�µ� �ʿ��� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iNextMemoryNeed);
			TextBox->Set_Content(ChangeText);
		}


	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Status_Check()
{

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (20 == TextBox->Get_UI_GroupID()) // ���� �Ŀ� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentPower);
			TextBox->Set_Content(ChangeText);

		}
		if (21 == TextBox->Get_UI_GroupID()) // ���� �Ŀ� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iNextPower);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentPower != m_iNextPower)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);

		}

		if (30 == TextBox->Get_UI_GroupID()) // ���� Ȱ�� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentVitality);
			TextBox->Set_Content(ChangeText);

		}
		if (31 == TextBox->Get_UI_GroupID()) // ���� Ȱ�� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iNextVitality);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentVitality != m_iNextVitality)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);

		}

		if (40 == TextBox->Get_UI_GroupID()) // ���� �Ŀ� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentPlague);
			TextBox->Set_Content(ChangeText);

		}
		if (41 == TextBox->Get_UI_GroupID()) // ���� �Ŀ� ���� ǥ��
		{
			wsprintf(ChangeText, CountText, m_iNextPlague);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentPlague != m_iNextPlague)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);

		}
	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Status_Info_Check()
{
	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (50 == TextBox->Get_UI_GroupID()) // ���� ����� ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentAttackPower);
			TextBox->Set_Content(ChangeText);

		}
		if (51 == TextBox->Get_UI_GroupID()) // ���� ����� ��
		{
			wsprintf(ChangeText, CountText, m_iNextAttackPower);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentAttackPower != m_iNextAttackPower)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}

		if (60 == TextBox->Get_UI_GroupID()) // ��ó �� ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentAmountOfWounds);
			TextBox->Set_Content(ChangeText);

		}
		if (61 == TextBox->Get_UI_GroupID()) // ��ó �� ��
		{
			wsprintf(ChangeText, CountText, m_iNextAmountOfWounds);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentAmountOfWounds != m_iNextAmountOfWounds)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}

		if (70 == TextBox->Get_UI_GroupID())  // ���� ����� ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentClawAttackPower);
			TextBox->Set_Content(ChangeText);

		}
		if (71 == TextBox->Get_UI_GroupID()) // ���� ����� ��
		{
			wsprintf(ChangeText, CountText, m_iNextClawAttackPower);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentClawAttackPower != m_iNextClawAttackPower)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}

		if (80 == TextBox->Get_UI_GroupID()) // ü�� ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentFullHp);
			TextBox->Set_Content(ChangeText);

		}
		if (81 == TextBox->Get_UI_GroupID()) // ü�� ��
		{
			wsprintf(ChangeText, CountText, m_iNextFullHp);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentFullHp != m_iNextFullHp)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}

		if (90 == TextBox->Get_UI_GroupID()) // ������ ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentFullMp);
			TextBox->Set_Content(ChangeText);

		}
		if (91 == TextBox->Get_UI_GroupID()) // ������ ��
		{
			wsprintf(ChangeText, CountText, m_iNextFullMp);
			TextBox->Set_Content(ChangeText);

			if (m_iCurrentFullMp != m_iNextFullMp)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}

	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_TalentPoint_Check()
{
	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (100 == TextBox->Get_UI_GroupID()) // Ư�� ��ü ����Ʈ ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentTalentPoint);
			TextBox->Set_Content(ChangeText);
			      
		}
		if (101 == TextBox->Get_UI_GroupID()) // Ư�� ��ü ����Ʈ ��
		{
			wsprintf(ChangeText, CountText, m_iNextTalentPoint);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentTalentPoint != m_iNextTalentPoint)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}
		if (110 == TextBox->Get_UI_GroupID()) // �̻�� Ư�� ����Ʈ ��
		{
			wsprintf(ChangeText, CountText, m_iCurrentUnspent);
			TextBox->Set_Content(ChangeText);

		}
		if (111 == TextBox->Get_UI_GroupID()) // �̻�� Ư�� ����Ʈ ��
		{
			wsprintf(ChangeText, CountText, m_iNextUnspent);
			TextBox->Set_Content(ChangeText);
			if (m_iCurrentUnspent != m_iNextUnspent)
				TextBox->Set_Change_TextColor(FONT_GREEN);
			else
				TextBox->Set_Change_TextColor(FONT_WHITE);
		}
	}

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Apply_Button()
{
	if (m_bApplyOn)
	{
		for (auto& Button : m_pApplyPopUp->Find_UI_Button())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (1 == Button->Get_UI_GroupID()) // ��
				{
					LevelUP_Apply();
					m_bApplyOn = false;
					m_pGameInstance->Set_All_UIObject_Condition_Open(m_pApplyPopUp, false);
				}
				if (2 == Button->Get_UI_GroupID()) // �ƴϿ�
				{
					m_bApplyOn = false;
					m_pGameInstance->Set_All_UIObject_Condition_Open(m_pApplyPopUp, false);
				}
			}
		}
	}
	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Reset_Button()
{
	if (m_bResetOn)
	{
		if (m_bEscapeCheck)
		{
			LevelUP_Reset();
			m_bResetOn = false;
			m_bEscapeCheck = false;
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerLevelUP"), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
			m_pGameInstance->UIGroup_Render_OnOff(m_eMyLevelID, TEXT("Layer_PlayerMenu"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
		}
		else
		{
			for (auto& Button : m_pResetPopUp->Find_UI_Button())
			{
				if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
				{
					if (1 == Button->Get_UI_GroupID()) // ��
					{
						LevelUP_Reset();
						m_bResetOn = false;
						m_pGameInstance->Set_All_UIObject_Condition_Open(m_pResetPopUp, false);
					}
					if (2 == Button->Get_UI_GroupID()) // �ƴϿ�
					{

						m_bResetOn = false;
						m_pGameInstance->Set_All_UIObject_Condition_Open(m_pResetPopUp, false);
					}

				}
			}
		}

	}
	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Apply()
{
	// ����
	m_iCurrentLevel = m_iNextLevel;

	int iMomoryCount = m_iMemoryCurrentCount - m_iMemoryNextCount;
	// ��� ����
	dynamic_cast<CPlayer*>(m_pPlayer)->Increase_MemoryFragment(-iMomoryCount);
	m_iMemoryTotalUse += m_iMemoryCurrentCount - m_iMemoryNextCount; //���ݱ��� �÷��̾ �������� ����� ��� ���� ������ ������


	m_iMemoryCurrentCount = dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment();
	m_iMemoryNextCount = m_iMemoryCurrentCount;

	//m_iMemoryNeed = { 529 }; // ���� ���� ���� �ʿ��� ����� ���� ���� �޾� �� ������ �׳� ����� => �ǽð� ������� ����


	// ĳ���� �ɷ�ġ ����
	m_iCurrentPower = m_iNextPower; // �� ��ġ

	m_iCurrentVitality = m_iNextVitality; // Ȱ�� ��ġ

	m_iCurrentPlague = m_iNextPlague; // ���� ��ġ

	// ĳ���� ���� ����
	dynamic_cast<CPlayer*>(m_pPlayer)->Set_Bonus_SkillDamage(m_fCurrentBonus_SkillDamage);
	m_fCurrentBonus_SkillDamage = m_fNextBonus_SkillDamage;

	dynamic_cast<CPlayer*>(m_pPlayer)->Increase_AttackPower(m_iNextAttackPower);
	m_iCurrentAttackPower = m_iNextAttackPower; // ���� ����� 

	m_iCurrentAmountOfWounds = m_iNextAmountOfWounds; // ��ó ��

	dynamic_cast<CPlayer*>(m_pPlayer)->Increase_ClawAttackPower(m_iNextClawAttackPower);
	m_iCurrentClawAttackPower = m_iNextClawAttackPower; // ��������

	dynamic_cast<CPlayer*>(m_pPlayer)->Set_PlayerFullHp(m_iNextFullHp);
	dynamic_cast<CPlayer*>(m_pPlayer)->Increase_PlayerHp(m_iNextFullHp - m_iCurrentFullHp);
	m_iCurrentFullHp = m_iNextFullHp; // ü��

	dynamic_cast<CPlayer*>(m_pPlayer)->Set_PlayerFullMp(m_iNextFullMp);
	dynamic_cast<CPlayer*>(m_pPlayer)->Increase_PlayerMp(m_iNextFullMp - m_iCurrentFullMp);
	m_iCurrentFullMp = m_iNextFullMp; // ������

	// Ư�� ����
	m_iCurrentTalentPoint = m_iNextTalentPoint;

	m_iCurrentUnspent = m_iNextUnspent;



	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LevelUP_Reset()
{
	// �÷ȴ� ���� ��� �ʱ�ȭ
	// ����
	m_iNextLevel = m_iCurrentLevel;
	m_iMemoryNextCount = m_iMemoryCurrentCount;

	//m_iMemoryNeed = { 529 }; // ���߿� ������ ��� �� �޸� ���� �ؾ� ��

	m_iNextPower = m_iCurrentPower; // �� ��ġ
	m_iNextVitality = m_iCurrentVitality; // Ȱ�� ��ġ

	m_iNextPlague = m_iCurrentPlague; // ���� ��ġ

	// ĳ���� ���� ����
	m_fNextBonus_SkillDamage = m_fCurrentBonus_SkillDamage;
	
	m_iNextAttackPower = m_iCurrentAttackPower; // ���� �����

	m_iNextAmountOfWounds = m_iCurrentAmountOfWounds; // ��ó ��

	m_iNextClawAttackPower = m_iCurrentClawAttackPower; // ��������

	m_iNextFullHp = m_iCurrentFullHp; // ü��

	m_iNextFullMp = m_iCurrentFullMp; // ������

	// Ư�� ����
	m_iNextTalentPoint = m_iCurrentTalentPoint;
	m_iNextUnspent = m_iCurrentUnspent;

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Ready_UIObject()
{
	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_2");
	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_3");

	LoadData_UIObject(LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_2");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP_3");



	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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
			if (iUIType == UI_TEXT)
		{
			Desc.eTextSort = static_cast<CUIObject::TEXTSORT>(iTextSort);
		}

		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(iLevelIndex, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);


	//MessageBox(g_hWnd, L"Load �Ϸ�", _T("����"), MB_OK);
	return S_OK;
}

CUIGroup_PlayerLevelUP* CUIGroup_PlayerLevelUP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerLevelUP* pInstance = new CUIGroup_PlayerLevelUP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerLevelUP::Clone(void* pArg)
{
	CUIGroup_PlayerLevelUP* pInstance = new CUIGroup_PlayerLevelUP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerLevelUP::Free()
{
	CUI_Manager::PLAYER_SAVE_STATE SetData = {};

	// ����
	SetData.iLevel = m_iCurrentLevel;
	SetData.iLevel = m_iNextLevel;
	// ĳ���� �ɷ�ġ ����
	SetData.iPower = m_iCurrentPower; // �� ��ġ

	SetData.iVitality = m_iCurrentVitality; // Ȱ�� ��ġ

	SetData.iPlague = m_iCurrentPlague; // ���� ��ġ

	// ĳ���� ���� ����

	SetData.fBonus_SkillDamage = m_fCurrentBonus_SkillDamage; // �߰� ��ų �����


	SetData.iAttackPower = m_iCurrentAttackPower; // ���� ����� 

	SetData.iAmountOfWounds = m_iCurrentAmountOfWounds; // ��ó ��

	SetData.iClawAttackPower = m_iCurrentClawAttackPower; // ��������

	SetData.iFullHP = m_iCurrentFullHp; // ü��

	SetData.iFullMP = m_iCurrentFullMp; // ������


	SetData.iTalentPoint = m_iCurrentTalentPoint;

	SetData.iUnspent = m_iCurrentUnspent;

	SetData.iMemoryFragment = m_iMemoryCurrentCount;

	m_pGameInstance->Set_Player_State_SaveData(SetData);
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_LEVELUP);
}
