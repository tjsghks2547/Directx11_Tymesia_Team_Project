#include "pch.h" 
#include "Loader_Tutorial.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"

#pragma region 보스 
#include "Boss_Magician.h"
#include "Boss_Varg.h"

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

#pragma region 오브젝트
#include "StaticObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "BillBoardObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "GroundObject.h"		// 인스턴싱용 게임오브젝트
#include "TriggerObject.h"		// 트리거용 게임오브젝트
#include "BlackScreen.h"
#pragma endregion

#include "GameItem.h"
#include "Button.h"

CLoader_Tutorial::CLoader_Tutorial(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

HRESULT CLoader_Tutorial::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_Tutorial::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID) //혹시모르니 다시한번 검사하는 느낌
	{
	case LEVEL_TUTORIAL:
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);
		hr = Loading_For_Level_Tutorial();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_Tutorial::Loading_For_Level_Tutorial()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_Nor_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_ORM_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/MaskData0.dds"), 1))))
		return E_FAIL;
	//이전 Height
	//lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/TerrainHeight11.txt")))))
	//	return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr , TEXT("../Bin/DataFiles/HeightData/Tutorial/TerrainHeight20.txt")))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));

	// 03.19 네비 Tutorial Map 
	// 04.08 튜토리얼 맵 네비 11번이 그거 예전 조커 나왓던 그 나무 큰 거 잇는데 거기 안뚫는 그 파일임;; 
	//  난 분명 해놧는데 어느샌가! 누군가! 파일 바꿔놓은듯? 지건마렵;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/Tuto/TestNavigation22.txt")))))
		return E_FAIL;

#pragma endregion 






