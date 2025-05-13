#include "pch.h" 
#include "Level_Hill.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	
#include "Level_Loading.h"

#include "Object.h"
#include "EnvironmentObject.h"
#include "TriggerObject.h"
#include "BlackScreen.h"

#include "UI_LeftBackground.h"

#include "Button.h"
#include "GameItem.h"
#include "DeadBranch.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"

#include "Terrain.h"

CLevel_Hill::CLevel_Hill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_Hill::Initialize()
{
	m_iCurrentLevel = LEVEL_HILL;

	m_pGameInstance->Set_Level_ForEventManager(m_iCurrentLevel);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))	
		return E_FAIL;		

	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	//CGameObject::GAMEOBJECT_DESC pDesc = {};
	//pDesc.iCurLevel = m_iCurrentLevel;
	//pDesc.fPosition = { -22.35f,1.f,-18.f,1.0f };
	//m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2"), CATEGORY_BOSS, &pDesc);

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
	
	if (FAILED(Ready_Layer_UIGroup_MapChange(TEXT("Layer_MapChange"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_LandingMessage(TEXT("Layer_Landing"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_Dialogue(TEXT("Layer_Dialogue"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_GameItem"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	//m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
	//m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);
	//m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_AUTO);


	// 플레이어 화면 키기
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_PlayerScreen"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	/*m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(L"Safe_House_-_Long.ogg", 0.8f);*/

	/* 맵 이름 알림*/
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_Landing"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);

	m_pGameInstance->Initialize_IsInWater(false);

	return S_OK;
}

void CLevel_Hill::Update(_float fTimeDelta)
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
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_PlayerInventory"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			}
			else
			{
				m_bStopMenuOpen = true;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true); // 마우스 이미지 켜기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_PlayerInventory"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_HILL, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
			}
		}
		
	}

	if (m_bNextLevelOpen)// 이 코드가 항상 Update 함수 마지막에 있어야 합니다.
	{
		_int iLoadingImage = {};
		m_pGameInstance->Clear_ItemInfo();
		switch (m_iNextLevel)
		{
		case LEVEL_SEAOFTREES:
			iLoadingImage = 1;
			break;
		case LEVEL_ROYALGARDEN:
			iLoadingImage = 3;
			break;
		case LEVEL_OCEAN:
			iLoadingImage = 5;
			break;
		}
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_iNextLevel), iLoadingImage, false));
	}

}

