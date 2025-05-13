#include "pch.h" 
#include "Loader_Ocean.h"
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
#include "Water.h"
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

CLoader_Ocean::CLoader_Ocean(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

_uint APIENTRY Thread_Main_Ocean(void* pArg)
{
	CLoader_Ocean*		pLoader_Ocean = static_cast<CLoader_Ocean*>(pArg);

	pLoader_Ocean->Start_Loading();

	return 0;
}

HRESULT CLoader_Ocean::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_Ocean::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_OCEAN: //혹시모르니 다시한번 검사하는 느낌
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);
		hr = Loading_For_Level_Ocean();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_Ocean::Loading_For_Level_Ocean()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_Nor_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_ORM_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/MaskData0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Water"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/T_Water_%d.png"), 4))))
		return E_FAIL;
	//이전 Height
	//lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/TerrainHeight11.txt")))))
	//	return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr , TEXT("../Bin/DataFiles/HeightData/Memory_Ocean/TerrainHeight20.txt")))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("물 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_VIBuffer_Water")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 200, 200, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/Memory_Ocean/TerrainHeight23.txt")))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

	/* For.Prototype_GameObject_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 




#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	

	// 03.19 네비 Tutorial Map 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/Memory_Ocean/TestNavigation13.txt")))))
		return E_FAIL;

	// 03.19 네비 Circus Map 
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Navigation"),
	//	CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/TestNavigation10.txt")))))
	//	return E_FAIL;
	



#pragma endregion 


#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));


	_matrix ObjectPreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Garden/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Biomass/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Cage/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Glass/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/GreenHouse/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Shelf/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Trees/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/SM_Fences/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Debris/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Clothes/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Ice_Palace/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/MemoryOcean/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Object_BillBoardObject"),
		CBillBoardObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region  인스턴싱용 환경 오브젝트

	lstrcpyW(m_szLoadingText, TEXT("인스턴싱용 환경 오브젝트"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Model_Grass0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Grass0/Grass0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Model_Tree0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Tree0/Tree0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Model_House0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_0/House0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_Component_Model_House3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House_3/House3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region 트리거 오브젝트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, TEXT("Prototype_GameObject_TriggerObject"),
		CTriggerObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

	/* 로딩이 완료되었습니다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader_Ocean::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	vector<string> strFBXNames;

	strFBXNames = __super::GetFBXFileNames(pFilePath);

	for (auto& FBXName : strFBXNames)
	{
		string strPrototypeName = "Prototype_Component_Model_" + FBXName;

		_tchar		szPrototypeName[MAX_PATH] = {};

		MultiByteToWideChar(CP_ACP, 0, strPrototypeName.c_str(), static_cast<_int>(strlen(strPrototypeName.c_str())), szPrototypeName, MAX_PATH);

		string strFBXPath = pFilePath + FBXName + ".fbx";


		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_OCEAN, szPrototypeName,
			CModel::Create(m_pDevice, m_pContext, strFBXPath.c_str(), CModel::MODEL_NONANIM, PreTransformMatrix))))
		{
			MSG_BOX("Failed To Load!");

			return E_FAIL;
		}
	}

	return S_OK;
}

CLoader_Ocean * CLoader_Ocean::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_Ocean*	pInstance = new CLoader_Ocean(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_Ocean");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Ocean::Free()
{
	__super::Free();
}