#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));

	//SM_Wall_Shelf
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Wall_Shelf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Sall_Shelf/SM_Wall_Shelf.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_Rag03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Rag03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Rag03/P_Rag03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodFence03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_WoodFence03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence03/SM_WoodFence03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodFence04
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_WoodFence04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence04/SM_WoodFence04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodStairs03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_WoodStairs03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodStairs03/SM_WoodStairs03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodStairs03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_HORSE_P_WoodenFrame02_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/HORSE_P_WoodenFrame02_05/HORSE_P_WoodenFrame02_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossAtriumCircle01
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircle01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircle01/P_BossAtriumCircle01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossCemetery_02_02
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossCemetery_02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_02_02/P_BossCemetery_02_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossCemetery_04
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossCemetery_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_04/P_BossCemetery_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossCemetery_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_05/P_BossCemetery_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossCemetery_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_06/P_BossCemetery_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossInteriorArches02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossInteriorArches02/P_BossInteriorArches02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_ChurchWindowGrilles01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_ChurchWindowGrilles01/P_ChurchWindowGrilles01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_LongStairs01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_LongStairs01/P_LongStairs01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Plains_CastleArch_Ruin_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Plains_CastleArch_Ruin_01/SM_Plains_CastleArch_Ruin_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Trim_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Trim_01a/SM_Trim_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01c/SM_Wall_8x8_Broken_01c.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01d/SM_Wall_8x8_Broken_01d.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01e"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01e/SM_Wall_8x8_Broken_01e.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing_base01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing_base01/Railing_base01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing_pillar01_2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing_pillar01_2/Railing_pillar01_2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing01_3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing01_3/Railing01_3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Brick_stone_stairs_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Brick_stone_stairs_1_a/SM_Brick_stone_stairs_1_a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Gate_17d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Gate_17d/SM_Gate_17d.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_ground_Road_Middle_250x250cm_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_ground_Road_Middle_250x250cm_1_a/SM_ground_Road_Middle_250x250cm_1_a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_T_P_BossRoomVines01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossRoomVines01/P_BossRoomVines01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down02/P_BossAtriumCircleRailing_Down02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down03/P_BossAtriumCircleRailing_Down03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down04/P_BossAtriumCircleRailing_Down04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Top01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Top01/P_BossAtriumCircleRailing_Top01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up01/P_BossAtriumCircleRailing_Up01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up02/P_BossAtriumCircleRailing_Up02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up04/P_BossAtriumCircleRailing_Up04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up03/P_BossAtriumCircleRailing_Up03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing03_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing03_1/Railing03_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Left01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Left01/P_Fortress_BossDoor_Left01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Right01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Right01/P_Fortress_BossDoor_Right01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Debris_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Debris_01a/SM_Debris_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Debris_02a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Debris_02a/SM_Debris_02a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Scafold_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Scafold_01b/SM_Scafold_01b.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Scafold_01c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Scafold_01c/SM_Scafold_01c.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_14"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_14/SM_fence_14.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_16/SM_fence_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_13/SM_fence_13.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CemeteryStairs01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_CemeteryStairs01/P_CemeteryStairs01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Brick_Floor"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Brick_Floor/brickFloor.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BaseWall_01_Corner"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_BaseWall_01_Corner.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BaseWall_02_Plain"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_BaseWall_02_Plain.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_LogPile_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_LogPile_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Separator"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Separator.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_crypt_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_crypt_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_crypt_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_crypt_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_crypt_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_curb_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Cloth08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_rock_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_rock_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_rock_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CemeteryStairs02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/P_CemeteryStairs02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CemeteryStairs03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/P_CemeteryStairs03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Stairs"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/SM_Stairs.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Stairs02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/SM_Stairs02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_urn_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_05"),

		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_12.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_19.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_21.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_22.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_23"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_23.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_24"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_24.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_25"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_25.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_26"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_26.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_27"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_27.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_29"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_29.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_30"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_30.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_31"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_31.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_32"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_32.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_33"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_33.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_34"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_34.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_fence_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Barrel_Closed"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Barrel/SM_Barrel_Closed.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_11"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_11.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_12.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_13.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_17"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_17.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_18"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_18.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_21.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_22.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL
		;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_23"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_23.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_25"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_25.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_26"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_26.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_27"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_27.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_28"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_28.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_29"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_29.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_31"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_31.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_32"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_32.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_33"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_33.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_34"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_34.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_36"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_36.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_37"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_37.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_38"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_38.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_41"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_41.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_42"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_42.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_44"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_44.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_46"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_46.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_48"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_48.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_50"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_50.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_51"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_51.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_53"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_53.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_54"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_54.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_55"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_55.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_56"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_56.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_60"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_60.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_61"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_61.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_62"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_62.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_gravestone_63"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_63.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Fence_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Fence_004.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Wall_Combined_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Wall_Combined_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Deceased06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_DeadTree_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_DeadTree_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_DeadTree_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_DeadTree_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree001"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree001.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree002"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree002.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree003"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree003.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree004"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree004.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree005"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree005.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree006"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree006_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_BigTree006_03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006_03_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_TreeAerialRoot01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_TreeAerialRoot01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_TreeAerialRoot02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_TreeAerialRoot02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BrokenTree01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_BrokenTree01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_DeadTree02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_DeadTree02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_DeadTree03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_DeadTree03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_DryTreeT3_branches_3_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_3_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_DryTreeT3_branches_5_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_5_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_11"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_11.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_14"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_14.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_19.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_28"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_28.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_36"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_36.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_38"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_38.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_39"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_39.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_40"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_40.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_41"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_41.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_tombstone_42"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_42.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing_base02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing_base02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing_base07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing_base07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing_base08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing_base08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing02_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing02_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing07_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing07_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing08_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing08_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing08_2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/Railing08_2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingDamage01_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage01_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingDamage01_3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage01_3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingDamage02_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage02_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingDamage05_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage05_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Railing06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingDamage07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingDamage07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_RailingPillarDamage01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railings/RailingPillarDamage01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Planks"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Planks.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Railing"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Railing.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_Tower"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Tower.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_chandelier04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/chandelier04/chandelier04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CircusBalloon01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusBalloon01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CircusBalloon02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusBalloon02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CircusballoonDamage01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusballoonDamage01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_CircusballoonDamage02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusballoonDamage02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_VariationBranch01_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch01_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_VariationBranch02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_VariationBranch03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_HORSE_P_WoodenStand02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/HORSE_P_WoodenStand02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BloodTent02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_BloodTent02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_BloodWoodChair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_BloodWoodChair01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Boss_Floor01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_Boss_Floor01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Boss_Floor02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_Boss_Floor02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_WoodenChair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_WoodenChair01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Cross"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Cross/Cross.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang01_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang01_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang02_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_Hang03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang03_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_P_HempRope"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_HempRope.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree01_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree01_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree01_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree01_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree02_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree02_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree02_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree03_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree03_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_tree03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree03_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Greenhouse_Elevator01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Greenhouse_Elevator/Greenhouse_Elevator01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_SM_WoodStair03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodStairs/SM_WoodStair03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_BillBoardObject"),
		CBillBoardObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region  인스턴싱용 환경 오브젝트

	lstrcpyW(m_szLoadingText, TEXT("인스턴싱용 환경 오브젝트"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Grass0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Grass0/Grass0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_Tree0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Tree0/Tree0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_House0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_0/House0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_Component_Model_House3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_3/House3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region 트리거 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_TriggerObject"),
		CTriggerObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


	/* 로딩이 완료되었습니다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader_Tutorial::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	return S_OK;
}

CLoader_Tutorial * CLoader_Tutorial::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_Tutorial*	pInstance = new CLoader_Tutorial(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_Tutorial");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Tutorial::Free()
{
	__super::Free();
}
