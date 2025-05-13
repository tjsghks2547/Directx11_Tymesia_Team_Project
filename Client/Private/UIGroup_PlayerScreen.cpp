#include "pch.h"
#include "UIGroup_PlayerScreen.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_Image.h"
#include "Player.h"
#include "UI_ItemBackground.h"
#include "UIGroup_Inventory.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_FixSlotFrame.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_MPBar3_MainBar.h"
#include "UI_HPBar5_Track.h"
#include "UIGroup_Skill.h"

// 아이템
#include "UI_LootNotifyBackground.h"
#include "UI_Item_Icon.h"

#include "UI_QuestBackground.h"

CUIGroup_PlayerScreen::CUIGroup_PlayerScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerScreen::CUIGroup_PlayerScreen(const CUIGroup_PlayerScreen& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerScreen::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerScreen::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;


	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eMyLevelID = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	
	m_pItmeScreen = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItmeScreen, false);


	m_pPoisonScreen = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_Quest_Poison");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pPoisonScreen, false);

	for (auto& Image : m_pPoisonScreen->Find_UI_Image())
	{
		if (3 == Image->Get_UI_GroupID())
		{
			m_pPoisonQuestDesc = Image;
		}
	}

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_Player"), "PLAYER");
	m_pPlayerSkillMgr = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PlayerSkillMgr();
	m_pPotion_Count = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Potion_Count();
	m_pPlayerSkillCoolTime = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Skill_CoolTime_Ptr();
	m_pGroupInven = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_PlayerInventory"), "Inventory");	
	m_pGroupSkill = m_pGameInstance->Get_GameObject_To_Layer(m_eMyLevelID, TEXT("Layer_PlayerSkill"), "UI_Skill");
	
	for (auto& Textbox : m_pMyScene->Find_UI_TextBox())
	{
		if (12 == Textbox->Get_UI_GroupID()) // PlayerHP
		{
			Textbox->Set_OnOff(false);
			m_pPlayerHPText = Textbox;
		}
		if (22 == Textbox->Get_UI_GroupID()) // PlayerMP
		{
			Textbox->Set_OnOff(false);
			m_pPlayerMPText = Textbox;
		}
		if (101 == Textbox->Get_UI_GroupID())
		{
			Textbox->Set_OnOff(false);
			m_pMonsterText = Textbox;
		}
	}

	Ready_Skill_Slot();
	Ready_Player_GageBar();


	return S_OK;
}

void CUIGroup_PlayerScreen::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	// 그룹에서 가장 메인이 되는 씬이 켜지는 경우 다른 것들도 켜지도록 => 해당 씬들의 업데이트를 켠다는 것
	if (m_pMyScene->Get_Scene_Render_State())
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItmeScreen, true);
	}
	else
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItmeScreen, false);
	}
	
}

