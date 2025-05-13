#include "pch.h" 
#include "Level_RoyalGarden.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	
#include "Level_Loading.h"

#include "Object.h"
#include "EnvironmentObject.h"
#include "TriggerObject.h"
#include "BlackScreen.h"

#include "DestructObject.h"
#include "BarrierScreen.h"

#include "Button.h"
#include "LockLine.h"
#include "GameItem.h"
#include "DeadBranch.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"

#include "Terrain.h"
#include "Water.h"

#include "Elevator_Door.h"
#include "DoorObject.h"
#include "DoorManager.h"
#include "LadderObject.h"
#include "Chair.h"
#include "ChairLamp.h"

CLevel_RoyalGarden::CLevel_RoyalGarden(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_RoyalGarden::Initialize()
{
	m_iCurrentLevel = LEVEL_ROYALGARDEN;

	m_pGameInstance->Set_Level_ForEventManager(m_iCurrentLevel);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))	
		return E_FAIL;		

	if (FAILED(Ready_Layer_Monster()))	
		return E_FAIL;

	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
			/*

	if (FAILED(Ready_Layer_Fade(TEXT("Layer_Fade"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_UIGroup_Inventory(TEXT("Layer_PlayerInventory"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerMenu(TEXT("Layer_PlayerMenu"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerLevelUP(TEXT("Layer_PlayerLevelUP"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerTalent(TEXT("Layer_PlayerTalent"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_Skill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_LandingMessage(TEXT("Layer_Landing"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_Dialogue(TEXT("Layer_Dialogue"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_GameItem"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	/*

	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);*/


	// 플레이어 화면 키기
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(L"Garden_Level_-_Exploration.ogg", 0.15f);

	/* 맵 이름 알림*/
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_Landing"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);

	return S_OK;
}

void CLevel_RoyalGarden::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_TAB))
	{
		if (!m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")))
		{
			m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_Organic_Magic_Accept_Quest_Drum_Impact_1.ogg"), CHANNELID::SOUND_UI, 0.2f);
			if (m_bStopMenuOpen)
			{

				m_bStopMenuOpen = false;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerInventory"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			}
			else
			{
				m_bStopMenuOpen = true;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true); // 마우스 이미지 켜기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerInventory"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
			}
		}
		
	}
	if (m_bNextLevelOpen)// 이 코드가 항상 Update 함수 마지막에 있어야 합니다.
	{
		m_pGameInstance->Clear_ItemInfo();
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_iNextLevel), 2, false));
	}
}

