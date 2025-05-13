#include "pch.h" 
#include "Loader_Hill.h"
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

CLoader_Hill::CLoader_Hill(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

HRESULT CLoader_Hill::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_Hill::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_HILL: //혹시모르니 다시한번 검사하는 느낌
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);
		hr = Loading_For_Level_Hill();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_Hill::Loading_For_Level_Hill()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_Nor_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_ORM_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/MaskData0.dds"), 1))))
		return E_FAIL;
	//이전 Height
	//lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/TerrainHeight11.txt")))))
	//	return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr , TEXT("../Bin/DataFiles/HeightData/Hill/TerrainHeight18.txt")))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	

	// 03.27 네비 Hill Map 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/Hill/PhilosopherNavigation.txt")))))
		return E_FAIL;

#pragma endregion 



#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));

	_matrix ObjectPreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_candle01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/candle/candle01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_candle02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/candle/candle02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_chandelier02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/candle/chandelier02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_candle01_fire"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/candle/candle01_fire.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_candle02_fire"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/candle/candle02_fire.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_08.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_10.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_Arches_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_Arches_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ruins_Arches_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Ruin/SM_Ruins_Arches_07.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Canopy01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Canopy01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_ChimneyLarge02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_ChimneyLarge02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Plank3m02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Plank3m02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Planks"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Planks.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_WoodChunks02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_WoodChunks02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_WoodFence03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_WoodFence03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_WoodFence04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_WoodFence04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_WoodStairs01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_WoodStairs01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_WoodStairs03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_WoodStairs03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_Aset_rock_volcanic_L_qjben_LOD0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/Aset_rock_volcanic_L_qjben_LOD0.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_tkerbglda_LOD3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/tkerbglda_LOD3.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_tjrsfbdda_LOD3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/tjrsfbdda_LOD3.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_Aset_wood_log_M_rhfdj_LOD3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/Aset_wood_log_M_rhfdj_LOD3.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_shxxJ_LOD0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/shxxJ_LOD0.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_siExJ_LOD0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/siExJ_LOD0.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_taDvf_LOD3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/taDvf_LOD3.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_tgykdcnba_LOD3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Rocks/tgykdcnba_LOD3.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Debris_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Debris/SM_Debris_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Debris_02a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Debris/SM_Debris_02a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_bench_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/bench_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_bench_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/bench_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_candlestick_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/candlestick_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_crossbow_chute"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/crossbow_chute.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_cup_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/cup_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_cupboard_big_base"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/cupboard_big_base.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_cupboard_big_door_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/cupboard_big_door_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_cupboard_big_door_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/cupboard_big_door_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_jar_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/jar_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_jar_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/jar_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_jar_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/jar_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_ladder_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/ladder_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_roof_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/roof_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_roof_triangle"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/roof_triangle.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_rug_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/rug_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_spoon_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/spoon_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_table_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/table_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_wall_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/wall_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_wall_window"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Archery/wall_window.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wood_Wall_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Wood_Wall_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wood_Wall_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Wood_Wall_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Plank_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Plank_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Plank_Broken_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Plank_Broken_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Bag_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Bag_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Bag_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Bag_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Bag_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Bag_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Chest_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Chest_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Chest_03_Part_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Chest_03_Part_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Container_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Container_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Stool_Quad_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Stool_Quad_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Box_Small_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Medieval_Armory/SM_Box_Small_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Arch_8m_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Arch_8m_05.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Arch_8m_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Arch_8m_06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Barrel_Ceiling_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Barrel_Ceiling_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ceiling_Flat_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Ceiling_Flat_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ceiling_Flat_Large_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Ceiling_Flat_Large_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Cupboard_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Cupboard_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Wall_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_Ruined_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Wall_Ruined_22.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_Thick_Ruined_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Wall_Thick_Ruined_06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Window_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Window_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Window_01_Ruined_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Window_01_Ruined_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Window_01_Ruined_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Window_01_Ruined_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Window_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Window_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Pointed_Roof_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Pointed_Roof_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Roof_12m_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Roof_12m_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Roof_12m_Ruined_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Roof_12m_Ruined_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Roof_12m_Ruined_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Roof_12m_Ruined_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Roof_12m_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Roof_12m_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Roof_12m_Ruined_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Roof_12m_Ruined_04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Ceiling_Intersection_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Ceiling_Intersection_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Brick_Column_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Brick_Column_05.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_Corner_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Wall_Corner_05.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_Corner_Half_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Wall_Corner_Half_04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Pilaster_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Pilaster_01b.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Trim_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Trim_04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Trim_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Trim_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Corbels_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Corbels_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Trim_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lordenfel/SM_Trim_07.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_14"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_14.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_16.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_13.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_05.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_07.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_08.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_09.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_10.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_12.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_15.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_19.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_20.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_21.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_22.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_23"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_23.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_24"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_24.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_25"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_25.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_26"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_26.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_27"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_27.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_29"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_29.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_30"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_30.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_31"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_31.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_32"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_32.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_33"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_33.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_34"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_34.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_fence_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_35.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_02_Leaf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_02_Leaf.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_branches_2_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_2_SM.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_DryTreeT3_branches_3_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_3_SM.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Banister_02b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Banister_02b.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Base_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Base_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Plank_4m_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Plank_4m_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_PlankRope_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_PlankRope_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_PlankThin_2m_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_PlankThin_2m_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Scafold_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Scafold_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Stairs_02a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Stairs_02a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_ThinWall_8x8_Broken_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_ThinWall_8x8_Broken_01b.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_TowerFloor_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_TowerFloor_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Trim_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Trim_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01b.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01c.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01d.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01f"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01f.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01g"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_8x8_Broken_01g.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wall_Broken_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wall_Broken_01b.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wheel_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ice_Palace/SM_Wheel_01a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_wall_short_brick_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_wall_short_brick/SM_wall_short_brick_1_a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_wall_short_brick_2_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_wall_short_brick/SM_wall_short_brick_2_a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_wall_short_brick_3_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_wall_short_brick/SM_wall_short_brick_3_a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_wall_short_brick_4_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_wall_short_brick/SM_wall_short_brick_4_a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_wall_short_brick_5_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_wall_short_brick/SM_wall_short_brick_5_a.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Fence_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Fence_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Fence_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Fence_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Fence_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Fence_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Scaffolding_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Scaffolding_06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Scaffolding_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Scaffolding_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Scaffolding_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Scaffolding_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Planks_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Planks_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wooden_Beam_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Wooden_Beam_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Plank_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Plank_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wooden_Support_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Wooden_Support_06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Wooden_Support_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Wooden_Support_07.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_SM_Scaffolding_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Scaffolding_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_brickFloor"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Debris/brickFloor.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_P_Archive_Chair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Archive_Chair01/P_Archive_Chair01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Left01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Left01/P_Fortress_BossDoor_Left01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Right01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Left01/P_Fortress_BossDoor_Right01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_Object_BillBoardObject"),
		CBillBoardObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region  인스턴싱용 환경 오브젝트

	lstrcpyW(m_szLoadingText, TEXT("인스턴싱용 환경 오브젝트"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_Grass0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Grass0/Grass0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_Tree0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Tree0/Tree0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_House0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_0/House0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_Component_Model_House3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_3/House3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region 트리거 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_HILL, TEXT("Prototype_GameObject_TriggerObject"),
		CTriggerObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


	/* 로딩이 완료되었습니다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader_Hill::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	return S_OK;
}

CLoader_Hill * CLoader_Hill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_Hill*	pInstance = new CLoader_Hill(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_Hill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Hill::Free()
{
	__super::Free();
}