void CUIGroup_PlayerScreen::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_0))
	{
		m_pGameInstance->Set_All_UIObject_Condition_Open(m_pPoisonScreen, true);
	}

	if (m_pGameInstance->Get_Scene_Render_State(m_pPoisonScreen))
	{
		// 카운트 증가되는거 ui에서 실시간으로 확인
		dynamic_cast<CUI_QuestBackground*>(m_pPoisonQuestDesc)->Set_MonsterCount(m_iQuestMonsterDeadCount);
		if (4 == m_iQuestMonsterDeadCount)
		{
			CGameObject::GAMEOBJECT_DESC pDesc = {};
			pDesc.iCurLevel = m_eMyLevelID;
			pDesc.fPosition = { -42.87f,100.59f,-100.27f,1.0f };
			m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2"), CATEGORY_BOSS, &pDesc);

			m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pPoisonScreen, false);
			m_pGameInstance->Set_All_UIObject_Condition_Open(m_pPoisonScreen, false);
		}
	}
	else
	{
		m_iQuestMonsterDeadCount = 0;
	}

	//if (FAILED(m_pGameInstance->Add_Monster(m_eMyLevelID, TEXT("Prototype_GameObject_Boss_Magician2"), CATEGORY_BOSS, &pDesc)))
	UI_Player_Skill_ImageSet();

	Button_Skill(); // 현재 임의로 키다운 시 스킬 실행
	Player_Info_GageBar();// 플레이어 정보를 출력하기에 상시 업데이트

	UI_Direction_HPBar(); // bar 연출
	UI_Direction_MPBar();

	if (m_pMonsterText->Get_OnOff()) //몬스터 처치 시 획득하는 기억 파편량 표시 유지 시간
	{
		m_fMonsterTextOnTime += fTimeDelta;
		if (m_fMonsterTextOnTime > 3.f)
		{
			m_pMonsterText->Set_OnOff(false);
			m_fMonsterTextOnTime = 0.0f;
		}
	}
	if (m_pPlayerHPText->Get_OnOff()) //몬스터 처치 시 획득하는 기억 파편량 표시 유지 시간
	{
		m_fHPTextOnTime += fTimeDelta;
		if (m_fHPTextOnTime > 3.f)
		{
			m_pPlayerHPText->Set_OnOff(false);
			m_fHPTextOnTime = 0.0f;
		}
	}
	if (m_pPlayerMPText->Get_OnOff()) //몬스터 처치 시 획득하는 기억 파편량 표시 유지 시간
	{
		m_fMPTextOnTime += fTimeDelta;
		if (m_fMPTextOnTime > 3.f)
		{
			m_pPlayerMPText->Set_OnOff(false);
			m_fMPTextOnTime = 0.0f;
		}
	}

	Item_Nudge_Check(fTimeDelta); // 획득하거나 버린 아이템이 있는지 체크하는 함수



}

void CUIGroup_PlayerScreen::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

HRESULT CUIGroup_PlayerScreen::Render()
{
	if (m_bRenderOpen)
	{
	}
	return S_OK;
}

void CUIGroup_PlayerScreen::Player_Info_GageBar()
{
	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";
	const _tchar* CountTextDouble = L"%d / %d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (10 == TextBox->Get_UI_GroupID()) // 현재 HP
		{
			wsprintf(ChangeText, CountTextDouble, dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentHp(), dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp());
			TextBox->Set_Content(ChangeText);
		}
		if (20 == TextBox->Get_UI_GroupID()) // 현재 MP
		{
			wsprintf(ChangeText, CountTextDouble, dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentMp(), dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullMp());
			TextBox->Set_Content(ChangeText);
		}
		//if (12 == TextBox->Get_UI_GroupID()) // 물약 드링킹 시 플러스되는 수치
		//{
		//	TextBox->Set_OnOff(false);
		//}
		//if (30 == TextBox->Get_UI_GroupID()) // 현재 물약 개수
		//{
		//	wsprintf(ChangeText, CountTextDouble, 3, 3);
		//	TextBox->Set_Content(ChangeText);
		//}
		//if (22 == TextBox->Get_UI_GroupID()) // mp 플러스 되는 수치 인 것 같음
		//{
		//	TextBox->Set_OnOff(false);
		//}
		//if (40 == TextBox->Get_UI_GroupID()) // 현재 깃털 개수
		//{
		//	wsprintf(ChangeText, CountTextDouble, 3, 3);
		//	TextBox->Set_Content(ChangeText);
		//}
		//if (100 == TextBox->Get_UI_GroupID()) // 현재 기억의 파편 개수
		//{
		//	wsprintf(ChangeText, CountText, dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment());
		//	TextBox->Set_Content(ChangeText);
		//}

	}

	for (auto& Image : m_pMyScene->Find_UI_Image())
	{
		if (1 == Image->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountText, dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment());
			Image->Set_Content(ChangeText);

		}
		if (20 == Image->Get_UI_GroupID()) // 물약 현재 개수 / 최대 개수
		{
			wsprintf(ChangeText, CountTextDouble, *m_pPotion_Count, 3); // 플레이어 멤버 변수로 만들어달라고 하자
			Image->Set_Content(ChangeText);
		}
		if (21 == Image->Get_UI_GroupID()) // 깃털 현재 개수 / 최대 개수
		{
			wsprintf(ChangeText, CountTextDouble, 3, 3); // 깃털은 몰?루
			Image->Set_Content(ChangeText);
		}
	}

}

