#include "pch.h"	 
#include "Loader_RoyalGarden.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"

#pragma region ���� 
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

#pragma region ����Ʈ ����
#include "Elite_Joker.h"
#include "HArmorLV2.h"

#include "Body_Joker.h"
#include "Body_HArmorLV2.h"

#include "Joker_Weapon.h"
#include "Weapon_GreatSword.h"

#pragma endregion 

#pragma region �Ϲ� ����
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

#pragma region �÷��̾� ���� 
#include "Player.h"
#include "LeftWeapon.h"
#include "RightWeapon.h"
#include "ClawWeapon.h"
#include "PlayerCamera.h"	
#ifdef _DEBUG
#include "Camera_Debug.h"
#endif // _DEBUG
#pragma endregion 

#pragma region ȯ���� 
#include "Sky.h"
#include "Terrain.h"
#include "Water.h"
#pragma endregion 

#pragma region Effect

#include "Effect_Mesh.h"
#include "Effect_Particle.h"

#pragma endregion

#pragma region �׽�Ʈ�� ��ٸ�
#include "Ladder.h"
#pragma endregion

#pragma region ������Ʈ
#include "StaticObject.h"		// (�ƴ� ��)�ν��Ͻ̿� ���ӿ�����Ʈ
#include "Emissive_StaticObject.h"		
#include "Glass_StaticObject.h"		
#include "BillBoardObject.h"		// (�ƴ� ��)�ν��Ͻ̿� ���ӿ�����Ʈ

#include "GroundObject.h"		// �ν��Ͻ̿� ���ӿ�����Ʈ
#include "Emissive_GroundObject.h"
#include "TriggerObject.h"		// Ʈ���ſ� ���ӿ�����Ʈ
#include "BlackScreen.h"
#pragma endregion

#include "GameItem.h"
#include "Button.h"

CLoader_RoyalGarden::CLoader_RoyalGarden(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

_uint APIENTRY Thread_Main_RoyalGarden(void* pArg)
{
	CLoader_RoyalGarden*		pLoader_RoyalGarden = static_cast<CLoader_RoyalGarden*>(pArg);

	pLoader_RoyalGarden->Start_Loading();

	return 0;
}

HRESULT CLoader_RoyalGarden::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_RoyalGarden::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if (FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_ROYALGARDEN: //Ȥ�ø𸣴� �ٽ��ѹ� �˻��ϴ� ����
		m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f);
		hr = Loading_For_Level_RoyalGarden();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

vector<string> GetFBXFileNames(const std::string& folderPath)
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

HRESULT CLoader_RoyalGarden::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	vector<string> strFBXNames;

	strFBXNames = __super::GetFBXFileNames(pFilePath);

	for (auto& FBXName : strFBXNames)
	{
		string strPrototypeName = "Prototype_Component_Model_" + FBXName;

		_tchar		szPrototypeName[MAX_PATH] = {};

		MultiByteToWideChar(CP_ACP, 0, strPrototypeName.c_str(), static_cast<_int>(strlen(strPrototypeName.c_str())), szPrototypeName, MAX_PATH);

		string strFBXPath = pFilePath + FBXName + ".fbx";


		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, szPrototypeName,
			CModel::Create(m_pDevice, m_pContext, strFBXPath.c_str(), CModel::MODEL_NONANIM, PreTransformMatrix))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader_RoyalGarden::Loading_For_Level_RoyalGarden()
{
	/* �����÷��̷����� ���� �ڿ��� �ε��մϴ�.  */
	lstrcpyW(m_szLoadingText, TEXT("�����÷��̷����� ���� �ڿ��� �ε� ���Դϴ�."));

	// �ʱ� ������ ���� Ʈ������ 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region ����
	lstrcpyW(m_szLoadingText, TEXT("�ؽ��� ������ �����Ѵ�."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_Nor_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/T_Terrain_ORM_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_TerrainMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/MaskData0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Gem_Mtrl"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/GemMtrl/Gem_%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Water"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/T_Water_%d.png"), 4))))
		return E_FAIL;

	//���� Height
	//lstrcpyW(m_szLoadingText, TEXT("�ͷ��� ������Ʈ ����"));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_VIBuffer_Terrain")
	//	, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/TerrainHeight11.txt")))))
	//	return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("�ͷ��� ������Ʈ ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 150, 150, 1, nullptr , TEXT("../Bin/DataFiles/HeightData/Royal_Garden/TerrainHeight22.txt")))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("�ͷ��� ������Ʈ ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_VIBuffer_Water")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 200, 200, 1, nullptr, TEXT("../Bin/DataFiles/HeightData/Royal_Garden/TerrainHeight21.txt")))))
		return E_FAIL;
	/*

//Prototype_Component_Shader_Water
//Prototype_Component_VIBuffer_Water*/
	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky ������"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 



#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("�׺���̼� ������ �����Ѵ�."));	

	// 03.19 �׺� Tutorial Map 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/NavigationFiles/Royal_Garden/TestNavigation45.txt")))))
		return E_FAIL;

	
		

#pragma endregion 





#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("������Ʈ��"));

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

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/Garden_Trees/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Load_BinaryModels("../Bin/Resources/Models/Objects/SM_Fences/", ObjectPreTransformMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_BillBoardObject"),
		CBillBoardObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveObject"),
		CEmissive_StaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GlassObject"),
		CGlass_StaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Prototype_GameObject_Object_EmissiveObject
	//Prototype_GameObject_Object_GlassObject
#pragma endregion 

#pragma region  �ν��Ͻ̿� ȯ�� ������Ʈ	

	lstrcpyW(m_szLoadingText, TEXT("�ν��Ͻ̿� ȯ�� ������Ʈ"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveGroundObject"),
		CEmissive_GroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region Ʈ���� ������Ʈ
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_TriggerObject"),
		CTriggerObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion



	/* �ε��� �Ϸ�Ǿ����ϴ� */
	lstrcpyW(m_szLoadingText, TEXT("�ε���."));	
	m_isFinished = true;
	return S_OK;
}

CLoader_RoyalGarden * CLoader_RoyalGarden::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_RoyalGarden*	pInstance = new CLoader_RoyalGarden(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_RoyalGarden");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_RoyalGarden::Free()
{
	__super::Free();
}