HRESULT CLevel_RoyalGarden::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Lights()
{
	CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_ROYALGARDEN, TEXT("Layer_Camera"), TEXT("Com_Transform")));

	_matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	_vector vCamInfo = { 60.f, 16.f / 9.f , 0.1f, 800.f };

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(200.7f, 100.3f, -1.f, 1.0f), XMVectorSet(100.f, 0.f, -1.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
		, matView, vCamInfo, pCamTransform)))
		return E_FAIL;

	CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();

	LIGHT_DESC LightDesc{};
	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 0.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.1f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.1f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.1f, 1.f, 1.f);
	LightDesc.iCurrentLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LightDesc));

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
	LightDesc.fRange = 3.f;
	LightDesc.iCurrentLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, pPlayerTransform)))
		return E_FAIL;

	FOGPARAMS FogDesc{};
	FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
	FogDesc.fFogStartDistance = _float2(0.04f, 8.f);
	FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
	FogDesc.g_FogColor = _float4(246.f / 255.f, 51.f / 255.f, 85.f / 255.f, 1.f);

	m_pGameInstance->Set_FogFactors(FogDesc);

	m_pGameInstance->Set_LightShaftValue(_float4(0.f, 0.f, 0.f, 0.f));

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
   CGameObject::GAMEOBJECT_DESC pDesc = {};

    pDesc.iCurLevel = m_iCurrentLevel;

    CTerrain::TERRAININFO pTerrainInfo = {};
    pTerrainInfo.vTerrainPos = _float4(-26.9196987f, 0.0f, -135.658707f, 1.0f);
    pTerrainInfo.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Terrain"), LEVEL_ROYALGARDEN, pLayerTag, &pTerrainInfo)))
        return E_FAIL;

	CWater::WATERINFO pWaterInfo = {};
	pWaterInfo.fPosition = _float4(-155.819336f, 13.8f, -148.f, 1.0f);
	pWaterInfo.iCurLevel = m_iCurrentLevel;
	pWaterInfo.fDullBlendFactor = 0.6f;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Water"), LEVEL_ROYALGARDEN, pLayerTag, &pWaterInfo)))
		return E_FAIL;
	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_ForkLift"), LEVEL_ROYALGARDEN, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Sky"), LEVEL_ROYALGARDEN, pLayerTag, &pDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	if (FAILED(Load_Objects(1)))// 정원
		return E_FAIL;

	if (FAILED(Load_Objects(2)))// 도서관
		return E_FAIL;

	if (FAILED(Load_Objects(5)))// 정원 보스맵
		return E_FAIL;

	if (FAILED(Load_TriggerObjects(5)))
		return E_FAIL;

	if (FAILED(Load_SpecificObjects(5)))
		return E_FAIL;

	if (FAILED(Load_DestructObjects(1)))
		return E_FAIL;

	CBarrierScreen::BARRIER_SCREEN_DESC     BarrierDesc = {};
	_float4 vBatEntrancePosition = { -7.1f, 16.2f, -38.4f, 1.f };           // 박쥐 보스방 나생문.
	BarrierDesc._fPosition = vBatEntrancePosition;
	BarrierDesc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_BarrierScreen"), m_iCurrentLevel, pLayerTag, &BarrierDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.0f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	//_float4 vTestPosition = { 78.8f , 13.75f, -66.235f, 1.f };		// 왕실정원 시작위치
	//_float4 vTestPosition = { 17.0f, 14.0f, -100.0f, 1.f };		// 해리포터책방두갈래길 시작위치
	//_float4 vTestPosition = { 101.0f, 14.0f, -97.0f, 1.f };		// 엘베 밑에 있는 시작 위치(근처에 첫 번 째 의자도 있음. ) 
	//_float4 vTestPosition = { -3.1f, 13.2f, -32.4f, 1.f };		// 보스 박쥐방 시작위치 
	_float4 vTestPosition = { -10.0f, 15.0f, -61.9f, 1.f };		// 두 번째 의자 박쥐 보스 방 직전.

	Desc._fPosition = vTestPosition;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera_Free::FREE_CAMERA_DESC		Desc = {};

	
	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	Desc.fFovy = XMConvertToRadians(50.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 800.f;
	Desc.fMouseSensor = 0.05f;
	Desc.fSpeedPerSec = 25.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Free"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Debug"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Monster()
{
	CGameObject::GAMEOBJECT_DESC pDesc = {};
	pDesc.iCurLevel = m_iCurrentLevel;

	for (_uint i = 0; i < 2; i++)
	{
 		Load_MonsterIndex(i);
		for (size_t j = 0; j < m_MonsterSpawnInfos[i].size(); j++)
		{
			switch (m_MonsterSpawnInfos[i][j].iMonsterIndex)
			{
			case NORMAL_VILLAGE_M0:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0"), CATEGORY_NORMAL, &pDesc)))
					return E_FAIL;
				break;
			case NORMAL_VILLAGE_M1:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1"), CATEGORY_NORMAL, &pDesc)))
					return E_FAIL;
				break;
			case NORMAL_SCYTHEM:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM"), CATEGORY_NORMAL, &pDesc)))
					return E_FAIL;
				break;
			case NORMAL_VILLAGE_F0:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0"), CATEGORY_NORMAL, &pDesc)))
					return E_FAIL;
				break;
			case NORMAL_VILLAGE_F1:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"), CATEGORY_NORMAL, &pDesc)))
					return E_FAIL;
				break;
			case ELITE_HARMORLV2:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2"), CATEGORY_ELITE, &pDesc)))
					return E_FAIL;
				break;
			case ELITE_PUNCH_MAN:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man"), CATEGORY_ELITE, &pDesc)))
					return E_FAIL;
				break;

			case ELITE_GRACE:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace"), CATEGORY_ELITE, &pDesc)))
					return E_FAIL;
				break;
			case ELITE_JOKER:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker"), CATEGORY_ELITE, &pDesc)))
					return E_FAIL;
				break;
			case ELITE_REASEARCHER:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Researcher"), CATEGORY_ELITE, &pDesc)))
					return E_FAIL;
				break;
			case BOSS_VARG:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg"), CATEGORY_BOSS, &pDesc)))
					return E_FAIL;
				break;

			case BOSS_ORD:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician"), CATEGORY_BOSS, &pDesc)))
					return E_FAIL;
				break;

			case BOSS_URD:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd"), CATEGORY_BOSS, &pDesc)))
					return E_FAIL;
				break;

			case BOSS_BAT:
				pDesc.fPosition = m_MonsterSpawnInfos[i][j].vMonsterPos;
				if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat"), CATEGORY_BOSS, &pDesc)))
					return E_FAIL;
				break;
			}
		}
	}
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_NPC(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GhostSemy"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), m_iCurrentLevel, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_Button(const _tchar* pLayerTag)
{
	CLockLine::LOCKLINE_DESC LineDesc = {};
	LineDesc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_LockLine"), m_iCurrentLevel, pLayerTag, &LineDesc)))
		return E_FAIL;

	CButton::BUTTON_DESC ButtonDesc = {};

	for (_uint i = 0; i < 1; ++i)
	{
		ButtonDesc._iButtonTypeIndex = i;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_InteractionButton"), m_iCurrentLevel, pLayerTag, &ButtonDesc)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CLevel_RoyalGarden::Ready_Layer_Item(const _tchar* pLayerTag)
{
	char* m_strObjectNames[256] =
	{
		"DeadBranch"
	};

	CGameItem::GAMEITEM_DESC ItemDesc = {};

	ItemDesc.iCurLevel = m_iCurrentLevel;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY1;
	ItemDesc.bTaken = true;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY2;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_MEMORY;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FORGIVEN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_SKILLPIECE;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FIELDITEM;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_BADZOKER;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_BEOMSEUNG;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FAKER;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_UIN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_PLAYERMASTER;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_YUBHIN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_1;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_2;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_3;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_4;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_5;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_6;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_HERB_7;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;
	//============================================================================================

	CDeadBranch::DEADBRANCH_DESC BranchDesc = {};
	BranchDesc.GameItemName = m_strObjectNames[0];
	BranchDesc.eItemType = ITEM_TYPE::ITEM_DEADBRANCH;
	BranchDesc.iCurLevel = m_iCurrentLevel;
	BranchDesc.bTaken = true;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_DeadBranch"), m_iCurrentLevel, pLayerTag, &BranchDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), m_iCurrentLevel, pLayerTag, &Desc, "PlayerLevelUp")))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), m_iCurrentLevel, pLayerTag, &Desc, "PlayerScreen")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag)
{

	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"), m_iCurrentLevel, pLayerTag, &Desc, "Inventory")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_LandingMessage(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Landing"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_Dialogue(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Dialogue"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_Skill(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Skill"), m_iCurrentLevel, pLayerTag, &Desc, "UI_Skill")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_Objects(_int iObject_Level)
{
	_ulong dwByte = {};
	_ulong dwByte2 = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/Level_Royal_Garden/ObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;
	_uint iSize2 = 0;

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iSize; i++)
	{
		CObject::OBJECT_DESC Desc{};

		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iObjectType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iBillBoardMeshNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &Desc.vColor, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fEmissivePower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iGlassNum, sizeof(_uint), &dwByte, nullptr);
		
		Desc.ObjectName = szLoadName;

		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		if (Desc.iObjectType == CObject::OBJECT_DEFAULT)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_StaticObject"), m_iCurrentLevel, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_BillBoardObject"), m_iCurrentLevel, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_EMISSIVE)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveObject"), m_iCurrentLevel, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_GLASS)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GlassObject"), m_iCurrentLevel, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
	}

	ReadFile(hFile, &iSize2, sizeof(_uint), &dwByte2, nullptr);
	CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};

	vector<_int> vecBoxSize;
	_int vectorBoxSize = {};

	for (_uint i = 0; i < iSize2; ++i)
	{
		CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};
		_char szLoadName[MAX_PATH] = {};

		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);
		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		Desc.ObjectName = szLoadName;

		ReadFile(hFile, &Desc.isCullingObject, sizeof(_bool), &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iInstanceType, sizeof(_uint), &dwByte2, nullptr);

		_uint iInstanceCount = 0;
		ReadFile(hFile, &iInstanceCount, sizeof(_uint), &dwByte2, nullptr);



		vector<VTX_MODEL_INSTANCE> vecInstanceData(iInstanceCount);
		vector<XMFLOAT3> vecInstancePosition(iInstanceCount);
		vector<XMFLOAT3> vecInstanceScale(iInstanceCount);
		vector<XMFLOAT4> vecInstanceRotation(iInstanceCount);

		vectorBoxSize++;
		for (_uint k = 0; k < iInstanceCount; ++k)
		{
			ReadFile(hFile, &vecInstanceData[k].InstanceMatrix, sizeof(XMFLOAT4X4), &dwByte2, nullptr);

			XMFLOAT4X4 matrix;
			memcpy(&matrix, vecInstanceData[k].InstanceMatrix, sizeof(XMFLOAT4X4));

			XMMATRIX matWorld = XMLoadFloat4x4(&matrix);

			XMVECTOR scale, rotation, translation;
			XMMatrixDecompose(&scale, &rotation, &translation, matWorld);

			XMStoreFloat3(&vecInstancePosition[k], translation);
			XMStoreFloat3(&vecInstanceScale[k], scale);

			XMFLOAT4 quaternion;
			ReadFile(hFile, &quaternion, sizeof(XMFLOAT4), &dwByte2, nullptr);
			rotation = XMLoadFloat4(&quaternion);

			vecInstanceRotation[k] = quaternion;

			vecBoxSize.resize(vectorBoxSize);
			ReadFile(hFile, &vecBoxSize[i], sizeof(_int), &dwByte2, nullptr);
		}

		Desc.vecInstancePosition = vecInstancePosition;
		Desc.vecInstanceScale = vecInstanceScale;
		Desc.vecInstanceRotation = vecInstanceRotation;
		Desc.vecBoxSize = vecBoxSize;

		switch (Desc.iInstanceType)
		{
		case CEnvironmentObject::ENV_DEFAULT:
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GroundObject"), m_iCurrentLevel, TEXT("Layer_GroundObject"), &Desc)))
				return E_FAIL;
			break;
		case CEnvironmentObject::ENV_EMISSIVE:
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveGroundObject"), m_iCurrentLevel, TEXT("Layer_GroundObject"), &Desc)))
				return E_FAIL;
			break;
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_TriggerObjects(_int iObject_Level)
{
	string strDataPath = "../Bin/DataFiles/TriggerData/TriggerObject";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;

	DWORD dwByte = 0;
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	vector<CTriggerObject*>		vecTriggerObject;

	vecTriggerObject.resize(iSize);

	CTriggerObject::TC_INFO Info = {};
	for (size_t i = 0; i < iSize; i++)
	{
		CTriggerObject::TC_DESC Desc{};
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScale, sizeof(_float3), &dwByte, nullptr);

		CTriggerObject* pTriggerObject = reinterpret_cast<CTriggerObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_TriggerObject"), LEVEL_ROYALGARDEN, TEXT("Layer_TriggerObject"), &Desc));

		if (nullptr != pTriggerObject)
			vecTriggerObject.push_back(pTriggerObject);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_InstancingObjects(_int iObject_Level)
{
	_ulong dwByte = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/ObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_Height(_int iObject_Level)
{
	_ulong dwByte = {};

	string strDataPath = "../Bin/DataFiles/HeightData/TerrainHeight";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint numVertices = {};
	ReadFile(hFile, &numVertices, sizeof(_uint), &dwByte, nullptr);



	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_MonsterIndex(_int iMonsterIndex_Level)
{
	string strDataPath = "../Bin/DataFiles/SpawnPoint/Royal_Garden/SpawnPoint";

	strDataPath = strDataPath + to_string(iMonsterIndex_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load SpawnPoint File!");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_uint iSize = 0;

	// 일반 오브젝트
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	_float4 vMonsterPos = {};
	_int	iMonsterIndex = { -1 };

	for (size_t i = 0; i < iSize; i++)
	{
		MONSTERSPAWNINFO SpawnInfo = {};

		ReadFile(hFile, &SpawnInfo.vMonsterPos,sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &SpawnInfo.iMonsterIndex, sizeof(_int), &dwByte, nullptr);

		if (iMonsterIndex < -1)
		{
			CloseHandle(hFile);

			MSG_BOX("Failed To Load SpawnPoint File!");
			return E_FAIL;
		}

		m_MonsterSpawnInfos[iMonsterIndex_Level].push_back(SpawnInfo);
	}

	CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_SpecificObjects(_int iObject_Level)
{
	string strDataPath = "../Bin/DataFiles/SpecificObjectData/Royal_Garden/SpecificObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_uint iSize = 0;
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);


	_uint iChairNum = 0;
	_uint iLampNum = 0;
	_uint iElevatorUp = 0;
	_uint iElevatorDown = 0;
	for (size_t i = 0; i < iSize; i++)
	{
		CSpecificObject::SpecificObject_Desc Desc{};

		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);

		string strName = szLoadName;
		Desc.iCurLevel = m_iCurrentLevel;
		CGameObject* pObject = nullptr;

		_uint iLadderDownNum = { 0 };
		_uint iLadderUpNum = { 0 };

		if (strName == "P_Archive_Chair01")
		{
			Desc.iPairNum = iChairNum;
			Desc.ObjectName = strName + "_" + to_string(iChairNum);
			pObject = reinterpret_cast<CChair*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Chair"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &Desc));
			++iChairNum;
		}
		else if (strName == "NPCLamp")
		{
			Desc.iPairNum = iLampNum;
			Desc.ObjectName = strName + "_" + to_string(iLampNum);
			pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_ChairLamp"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &Desc));
			++iLampNum;
		}
		else if (strName == "Elevator_Up")
		{
			if (iElevatorUp == 1)
			{
				CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();
				CNavigation* pPlayerNavigation = reinterpret_cast<CNavigation*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Find_Component(TEXT("Com_Navigation")));

				CElevator_Door::ElevatorDoorDesc ElevatorDesc;

				ElevatorDesc.fPosition = Desc.fPosition;
				ElevatorDesc.fRotation = Desc.fRotation;
				ElevatorDesc.fScaling = Desc.fScaling;
				ElevatorDesc.fFrustumRadius = Desc.fFrustumRadius;

				ElevatorDesc.iCurLevel = m_iCurrentLevel;
				ElevatorDesc.isCulling = false;
				ElevatorDesc.pCullingLayerTag = TEXT("");
				ElevatorDesc.pLoadLayerTag = TEXT("");
				//ElevatorDesc.iLoadIndex.push_back(314);
				ElevatorDesc.pPlayerTransform = pPlayerTransform;
				ElevatorDesc.pPlayerNavigation = pPlayerNavigation;

				ElevatorDesc.iPairNum = iElevatorUp;
				ElevatorDesc.ObjectName = strName + "_" + to_string(iElevatorUp);
				pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Elevator_Door"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &ElevatorDesc));
			}
			else
			{
				CNavigation* pPlayerNavigation = reinterpret_cast<CNavigation*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Find_Component(TEXT("Com_Navigation")));

				CElevator_Door::ElevatorDoorDesc ElevatorDesc;

				ElevatorDesc.fPosition = Desc.fPosition;
				ElevatorDesc.fRotation = Desc.fRotation;
				ElevatorDesc.fScaling = Desc.fScaling;
				ElevatorDesc.fFrustumRadius = Desc.fFrustumRadius;

				ElevatorDesc.iCurLevel = m_iCurrentLevel;
				ElevatorDesc.isCulling = false;
				ElevatorDesc.pCullingLayerTag = TEXT("");
				ElevatorDesc.pLoadLayerTag = TEXT("");

				ElevatorDesc.pPlayerNavigation = pPlayerNavigation;

				ElevatorDesc.iPairNum = iElevatorUp;
				ElevatorDesc.ObjectName = strName + "_" + to_string(iElevatorUp);
				pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Elevator_Door"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &ElevatorDesc));
			}
			++iElevatorUp;

		}
		else if (strName == "Elevator_Down")
		{
			if (iElevatorDown == 0)
			{
				CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();
				CNavigation* pPlayerNavigation = reinterpret_cast<CNavigation*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Find_Component(TEXT("Com_Navigation")));

				CElevator_Door::ElevatorDoorDesc ElevatorDesc;

				ElevatorDesc.fPosition = Desc.fPosition;
				ElevatorDesc.fRotation = Desc.fRotation;
				ElevatorDesc.fScaling = Desc.fScaling;
				ElevatorDesc.fFrustumRadius = Desc.fFrustumRadius;

				ElevatorDesc.iCurLevel = m_iCurrentLevel;
				ElevatorDesc.isCulling = false;
				//ElevatorDesc.iLoadIndex.push_back(313);
				//ElevatorDesc.iLoadIndex.push_back(319);
				ElevatorDesc.pCullingLayerTag = TEXT("");
				ElevatorDesc.pLoadLayerTag = TEXT("");
				ElevatorDesc.pPlayerTransform = pPlayerTransform;
				ElevatorDesc.pPlayerNavigation = pPlayerNavigation;

				ElevatorDesc.iPairNum = iElevatorDown;
				ElevatorDesc.ObjectName = strName + "_" + to_string(iElevatorDown);
				pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Elevator_Door"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &ElevatorDesc));
			}
			else
			{
				CNavigation* pPlayerNavigation = reinterpret_cast<CNavigation*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Player"), "PLAYER")->Find_Component(TEXT("Com_Navigation")));

				CElevator_Door::ElevatorDoorDesc ElevatorDesc;

				ElevatorDesc.fPosition = Desc.fPosition;
				ElevatorDesc.fRotation = Desc.fRotation;
				ElevatorDesc.fScaling = Desc.fScaling;
				ElevatorDesc.fFrustumRadius = Desc.fFrustumRadius;

				ElevatorDesc.iCurLevel = m_iCurrentLevel;
				ElevatorDesc.isCulling = false;
				ElevatorDesc.pCullingLayerTag = TEXT("");
				ElevatorDesc.pLoadLayerTag = TEXT("");

				ElevatorDesc.pPlayerNavigation = pPlayerNavigation;

				ElevatorDesc.iPairNum = iElevatorDown;
				ElevatorDesc.ObjectName = strName + "_" + to_string(iElevatorDown);
				pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Elevator_Door"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &ElevatorDesc));
			}
			++iElevatorDown;

		}
		else if ((strName == "P_Ladder02_Down"))
		{
			CLadderObject::LADDER_DESC LadderDesc;
			LadderDesc.fPosition = Desc.fPosition;
			LadderDesc.fRotation = Desc.fRotation;
			LadderDesc.fScaling = Desc.fScaling;
			LadderDesc.fFrustumRadius = Desc.fFrustumRadius;

			LadderDesc.iCurLevel = m_iCurrentLevel;
			LadderDesc.iObjectNumber = iLadderDownNum;
			LadderDesc.ObjectName = strName + "_" + to_string(iLadderDownNum);
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Ladder_Object"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &LadderDesc)))
				return E_FAIL;
			++iLadderDownNum;
		}
		else if (strName == "P_Ladder02_Up")
		{
			CLadderObject::LADDER_DESC LadderDesc;
			LadderDesc.fPosition = Desc.fPosition;
			LadderDesc.fRotation = Desc.fRotation;
			LadderDesc.fScaling = Desc.fScaling;
			LadderDesc.fFrustumRadius = Desc.fFrustumRadius;

			LadderDesc.iCurLevel = m_iCurrentLevel;
			LadderDesc.iObjectNumber = iLadderUpNum;
			LadderDesc.ObjectName = strName + "_" + to_string(iLadderUpNum);
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Ladder_Object"), m_iCurrentLevel, TEXT("Layer_SpecificObject"), &LadderDesc)))
				return E_FAIL;
			++iLadderUpNum;
		}
	}

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_DestructObjects(_int iObject_Level)
{
	string strDataPath = "../Bin/DataFiles/DestructObjectData/Royal_Garden/DestructObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_uint iSize = 0;
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iSize; i++)
	{
		CDestructObject::DestructObject_Desc Desc{};

		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);

		Desc.ObjectName = szLoadName;
		Desc.iCurLevel = m_iCurrentLevel;

		CDestructObject* pObject = nullptr;
		if (pObject == nullptr)
		{
			pObject = reinterpret_cast<CDestructObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_DestructObject"), m_iCurrentLevel, TEXT("Layer_DestructObject"), &Desc));
		}
	}

	return S_OK;
}


CLevel_RoyalGarden * CLevel_RoyalGarden::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_RoyalGarden*	pInstance = new CLevel_RoyalGarden(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_RoyalGarden");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_RoyalGarden::Free()
{
	__super::Free();

	m_pGameInstance->Delete_All_Monster();
	m_pGameInstance->Reset_Effect();

	m_pGameInstance->StopSlowly(CHANNELID::SOUND_BGM);
	m_pGameInstance->Delete_Static_Light(m_iCurrentLevel);
}