void CUIGroup_PlayerScreen::UI_Direction_HPBar()
{
	_int iFullHP = dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp();
	_int iCurrentHP = dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentHp();
	_int iHpBarTransform = static_cast<int>(dynamic_cast<CTransform*>(dynamic_cast<CUI_HPBar3_MainBar*>(m_pHPGageBar)->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION).x);
	_int iHpBarSizeX = static_cast<int>(dynamic_cast<CTransform*>(dynamic_cast<CUI_HPBar3_MainBar*>(m_pHPGageBar)->Find_Component(TEXT("Com_Transform")))->Compute_Scaled().x);


	dynamic_cast<CUI_HPBar3_MainBar*>(m_pHPGageBar)->Set_PlayerHP_Info((float)iFullHP, (float)iCurrentHP);

	if (iCurrentHP >= iFullHP || iCurrentHP <= 0 )
	{
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_Open_Image(false);
	}
	else
	{
		// HPBar 길이 258
		// CurrentHP / FullHP => 나온 비율 ex. 1 /10 => Track의 X 좌표에서 258 * 1/10 = 25.8 이만큼을 뺀다 
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_Open_Image(true);

		_int iHPTrackX = iHpBarTransform + 129;
		_int fHPBarLastPointX = static_cast<int>((float)iHpBarSizeX * (1.0f -(((float)iCurrentHP / (float)iFullHP))));
				
		iHPTrackX -= fHPBarLastPointX;
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_ChangeX(iHPTrackX);
	}


}

void CUIGroup_PlayerScreen::UI_Direction_MPBar()
{
	_int iFullMP = dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullMp();
	_int iCurrentMP = dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentMp();

	dynamic_cast<CUI_MPBar3_MainBar*>(m_pMPGageBar)->Set_PlayerMp_Info ((float)iFullMP, (float)iCurrentMP);

}

void CUIGroup_PlayerScreen::UI_Player_Skill_ImageSet()
{

	switch (dynamic_cast<CPlayer*>(m_pPlayer)->Get_Player_Take_Away_Skill())
	{
	case PLAYER_SKILL_AXE:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(1); // 도끼
		break;
	case PLAYER_SKILL_HALBERD:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(11); // 핼버드
		break;
	case PLAYER_SKILL_SCYTHE:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(17); // 낫
		break;
	case PLAYER_SKILL_CANESWORD:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(14); // 케인
		break;
	case PLAYER_SKILL_GREADSWORD:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(10); // 대검
		break;
	case PLAYER_SKILL_JAVELINSWORD:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(20); // 투창검
		break;
	case PLAYER_SKILL_START:
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_TexIcon(0); // 스킬 없음
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Take_Away_Skill(PLAYER_SKILL_START);
		break;


	}
	CUIGroup_Skill* pSkill = dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill);
	if (!pSkill->Get_PlayerSkill_List().empty())
	{
		switch (dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().at(0))
		{
		case PLAYER_SKILL_AXE:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(1); // 도끼
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_AXE);
			break;
		case PLAYER_SKILL_HALBERD:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(11); // 핼버드
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_HALBERD);
			break;
		case PLAYER_SKILL_SCYTHE:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(17); // 낫
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_SCYTHE);
			break;
		case PLAYER_SKILL_CANESWORD:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(14); // 케인
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_CANESWORD);
			break;
		case PLAYER_SKILL_GREADSWORD:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(10); // 대검
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_GREADSWORD);
			break;
		case PLAYER_SKILL_JAVELINSWORD:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(20); // 투창검
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Skill_1st(PLAYER_SKILL_JAVELINSWORD);
			break;
		default:
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_TexIcon(0); // 스킬 없음
			break;
		}
		if (2 <= pSkill->Get_PlayerSkill_List().size())
		{
			switch (dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().at(1))
			{
			case PLAYER_SKILL_AXE:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(1); // 도끼
				break;
			case PLAYER_SKILL_HALBERD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(11); // 핼버드
				break;
			case PLAYER_SKILL_SCYTHE:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(17); // 낫
				break;
			case PLAYER_SKILL_CANESWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(14); // 케인
				break;
			case PLAYER_SKILL_GREADSWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(10); // 대검
				break;
			case PLAYER_SKILL_JAVELINSWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(20); // 투창검
				break;
			default:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_TexIcon(0); // 스킬 없음
				break;
			}
		}
		if (3 <= pSkill->Get_PlayerSkill_List().size())
		{
			switch (dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().at(2))
			{
			case PLAYER_SKILL_AXE:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(1); // 도끼
				break;
			case PLAYER_SKILL_HALBERD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(11); // 핼버드
				break;
			case PLAYER_SKILL_SCYTHE:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(17); // 낫
				break;
			case PLAYER_SKILL_CANESWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(14); // 케인
				break;
			case PLAYER_SKILL_GREADSWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(10); // 대검
				break;
			case PLAYER_SKILL_JAVELINSWORD:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(20); // 투창검
				break;
			default:
				dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_TexIcon(0); // 스킬 없음
				break;
			}
		}
	}
}