HRESULT CLevel_Hill::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Hill::Ready_Lights()
{

	//CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_SEAOFTREES, TEXT("Layer_Camera"), TEXT("Com_Transform")));

	CTransform* pCamTransform = (m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"))->Get_Transfrom();


	_matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	_vector vCamInfo = { 60.f, 16.f / 9.f , 0.1f, 800.f };

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(200.7f, 100.3f, -1.f, 1.0f), XMVectorSet(100.f, 0.f, -1.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
		, matView, vCamInfo, pCamTransform)))
		return E_FAIL;

	CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_HILL, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();

	LIGHT_DESC LightDesc{};
	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 0.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.iCurrentLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LightDesc));

	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	//LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
	//LightDesc.fRange = 3.f;
	//LightDesc.iCurrentLevel = m_iCurrentLevel;

	//if (FAILED(m_pGameInstance->Add_Light(LightDesc, pPlayerTransform)))
	//	return E_FAIL;

	FOGPARAMS FogDesc{};
	FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
	FogDesc.fFogStartDistance = _float2(0.05f, 8.f);
	FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
	FogDesc.g_FogColor = _float4(0.5f, 0.5f, 0.5f, 1.f);

	m_pGameInstance->Set_FogFactors(FogDesc);

	m_pGameInstance->Set_LightShaftValue(_float4(1.f, 0.97f, 1.f, 1.f));

	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC pDesc = {};

	pDesc.iCurLevel = m_iCurrentLevel;

	CTerrain::TERRAININFO pTerrainInfo = {};
	pTerrainInfo.vTerrainPos = _float4(-50.0f, 0.0f, -100.0f, 1.0f);
	pTerrainInfo.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_Terrain"), LEVEL_HILL, pLayerTag, &pTerrainInfo)))
		return E_FAIL;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_ForkLift"), LEVEL_HILL, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_Sky"), LEVEL_HILL, pLayerTag, &pDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	//현재 몬스터와 기본맵이 있는 Prototype용 맵							-> Load_Objects(16);
	//Map Tool 기능 및 Test용 맵											-> Load_Objects(87);
	//튜토리얼 맵 수정 중 (크기 조절 중 98번 맵파일은 잠시 봉인합니다.		-> Load_Objects(107);
	// 
	//Load_Objects(140); //Tutorial Map
	//Load_Objects(142); //Tutorial Map
	if (FAILED(Load_Objects(193)))
		return E_FAIL;//HILL Map
	//Load_Objects(301); //Circus Map
	//Load_Objects(303); //Circus Map


	//Load_TriggerObjects(0);			// 원래 의자 쪽에 있었던 트리거 오브젝트 파일
	Load_TriggerObjects(1);				// 이제 보스 입구 쪽에 심어져있는 파일임.
	/* 여기서 맵 파일 하나하나 다 읽어와야함 */

	//_ulong dwByte = {}; 
	////HANDLE hFile = CreateFile(TEXT("../Map_File/real76.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(TEXT("../Map_File/real143.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//while (true)
	//{
	//	_tchar Layer_Name[MAX_PATH] = {}; //레이어 이름										
	//	ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

	//	if (dwByte == 0)
	//		break;
	//	/* 이름 작업 */
	//	_char   Prototype_Name[MAX_PATH] = {};

	//	ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


	//	_float4x4 WorldMatrix = {};
	//	ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
	//	//int a = 4;

	//	_tchar Translate_wchar[MAX_PATH] = {};
	//	MultiByteToWideChar(CP_ACP, 0, Prototype_Name, static_cast<_int>(strlen(Prototype_Name)), Translate_wchar, MAX_PATH);

	//	/* 이제 TRANSFORM만 건들면 될듯함.*/
	//	//int b = 4;
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, Translate_wchar, LEVEL_HILL, Layer_Name)))
	//		return E_FAIL;

	//	//CTransform* pTrasnform = dynamic_cast<CTransform*>(
	//	//* Find GaemObject 만들어야 할듯
	//	// 구분할 수 있는 방법을 생각해봐야할듯.
	//	map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();		

	//	auto& Level_Hill = Level_Layers[3];

	//	for (auto& Layers : Level_Hill)
	//	{
	//		//auto& iter = find(Level_Hill.begin(), Level_Hill.end(), Layer_Name);	
	//		auto iter = Level_Hill.find(Layer_Name);

	//		if (iter == Level_Hill.end())
	//			return E_FAIL;

	//		else
	//		{
	//			CTransform* pTranform = dynamic_cast<CTransform*>(
	//				iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));	

	//			pTranform->Set_WorldMatrix(WorldMatrix);	
	//		}
	//	}
	//}
	//CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	_float4 vTestPosition = { 3.45f, 1.10f, -15.61f, 1.0f }; //의자 옆 위치  // 3월 27일	

	Desc._fPosition = vTestPosition;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Debug"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;
