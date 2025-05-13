#include "pch.h" 
#include "Loader_SeaOfTrees.h"
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
#include "LadderObject.h"
#include "BlackScreen.h"
#pragma endregion

#include "GameItem.h"
#include "Button.h"

CLoader_SeaOfTrees::CLoader_SeaOfTrees(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

_uint APIENTRY Thread_Main_SeaOfTrees(void* pArg)
{
	CLoader_SeaOfTrees*		pLoader_SeaOfTrees = static_cast<CLoader_SeaOfTrees*>(pArg);

	pLoader_SeaOfTrees->Start_Loading();

	return 0;
}

HRESULT CLoader_SeaOfTrees::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	vector<string> strFBXNames;

	strFBXNames = __super::GetFBXFileNames(pFilePath);

	for (auto& FBXName : strFBXNames)
	{
		string strPrototypeName = "Prototype_Component_Model_" + FBXName;

		_tchar		szPrototypeName[MAX_PATH] = {};

		MultiByteToWideChar(CP_ACP, 0, strPrototypeName.c_str(), static_cast<_int>(strlen(strPrototypeName.c_str())), szPrototypeName, MAX_PATH);

		string strFBXPath = pFilePath + FBXName + ".fbx";


		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, szPrototypeName,
			CModel::Create(m_pDevice, m_pContext, strFBXPath.c_str(), CModel::MODEL_NONANIM, PreTransformMatrix))))
			return E_FAIL;	
	}

	return S_OK;
}

HRESULT CLoader_SeaOfTrees::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_SeaOfTrees::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_SEAOFTREES: //혹시모르니 다시한번 검사하는 느낌
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);
		hr = Loading_For_Level_SeaOfTrees();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_SeaOfTrees::Loading_For_Level_SeaOfTrees()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_Nor_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_ORM_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/MaskData0.dds"), 1))))
		return E_FAIL;
	//이전 Height
	//lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/TerrainHeight11.txt")))))
	//	return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr , TEXT("../Bin/DataFiles/HeightData/TerrainHeight17.txt")))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 



#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	

	// 03.19 네비 Circus Map 
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Navigation"),
	//	CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/TestNavigation48.txt")))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/Sea_Tree/TestNavigation55.txt")))))
		return E_FAIL;
	



#pragma endregion 


#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));
	_matrix ObjectPreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_Planks"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Planks.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_Railing"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Railing.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_Tower"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Wood Railings/SM_Tower.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_chandelier04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/chandelier04/chandelier04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_CircusBalloon01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusBalloon01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_CircusBalloon02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusBalloon02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_CircusballoonDamage01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusballoonDamage01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_CircusballoonDamage02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_CircusballoonDamage02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_VariationBranch01_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch01_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_VariationBranch02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_VariationBranch03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBalloon/P_VariationBranch03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_HORSE_P_WoodenStand02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/HORSE_P_WoodenStand02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_BloodTent02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_BloodTent02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_BloodWoodChair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_BloodWoodChair01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Boss_Floor01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_Boss_Floor01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Boss_Floor02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_Boss_Floor02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_WoodenChair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/CircusBossRoom/P_WoodenChair01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_Cross"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Cross/Cross.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang01_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang01_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang01_03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang02_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Hang03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_Hang03_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_HempRope"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Hang/P_HempRope.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree01_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree01_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree01_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree01_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree02_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree02_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree02_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree03_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree03_01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_tree03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/tree03_02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_Greenhouse_Elevator01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Greenhouse_Elevator/Greenhouse_Elevator01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_WoodStair03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodStairs/SM_WoodStair03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_WoodFence04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence04/SM_WoodFence04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_WoodFence03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence03/SM_WoodFence03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth01.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth02.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth04.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth05.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth06.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth07.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_P_Cloth08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth08.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_fence_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_12.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_fence_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_19.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_Component_Model_SM_WoodStairs03."),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Circus_08/SM_WoodStairs03.fbx", CModel::MODEL_NONANIM, ObjectPreTransformMatrix))))
		return E_FAIL;


	if(FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Circus_08/", ObjectPreTransformMatrix)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_BillBoardObject"),
		CBillBoardObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_Ladder"),
		CLadderObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region  인스턴싱용 환경 오브젝트

	lstrcpyW(m_szLoadingText, TEXT("인스턴싱용 환경 오브젝트"));
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region 트리거 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_TriggerObject"),
		CTriggerObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion


	/* 로딩이 완료되었습니다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader_SeaOfTrees * CLoader_SeaOfTrees::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_SeaOfTrees*	pInstance = new CLoader_SeaOfTrees(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_SeaOfTrees");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_SeaOfTrees::Free()
{
	__super::Free();
}