void CUIGroup_PlayerScreen::Item_Nudge_Check(_float fTimeDelta)
{
	if (0 != m_pGameInstance->Get_Item_Drop_Info().size()) // 버리는 아이템이 있는경우
	{
		auto& listDropItmeInfo = m_pGameInstance->Get_Item_Drop_Info();

		auto it = listDropItmeInfo.begin();

		while (it != listDropItmeInfo.end())
		{
			if (!m_bNudgeUse[0]) // 1번 넛지 꺼져 있으면
			{
				Item_Nudge_Info(*it, ITEM_DROP, 1); // 추가해서 재생 
				it = listDropItmeInfo.erase(it);
				m_bNudgeUse[0] = true;
			}
			else if (!m_bNudgeUse[1])
			{
				Item_Nudge_Info(*it, ITEM_DROP, 2);
				it = listDropItmeInfo.erase(it);
				m_bNudgeUse[1] = true;
			}
			else if (!m_bNudgeUse[2])
			{
				Item_Nudge_Info(*it, ITEM_DROP, 3);
				it = listDropItmeInfo.erase(it);
				m_bNudgeUse[2] = true;
			}
			else if (!m_bNudgeUse[3])
			{
				Item_Nudge_Info(*it, ITEM_DROP, 4);
				it = listDropItmeInfo.erase(it);
				m_bNudgeUse[3] = true;
			}
			else
			{
				break;
			}
		}
		
	}
	if (0 != m_pGameInstance->Get_Item_Save_Info().size()) // 획득한 아이템이 있는 경우
	{

		auto& listSaveItmeInfo = m_pGameInstance->Get_Item_Save_Info();
		
		if (0 != listSaveItmeInfo.size())
		{
			auto it = listSaveItmeInfo.begin();

			while (it != listSaveItmeInfo.end())
			{
				if (!m_bNudgeUse[0]) // 1번 넛지 꺼져 있으면
				{
					Item_Nudge_Info(*it, ITEM_SAVE, 1); // 추가해서 재생 
					it = listSaveItmeInfo.erase(it);
					m_bNudgeUse[0] = true;
				}
				else if (!m_bNudgeUse[1])
				{
					Item_Nudge_Info(*it, ITEM_SAVE, 2);
					it = listSaveItmeInfo.erase(it);
					m_bNudgeUse[1] = true;
				}
				else if (!m_bNudgeUse[2])
				{
					Item_Nudge_Info(*it, ITEM_SAVE, 3);
					it = listSaveItmeInfo.erase(it);
					m_bNudgeUse[2] = true;
				}
				else if (!m_bNudgeUse[3])
				{
					Item_Nudge_Info(*it, ITEM_SAVE, 4);
					it = listSaveItmeInfo.erase(it);
					m_bNudgeUse[3] = true;
				}
				else
				{
					break;
				}
			}
		}
	} 
	_float2 fSetPos = {};
	for (_int i = 0; i < 4; i++) // n초 뒤에 알림 끄기
	{
		if (m_bNudgeUse[i])
		{
			m_fTimeCheck[i] += fTimeDelta;

			if (m_fTimeCheck[i] > 4) // 끄고
			{
				m_bNudgeUse[i] = false;
				m_fTimeCheck[i] = 0;

				for (auto& Image : m_pItmeScreen->Find_UI_Image())
				{
					if (i + 1 == Image->Get_UI_GroupID()) // 배경 이미지 설정
					{
						dynamic_cast<CUI_Image*>(Image)->Set_OnOff(false);
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_LootNotifyBackground*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos.y = ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y ;
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, { 1500.f,fSetPos.y });
					}
					if (i + 11 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
					{
						dynamic_cast<CUI_Image*>(Image)->Set_OnOff(false);
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_Item_Icon*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos.y = ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y;
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, { 1280.f,fSetPos.y });

					}
				}
				for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
				{
					if (i + 1 == TextBox->Get_UI_GroupID())
					{
						dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(false);
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos.y = BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y;
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, { 1320.f,fSetPos.y });
					}
					if (i + 11 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
					{
						dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(false);
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos.y = BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y;
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, { 1335.f,fSetPos.y });
					}
				}
			}
			else if (m_fTimeCheck[i] > 2) // 사라지고
			{
				for (auto& Image : m_pItmeScreen->Find_UI_Image())
				{
					if (i + 1 == Image->Get_UI_GroupID()) // 배경 이미지 설정
					{
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_LootNotifyBackground*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + m_fNudgeSpeed ,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
					if (i + 11 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
					{
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_Item_Icon*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + m_fNudgeSpeed ,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);

					}
				}
				for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
				{
					if (i + 1 == TextBox->Get_UI_GroupID())
					{
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + m_fNudgeSpeed ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
					if (i + 11 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
					{
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + m_fNudgeSpeed ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
				}
			}
			else // 생겨나고
			{
				for (auto& Image : m_pItmeScreen->Find_UI_Image())
				{
					if (i + 1 == Image->Get_UI_GroupID()) // 배경 이미지 설정
					{
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_LootNotifyBackground*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x - m_fNudgeSpeed ,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						if (1500.f > fSetPos.x)
							fSetPos.x = 1500.f;
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
					if (i + 11 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
					{
						CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_Item_Icon*>(Image)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x - m_fNudgeSpeed ,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						if (1280.f > fSetPos.x)
							fSetPos.x = 1280.f;
						ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);

					}
				}
				for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
				{
					if (i + 1 == TextBox->Get_UI_GroupID())
					{
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x - m_fNudgeSpeed ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						if (1320.f > fSetPos.x)
							fSetPos.x = 1320.f;
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
					if (i + 11 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
					{
						CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
						fSetPos = { BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x - m_fNudgeSpeed ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y };
						if (1335.f > fSetPos.x)
							fSetPos.x = 1335.f;
						BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, fSetPos);
					}
				}

			}

		}
	}

}

void CUIGroup_PlayerScreen::Item_Nudge_Info(ITEM_TYPE eItemType, ITEM_STATE eState, _int iNum)
{
	UI_Item MakeInfo = {};
	for (auto& ItemInfo : dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Vector_Itme_default_Info()) 
	{
		if (eItemType == ItemInfo.ItemType)
		{
			MakeInfo = ItemInfo;// 인벤 아이템 정보 컨테이너 가져와서
			break;
		}
	}

	_uint iTexNum = {};
	if (eState == ITEM_SAVE)
	{
		switch (MakeInfo.ItemType) // 아이템 타입에 따른 정보 조합
		{
		case ITEM_TYPE::ITEM_KEY1:
			MakeInfo.ItemDesc = L"- 일반 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_KEY2:
			MakeInfo.ItemDesc = L"- 일반  ";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_MEMORY:
			MakeInfo.ItemDesc = L"- 소비 -";
			iTexNum = 0;
			break;
		case ITEM_TYPE::ITEM_FORGIVEN:
			MakeInfo.ItemDesc = L"- 소비 -";
			iTexNum = 0;
			break;
		case ITEM_TYPE::ITEM_SKILLPIECE:
			MakeInfo.ItemDesc = L"- 기술의 파편 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_FIELDITEM:
			MakeInfo.ItemDesc = L"- 특별한 아이템 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_BADZOKER:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_BEOMSEUNG:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_FAKER:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_UIN:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_PLAYERMASTER:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;
		case ITEM_TYPE::ITEM_YUBHIN:
			MakeInfo.ItemDesc = L"- 비밀 이야기 -";
			iTexNum = 3;
			break;

		case ITEM_TYPE::ITEM_HERB_1:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_2:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_3:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_4:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_5:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_6:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		case ITEM_TYPE::ITEM_HERB_7:
			MakeInfo.ItemDesc = L"- 재료 -";
			iTexNum = 2;
			break;
		default:
			MakeInfo.ItemDesc = L"- 기술의 파편 -";
			iTexNum = 2;
			break;

		}
	}
	else if (eState == ITEM_DROP)
	{
		MakeInfo.ItemDesc = L"- 떨어뜨림 -";
		iTexNum = 1;
	}

	// 여기서 시작, 도착 좌표해서 보간하면 될 듯
	for (auto& Image : m_pItmeScreen->Find_UI_Image())
	{
		if (iNum == Image->Get_UI_GroupID()) // 배경 이미지 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(iTexNum);

			CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_LootNotifyBackground*>(Image)->Find_Component(TEXT("Com_Transform")));
			ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, 
				{ ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + 528.f ,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y });

		}
		if (iNum + 10 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(MakeInfo.ItemIconNum);

			CTransform* ImageTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_Item_Icon*>(Image)->Find_Component(TEXT("Com_Transform")));
			ImageTrans->Set_State_UIObj(CTransform::STATE_POSITION, 
				{ ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + 528.f,ImageTrans->Get_State_UIObj(CTransform::STATE_POSITION).y });

		}
	}
	for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
	{
		if (iNum == TextBox->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemName);
			
			CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
			BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, 
				{ BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + 528.f ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y });


		}
		if (iNum + 10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemDesc);

			CTransform* BoxTrans = dynamic_cast<CTransform*>(dynamic_cast<CUI_TextBox*>(TextBox)->Find_Component(TEXT("Com_Transform")));
			BoxTrans->Set_State_UIObj(CTransform::STATE_POSITION, 
				{ BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).x + 528.f ,BoxTrans->Get_State_UIObj(CTransform::STATE_POSITION).y });
		}
	}

}