#endif // _DEBUG

	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_NPC(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.fPosition = _float4{ -1.6f, 1.1f, -10.9f, 1.f };
	Desc.fScaling = _float3{ 0.0025f, 0.0025f, 0.0025f };

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Hill::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), LEVEL_HILL, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Hill::Ready_Layer_Button(const _tchar* pLayerTag)
{
	CButton::BUTTON_DESC ButtonDesc = {};

	for (_uint i = 0; i < 1; ++i)
	{
		ButtonDesc._iButtonTypeIndex = i;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_InteractionButton"), LEVEL_HILL, pLayerTag, &ButtonDesc)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CLevel_Hill::Ready_Layer_Item(const _tchar* pLayerTag)
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
	BranchDesc.bTaken = true;

	BranchDesc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_DeadBranch"), LEVEL_HILL, pLayerTag, &BranchDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Hill::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_HILL, pLayerTag, &Desc, "PlayerLevelUp")))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Hill::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Hill::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_HILL, pLayerTag, &Desc, "PlayerScreen")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"), LEVEL_HILL, pLayerTag, &Desc, "Inventory")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_MapChange(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_MapChange"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_LandingMessage(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Landing"), LEVEL_HILL, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_Dialogue(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Dialogue"), LEVEL_HILL, pLayerTag,&Desc)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Ready_Layer_UIGroup_Skill(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Skill"), LEVEL_HILL, pLayerTag, &Desc, "UI_Skill")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Hill::Load_Objects(_int iObject_Level)
{
	_ulong dwByte = {};
	_ulong dwByte2 = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/Level_Philsopers_Hill/ObjectData";

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

		Desc.ObjectName = szLoadName;
		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		CObject* pObject = nullptr;

		if (Desc.iObjectType == CObject::OBJECT_DEFAULT)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_HILL, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_Object_BillBoardObject"), LEVEL_HILL, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
	}

	ReadFile(hFile, &iSize2, sizeof(_uint), &dwByte2, nullptr);
	CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};

	vector<_int> vecBoxSize;
	_int vectorBoxSize = {};

	/*for (_uint i = 0; i < iSize2; ++i)
	{
		_uint iVecInstanceDataSize = 0;
		ReadFile(hFile, &iVecInstanceDataSize, sizeof(_uint), &dwByte2, nullptr);

		vector<VTX_MODEL_INSTANCE> vecInstanceData = {};
		vecInstanceData.resize(iVecInstanceDataSize);

		for (_uint i = 0; i < iVecInstanceDataSize; ++i)
		{
			ReadFile(hFile, &vecInstanceData[i].InstanceMatrix, sizeof(XMFLOAT4X4), &dwByte2, nullptr);
		}
	}

	_uint iInstancedGroundObjectNumSize = 0;
	ReadFile(hFile, &iInstancedGroundObjectNumSize, sizeof(_uint), &dwByte2, nullptr);


	vector<_float3>                         vecInstancedGroundObjectPos;
	vector<_float3>                         vecInstancedGroundObjectScale;
	vector<_float3>                         vecInstancedGroundObjectRotation;

	vecInstancedGroundObjectPos.resize(iInstancedGroundObjectNumSize);
	vecInstancedGroundObjectScale.resize(iInstancedGroundObjectNumSize);
	vecInstancedGroundObjectRotation.resize(iInstancedGroundObjectNumSize);

	for (size_t i = 0; i < vecInstancedGroundObjectPos.size(); i++)
	{
		_float3 fGroundObjectPos;
		ReadFile(hFile, &fGroundObjectPos, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectPos[i] = (fGroundObjectPos);
		Desc.vecInstancePosition.push_back(fGroundObjectPos);
	}

	for (size_t i = 0; i < vecInstancedGroundObjectScale.size(); i++)
	{
		_float3 fGroundObjectScale;
		ReadFile(hFile, &fGroundObjectScale, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectScale[i] = (fGroundObjectScale);
		Desc.vecInstanceScale.push_back(fGroundObjectScale);
	}

	for (size_t i = 0; i < vecInstancedGroundObjectRotation.size(); i++)
	{
		_float3 fGroundObjectRotation;
		ReadFile(hFile, &fGroundObjectRotation, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectRotation[i] = (fGroundObjectRotation);
		Desc.vecInstanceRotation.push_back(fGroundObjectRotation);
	}

	for (size_t i = 0; i < iSize2; i++)
	{
		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		Desc.ObjectName = szLoadName;

		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_HILL, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_HILL, TEXT("Layer_GroundObject"), &Desc)))
			return E_FAIL;
	}*/

	for (_uint i = 0; i < iSize2; ++i)
	{
		CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};
		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		Desc.ObjectName = szLoadName;

		ReadFile(hFile, &Desc.isCullingObject, sizeof(_bool), &dwByte2, nullptr);

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
		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		CEnvironmentObject* pEnvironment = reinterpret_cast<CEnvironmentObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_HILL, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_HILL, TEXT("Layer_GroundObject"), &Desc));

		if (nullptr == pEnvironment)
			return E_FAIL;

		//if (pEnvironment != nullptr)
		//{
		//	pEnvironment->Set_ModelInstanceVector(vecInstanceData);
		//	m_EnvironmentObjects.push_back(pEnvironment);

		//	m_vecInstancedGroundObjectPos.insert(m_vecInstancedGroundObjectPos.end(), vecInstancePosition.begin(), vecInstancePosition.end());
		//	m_vecInstancedGroundObjectScale.insert(m_vecInstancedGroundObjectScale.end(), vecInstanceScale.begin(), vecInstanceScale.end());
		//	m_vecInstancedGroundObjectRotation.insert(m_vecInstancedGroundObjectRotation.end(), vecInstanceRotation.begin(), vecInstanceRotation.end());
		//	//m_vecBoxSize.insert(m_vecBoxSize.end(), vecBoxSize.begin(), vecBoxSize.end());
		//	m_vecBoxSize = vecBoxSize;


		//	for (_uint t = 0; t < vecBoxSize.size(); ++t)
		//	{
		//		dynamic_cast<CGroundObject*>(pEnvironment)->Set_BoxSize(vecBoxSize[t]);
		//	}
		//}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_Hill::Load_TriggerObjects(_int iObject_Level)
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

		CTriggerObject* pTriggerObject = reinterpret_cast<CTriggerObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_HILL, TEXT("Prototype_GameObject_TriggerObject"), LEVEL_HILL, TEXT("Layer_TriggerObject"), &Desc));

		if (nullptr != pTriggerObject)
			vecTriggerObject.push_back(pTriggerObject);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_Hill::Load_InstancingObjects(_int iObject_Level)
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

HRESULT CLevel_Hill::Load_Height(_int iObject_Level)
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


CLevel_Hill * CLevel_Hill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Hill*	pInstance = new CLevel_Hill(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Hill");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Hill::Free()
{
	__super::Free();

	m_pGameInstance->Reset_Effect();

	m_pGameInstance->Delete_Static_Light(m_iCurrentLevel);
}
