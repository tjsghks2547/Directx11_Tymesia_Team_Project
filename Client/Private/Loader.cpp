#include "pch.h" 
#include "Loader.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"

#pragma region 보스 
#include "Boss_Magician.h"
#include "Boss_Varg.h"
#include "Boss_Urd.h"

#include "Body_Varg.h"
#include "Body_Magician.h"

#include "VargKnife.h"
#include "Weapon_Cane.h"
#include "Weapon_Magician_Sword.h"

#include "UI_Boss_HP_Bar.h"
#include "UI_Boss_HP_Bar_Gage.h"
#include "UI_Boss_HP_Bar_Gage_Effect.h"
#include "UI_Boss_HP_Phase.h"
#pragma endregion 

#pragma region 엘리트 몬스터
#include "Elite_Joker.h"
#include "HArmorLV2.h"

#include "Body_Joker.h"
#include "Body_HArmorLV2.h"

#include "Joker_Weapon.h"
#include "Weapon_GreatSword.h"

#pragma endregion 

#pragma region 일반 몬스터
#include "Normal_VillageM0.h"
#include "Normal_VillageM1.h"
#include "Normal_VillageF0.h"
#include "Normal_VillageF1.h"

#include "Body_VillageM0.h"
#include "Body_VillageM1.h"
#include "Body_VillageF0.h"
#include "Body_VillageF1.h"

#include "Weapon_Axe.h"
#include "Weapon_Dagger.h"
#include "Weapon_Shield.h"

#include "Monster_HP_Bar.h"
#include "Monster_HP_Gage.h"
#include "Monster_HP_Gage_Effect.h"
#include "Monster_HP_Bar_Effect.h"
#include "Locked_On.h"
#pragma endregion 

#pragma region NPC

#include "Aisemy.h"
#include "Body_Aisemy.h"

#pragma endregion

#pragma region 플레이어 관련 
#include "Player.h"
#include "LeftWeapon.h"
#include "RightWeapon.h"
#include "ClawWeapon.h"
#include "PlayerCamera.h"
#ifdef _DEBUG
#include "Camera_Debug.h"
#endif // _DEBUG
#pragma endregion 

#pragma region 환경요소 
#include "Sky.h"
#include "Terrain.h"
#pragma endregion 

#pragma region Effect

#include "Effect_Mesh.h"
#include "Effect_Particle.h"

#pragma endregion

#pragma region 테스트용 사다리
#include "Ladder.h"
#pragma endregion

#pragma region  UIs
#include "UI_TextBox.h"

#include "UI_SkillIcon.h"
#include "UI_Effect_Frame.h"
#include "UI_Effect_SlotFrame.h"

#include "UIGroup_GameIntro.h"
#include "UIGroup_PlayerMenu.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UIGroup_PlayerTalent.h"
#include "UIGroup_PlayerScreen.h"
#include "UIGroup_Inventory.h"
#include "UIGroup_Loading.h"

#include "UI_MouseCursor.h"

#include "UI_NewHint.h"
#include "UI_LockedIcon.h"
#include "UI_LevelImage.h"
#include "UI_KeyBox_Long.h"
#include "UI_Indicator_Selected.h"
#include "UI_HightlightBar.h"
#include "UI_DiamondIcon.h"
#include "UI_RightBackground.h"
#include "UI_LeftBackground.h"
#include "UI_ButtonHighlight.h"
#include "UI_DecorationLine.h"

#include "UI_SceneAboveBackground.h"
#include "UI_SceneBackground.h"
#include "UI_SceneUnderBackground.h"
#include "UI_UnderLine.h"
#include "UI_Arrow.h"
#include "UI_Attribute_Slot_Active.h"
#include "UI_BaseTalentIndicator.h"
#include "UI_ContentBackground.h"
#include "UI_DescBackground.h"
#include "UI_Frame.h"

#include "UI_Arrow_Long.h"
#include "UI_BlackBar.h"
#include "UI_DecorationFrame.h"
#include "UI_ButtonBackground.h"
#include "UI_TitleBackgroundBar.h"


#include "UI_ItemBackground.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_Item_Icon.h"

#include "UI_Feather_Icon.h"
#include "UI_HPBar1_Border.h"
#include "UI_HPBar2_BG.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_MemoryShardsIcon.h"
#include "UI_MPBar1_Border.h"
#include "UI_MPBar2_BG.h"
#include "UI_MPBar3_MainBar.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_Potion_DefaultType.h"
#include "UI_DialogBackground.h"

#include "UI_GameLogoImage.h"
#include "UI_LoadingScreen.h"
#include "UI_LoadingIcon.h"
#include "UI_LootNotifyBackground.h"
#pragma endregion

#pragma region 오브젝트
#include "StaticObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "BillBoardObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "GroundObject.h"		// 인스턴싱용 게임오브젝트
#include "TriggerObject.h"		// 트리거용 게임오브젝트
#include "BlackScreen.h"
#pragma endregion

#include "GameItem.h"
#include "Button.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	/*switch (m_eNextLevelID)
	{
	case LEVEL_STATIC:
		hr = Loading_For_Level_Static();
		break;
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_TUTORIAL:
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.3f);
		hr = Loading_For_Level_Tutorial();
		break;
	}*/

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}
#ifdef _DEBUG	
void CLoader::SetUp_WindowText()	
{
	SetWindowText(g_hWnd, m_szLoadingText);
}
#endif

vector<string> CLoader::GetFBXFileNames(const string& folderPath)
{
	vector<string> fbxFiles;

	string searchPath = folderPath + "\\*.fbx";
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			string filename = findData.cFileName;
			size_t dot = filename.find_last_of('.');
			if (dot != string::npos)
				filename = filename.substr(0, dot);

			fbxFiles.push_back(filename);
		} while (FindNextFileA(hFind, &findData) != 0);

		FindClose(hFind);
	}

	return fbxFiles;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