void CUIGroup_PlayerScreen::Item_In_Out_Pop()
{
	for (auto& ItemInfo : m_pItmeScreen->Find_UI_Image())
	{
		if (1 == ItemInfo->Get_UI_GroupID())
		{

		}
		if ((10) == ItemInfo->Get_UI_GroupID())
		{

		}
	}


}

void CUIGroup_PlayerScreen::Button_Skill()
{

	if (static_cast<CPlayer*>(m_pPlayer)->Get_Skill_Number() == 1	
		&& !m_bSkillUse_Fix
		&& !m_bSkillUSe_Plunder
		&& 0 != dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_TexIcon()
		&& false == *m_pPlayerSkillCoolTime)
	{
		/* 고정 스킬*/
		dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_SkillOn(true);
		m_bSkillUse_Fix = true;
	
		printf("UIGroup_PlayerScreen 고정 스킬 : m_pPlayerSkillCoolTime => FALSE\n");

	}
	if (dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_EffectOn())
	{
		m_bSkillUse_Fix = false;
		m_pEffectSkill_2->Set_OnOff(true);
		dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_EffectOn(false);
		if (!dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().empty())
		{
			PLAYER_SKILL eSave = dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().front();
			dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().pop_front();
			dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().push_back(eSave);

			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_fIcon_CreativeTime(0.0f);
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_fIcon_CreativeTime(0.0f);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Skill_CoolTime(true);
			printf("UIGroup_PlayerScreen 고정 스킬 : m_pPlayerSkillCoolTime => TRUE\n");

		}
	}

	if (static_cast<CPlayer*>(m_pPlayer)->Get_Skill_Number() == 2
		&& !m_bSkillUse_Fix
		&& !m_bSkillUSe_Plunder
		&& 0 != dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Get_TexIcon()
		&& false == *m_pPlayerSkillCoolTime)
	{
		/* 약탈 스킬*/
		if (PLAYER_SKILL_START != dynamic_cast<CPlayer*>(m_pPlayer)->Get_Player_Take_Away_Skill()
			&& !dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Get_SkillOn())
		{
			printf("UIGroup_PlayerScreen 약탈 스킬 : m_pPlayerSkillCoolTime => FALSE\n");
			dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_SkillOn(true);
			dynamic_cast<CPlayer*>(m_pPlayer)->Set_Player_Take_Away_Skill(PLAYER_SKILL_START);
			m_bSkillUSe_Plunder = true;

		}
	}
	if (dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Get_EffectOn())
	{
		m_bSkillUSe_Plunder = false;
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_EffectOn(false);
		dynamic_cast<CPlayer*>(m_pPlayer)->Set_Skill_CoolTime(true);
		printf("UIGroup_PlayerScreen 약탈 스킬 : m_pPlayerSkillCoolTime => TRUE\n");

	}

	if (m_pGameInstance->isKeyEnter(DIK_C) && 
		!dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_SkillUpdate()&& 
		!dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_SkillChange())
	{
		dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_SkillChange(true);
		dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_SkillChange(true);
		dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_SkillChange(true);
		
	}
	if (dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_SkillUpdate()
		&& dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Get_SkillChange())
	{
		if (!dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().empty())
		{
			PLAYER_SKILL eSave = dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().front();
			dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().pop_front();
			dynamic_cast<CUIGroup_Skill*>(m_pGroupSkill)->Get_PlayerSkill_List().push_back(eSave);
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_2)->Set_SkillChange(false);
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_1)->Set_SkillChange(false);
			dynamic_cast<CUI_FixSlotFrame*>(m_pRevolvingSkill_3)->Set_SkillChange(false);
		}
	}

}

void CUIGroup_PlayerScreen::UI_Direction_Monster_MemoryGet()
{
}

HRESULT CUIGroup_PlayerScreen::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_Quest_Poison");

	return S_OK;
}

void CUIGroup_PlayerScreen::Ready_Skill_Slot()
{
	/*스킬 사용 연출을 위해 미리 멤버 변수로 설정하기*/
	for (auto& Slot : m_pMyScene->Find_UI_Image())
	{
		if (30 == Slot->Get_UI_GroupID())
		{
			m_pRevolvingSkill_1 = Slot;
		}
		if (31 == Slot->Get_UI_GroupID()) // 가운데
		{
			m_pRevolvingSkill_2 = Slot;
		}
		if (32 == Slot->Get_UI_GroupID())
		{
			m_pRevolvingSkill_3 = Slot;
		}
		if (33 == Slot->Get_UI_GroupID())
		{
			Slot->Set_OnOff(false);
			m_pEffectSkill_2 = Slot;
		}
		if (40 == Slot->Get_UI_GroupID())
		{
			m_pPlunderSkill = Slot;
		}
	}

}

void CUIGroup_PlayerScreen::Ready_Player_GageBar()
{
	/*HP,MP 연출을 위해 미리 멤버 변수로 설정하기*/
	for (auto& Gage : m_pMyScene->Find_UI_Image())
	{
		if (10 == Gage->Get_UI_GroupID())
			m_pHPGageBar = Gage;
		if (11 == Gage->Get_UI_GroupID())
			m_pHPGageTrack = Gage;
		if (20 == Gage->Get_UI_GroupID())
			m_pMPGageBar = Gage;
	}


}

HRESULT CUIGroup_PlayerScreen::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

CUIGroup_PlayerScreen* CUIGroup_PlayerScreen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerScreen* pInstance = new CUIGroup_PlayerScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerScreen::Clone(void* pArg)
{
	CUIGroup_PlayerScreen* pInstance = new CUIGroup_PlayerScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerScreen::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_PLAYERSCREEN);
}
