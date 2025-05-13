#include "pch.h" 
#include "Loader_Static_Logo.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"

#pragma region 보스 
#include "Boss_Varg.h"
#include "Boss_Magician.h"
#include "Boss_Magician2.h"
#include "Boss_Urd.h"
#include "Boss_Bat.h"

#include "Body_Varg.h"
#include "Body_Magician.h"
#include "Body_Magician2.h"
#include "Body_Urd.h"
#include "Body_Bat.h"
#include "Decorative_Spike.h"

#include "VargKnife.h"
#include "Boss_Varg_Camera.h"
#include "Boss_Magician_Camera.h"	
#include "Boss_Mutation_Magician_Camera.h"
#include "Boss_Urd_Camera.h"	
#include "Boss_Bat_Camera.h"

#include "Weapon_Cane.h"
#include "Weapon_Magician_Sword.h"
#include "Weapon_Magician2_Sword.h"
#include "Weapon_Urd_Sword.h"
#include "Stand_Stack_Sword.h"
#include "Bat_Spike.h"

#include "Projectile_Card.h"
#include "Projectile_Intro_Card.h"
#include "Projectile_Air.h"
#include "Decorative_Tonic.h"
#include "Decorative_Mutation.h"
#include "Decorative_Mutation2.h"

#include "UI_Boss_HP_Bar.h"
#include "UI_Boss_HP_Bar_Gage.h"
#include "UI_Boss_HP_Bar_Gage_Effect.h"
#include "UI_Boss_HP_Phase.h"
#pragma endregion 

#pragma region 엘리트 몬스터
#include "Elite_Joker.h"
#include "HArmorLV2.h"
#include "Elite_Punch_Man.h"
#include "Elite_Grace.h"
#include "Elite_Researcher.h"

#include "Body_Joker.h"
#include "Body_HArmorLV2.h"
#include "Body_Punch_Man.h"
#include "Body_Grace.h"
#include "Body_Researcher.h"
#include "Decorative_Researcher.h"

#include "Joker_Weapon.h"
#include "Weapon_GreatSword.h"
#include "Weapon_Book.h"
#include "Projectile_Dagger.h"
#include "Projectile_FireBall.h"
#pragma endregion 

#pragma region 일반 몬스터
#include "Normal_VillageM0.h"
#include "Normal_VillageM1.h"
#include "Normal_VillageF0.h"
#include "Normal_VillageF1.h"
#include "Normal_ScytheM.h"
#include "Circus_Balloon.h"

#include "Body_VillageM0.h"
#include "Body_VillageM1.h"
#include "Body_VillageF0.h"
#include "Body_VillageF1.h"
#include "Body_ScytheM.h"
#include "Decorative_Hat.h"

#include "Weapon_Axe.h"
#include "Weapon_Dagger.h"
#include "Weapon_Shield.h"
#include "Weapon_Monster_Scythe.h"

#include "Monster_HP_Bar.h"
#include "Monster_HP_Gage.h"
#include "Monster_HP_Gage_Effect.h"
#include "Monster_HP_Bar_Effect.h"
#include "Locked_On.h"
#pragma endregion 

#pragma region NPC

#include "Aisemy.h"
#include "Body_Aisemy.h"

#include "GhostAisemy.h"
#include "Body_GhostSemy.h"
#include "Lamp.h"


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
#include "Weapon_Halberd.h"	
#include "Weapon_Scythe.h"	
#include "Player_Weapon_Axe.h"
#include "Player_Weapon_Cane.h"
#include "Player_Weapon_Cane_Sword.h"
#include "Player_Weapon_GreadSword.h"
#include "Player_Weapon_JavelinSword.h"
#pragma endregion 

#pragma region 환경요소 
#include "Sky.h"
#include "Terrain.h"
#pragma endregion 

#pragma region Effect

#include "Effect_Mesh.h"
#include "Effect_Sword.h"
#include "Effect_Particle.h"
#include "Effect_Instancing.h"

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
#include "UIGroup_Landing.h"
#include "UIGroup_MapChange.h"
#include "UIGroup_Skill.h"
#include "UIGroup_Dialogue.h"

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


//#include "UI_EmptyItemBox.h"
#include "UI_ItemBackground.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_Item_Icon.h"

#include "UI_Feather_Icon.h"
#include "UI_HPBar1_Border.h"
#include "UI_HPBar2_BG.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_HPBar5_Track.h"
#include "UI_MemoryShardsIcon.h"
#include "UI_MPBar1_Border.h"
#include "UI_MPBar2_BG.h"
#include "UI_MPBar3_MainBar.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_FixSlotFrame.h"
#include "UI_Potion_DefaultType.h"
#include "UI_DialogBackground.h"

#include "UI_GameLogoImage.h"
#include "UI_LoadingScreen.h"
#include "UI_LoadingIcon.h"
#include "UI_LootNotifyBackground.h"
#include "UI_Talent_Icon.h"
#include "UI_LandingScreen.h"

#include "UI_MapEntryFrame.h"
#include "UI_Map_Save_Image.h"
#include "UI_MediaFrame.h"

#include "UI_Skill_Slot.h"

#include "UI_DialogueTalkBackground.h"
#include "UI_DialogueWindowBackground.h"
#include "UI_NextLineHint.h"

#include "UI_Bar_Poison.h"
#include "UI_Frame_Poison.h"
#include "UI_QuestBackground.h"

#include "UI_EndingScene.h"
#include "UI_YellowScreen.h"
#pragma endregion

#pragma region 오브젝트
#include "StaticObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "BillBoardObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "GroundObject.h"		// 인스턴싱용 게임오브젝트
#include "TriggerObject.h"		// 트리거용 게임오브젝트
#include "BlackScreen.h"
#include "SpecificObject.h"		//	맵 어디서든 다 쓰일 오브젝트 얘가 진짜 사다리 의자 이런거 관리함 ㅋ

#include "Chair.h"
#include "ChairLamp.h"
#include "Elevator_Door.h"
#include "LadderObject.h"
#include "DoorObject.h"
#include "DoorManager.h"

#include "DestructObject.h"
#include "BarrierScreen.h"

#pragma endregion

#pragma region 상호작용 오브젝트 
#include "GameItem.h"			// 인게임 속,(드랍아이템) 게임오브젝트 
#include "Button.h"				// 상호작용할 때 나올 UI 오브젝트(아이템 / 의자 / 사다리 etc..)
#include "DeadBranch.h"			// 플레이어 사망 후, 해당 사망한 자리에서 나올 게임오브젝트.

#include "LockLine.h"
#pragma endregion

#pragma region 로고 카메라
#include "Camera_Logo.h"
#pragma endregion

CLoader_Static_Logo::CLoader_Static_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLoader{ pDevice, pContext }
{
}

HRESULT CLoader_Static_Logo::Initialize(LEVELID eNextLevelID)
{
	__super::Initialize(eNextLevelID);

	return S_OK;
}

HRESULT CLoader_Static_Logo::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	if(FAILED(CoInitializeEx(nullptr, 0)))
		return E_FAIL;

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_STATIC:
		hr = Loading_For_Level_Static();
		break;
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader_Static_Logo::Loading_For_Level_Static()
{
	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	
#pragma region 카메라 
	/* For.Prototype_GameObject_Camera_Free */

	lstrcpyW(m_szLoadingText, TEXT("카메라 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Free"),	
		CCamera_Free::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Debug"),	
		CCamera_Debug::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#endif // _DEBUG
#pragma endregion	



#pragma region 티메시아 캐릭터 
	lstrcpyW(m_szLoadingText, TEXT("티메시아 모델을 생성한다."));
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corner"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus/Corvus.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* 캐릭터 ORM 머리 텍스처  */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Head"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Corvus/T_C_CorvusHead01_ORM.dds"), 1))))
		return E_FAIL;

	/* 캐릭터 ORM 몸통 텍스처  */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Body"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Corvus/T_C_CorvusBody01_ORM.dds"), 1))))
		return E_FAIL;

	/* 캐릭터 ORM 다리 텍스처  */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Leg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Corvus/T_C_CorvusLegs01_ORM.dds"), 1))))
		return E_FAIL;

	/* 캐릭터 ORM 망토 텍스처  */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_ORM_Texture_Corvus_Clock"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Corvus/T_C_CorvusCloak02_ORM.dds"), 1))))
		return E_FAIL;



#pragma endregion 

#pragma region 티메시아 캐릭터 오른손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 오른손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Right_Weapon/Corvus_Right_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Right_Weapon"),
		CRightWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 


#pragma region 티메시아 캐릭터 왼손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 왼손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Left_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Left_Weapon/Corvus_Left_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Left_Weapon"),
		CLeftWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 티메시아 손톱 갈퀴 공격 
	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Right_Claw"),
		CClawWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 티메시아 할버드 무기 
	PreTransformMatrix = XMMatrixIdentity();

	lstrcpyW(m_szLoadingText, TEXT("주인공 할버드 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Halberd"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Halberd/Halberd.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Halberd"),
		CWeapon_Halberd::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 


#pragma region 티메시아 낫 무기	
	PreTransformMatrix = XMMatrixIdentity();

	lstrcpyW(m_szLoadingText, TEXT("주인공 낫 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Scythe"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Scythe/Scythe.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Scythe"),
		CWeapon_Scythe::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 티메시아 도끼 무기	
	PreTransformMatrix = XMMatrixIdentity();

	lstrcpyW(m_szLoadingText, TEXT("주인공 도끼 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Axe"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Axe/Weapon_Axe.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Axe"),
		CPlayer_Weapon_Axe::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 플레이어 지팡이 무기 
	lstrcpyW(m_szLoadingText, TEXT("지팡이 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Cane"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Cane/Weapon_Cane.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Cane"),
		CPlayer_Weapon_Cane::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 


#pragma region 플레이어 지팡이 검 무기	
	lstrcpyW(m_szLoadingText, TEXT("지팡이 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Cane_Sword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Cane_Sword/Weapon_Cane_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Cane_Sword"),
		CPlayer_Weapon_Cane_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 플레이어 대검 무기	
	lstrcpyW(m_szLoadingText, TEXT("대검 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_GreadSword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_GreadSword/Weapon_GreadSword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_GreadSword"),
		CPlayer_Weapon_GreadSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 플레이어 자벨린 무기		
	lstrcpyW(m_szLoadingText, TEXT("자벨린 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Javelin_Sword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Javelin_Sword/Weapon_Javelin_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Weapon_Scythe */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Javelin_Sword"),
		CPlayer_Weapon_JavelinSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 



#pragma region 티메시아 카메라 
	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_PlayerCamera"),
		CPlayerCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 티메시아 보스

	lstrcpyW(m_szLoadingText, TEXT("보스 모델을 생성한다."));

	//오두르
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Boss_Magician.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;
	
	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Sword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Sword/Magician_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Cane"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Cane/Magician_Cane.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Projectile_Card"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Card/Projectile_Card.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Exodia_Card"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Card/Exodia.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Body"),
		CBody_Magician::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Sword"),
		CWeapon_Magician_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Cane"),
		CWeapon_Cane::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Card"),
		CProjectile_Card::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Intro_Card"),
		CProjectile_Intro_Card::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician"),
		CBoss_Magician::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//변이 오두르

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician2_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician2/Boss_Magician2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Tonic"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician2/Tonic/Tonic.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2_Body"),
		CBody_Magician2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Tonic"),
		CDecorative_Tonic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2_Sword"),
		CWeapon_Magician2_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2"),
		CBoss_Magician2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//바그

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Varg_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Boss_Varg.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg_Body"),
		CBody_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Varg_Knife"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Knife/VargKnife.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg_Knife"),
		CVargKnife::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg"),
		CBoss_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//우르드

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Urd_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Urd/Boss_Urd.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Urd_Sword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Urd/Urd_Sword/NoAnim/Urd_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Body"),
		CBody_Urd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Sword"),
		CWeapon_Urd_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Stack_Sword"),
		CStand_Stack_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd"),
		CBoss_Urd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//매달린 여왕
	//Decorative_Spikes
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Bat_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Bat/Boss_Bat.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Spike"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Bat/Bat_Spike/Bat_Spike1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat_Body"),
		CBody_Bat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Spike"),
		CDecorative_Spike::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat"),
		CBoss_Bat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Air"),
		CProjectile_Air::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Bat_Spike"),
		CBat_Spike::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//보스 바그 카메라 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg_Camera"),	
		CBoss_Varg_Camera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


	//보스 매지션1 (오두르)  카메라 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Camera"),
		CBoss_Magician_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//보스 변종 오두르 LV2   카메라 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Mutation_Magician_Camera"),
		CBoss_Mutation_Magician_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//보스 Urd   카메라 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Camera"),	
		CBoss_Urd_Camera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

	//보스 Bat  카메라	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat_Camera"),
		CBoss_Bat_Camera::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//보스 HP바

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Boss_HP/BossHP%d.png"), 5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"),
		CUI_Boss_HP_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage"),
		CUI_Boss_HP_Bar_Gage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage_Effect"),
		CUI_Boss_HP_Bar_Gage_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Phase"),
		CUI_Boss_HP_Phase::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region 엘리트 몬스터

	lstrcpyW(m_szLoadingText, TEXT("엘리트 몬스터 모델을 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Joker_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Joker/Elite_Joker.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Joker_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Joker/Weapon/Joker_Weapon.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker_Body"),
		CBody_Joker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker_Weapon"),
		CJoker_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker"),
		CElite_Joker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_HArmorLV2_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/HArmorLV2/HArmorLV2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_GreatSword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/GreatSword/GreatSword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2_Body"),
		CBody_HArmorLV2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_GreatSword"),
		CWeapon_GreatSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2"),
		CHArmorLV2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Punch_Man_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Punch_Man/Elite_Punch_Man.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man_Body"),
		CBody_Punch_Man::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man"),
		CElite_Punch_Man::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Grace_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Grace/Elite_Grace.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Projectile_Dagger"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Projectile/Dagger/Projectile_Dagger.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace_Body"),
		CBody_Grace::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace"),
		CElite_Grace::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Dagger"),
		CProjectile_Dagger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Elite_Researcher_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Researcher/Elite_Researcher.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Researcher"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Researcher/Decorative/Decorative_Researcher.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Book"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Researcher/Weapon_Book.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Researcher_Body"),
		CBody_Researcher::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Researcher"),
		CElite_Researcher::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Book"),
		CWeapon_Book::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_FireBall"),
		CProjectile_FireBall::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Researcher"),
		CDecorative_Researcher::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region 일반 몬스터
	lstrcpyW(m_szLoadingText, TEXT("일반 몬스터 모델을 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_VillageM0_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageM0/Normal_VillageM0.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_VillageM1_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageM1/Normal_VillageM1.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_VillageF0_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageF0/Normal_VillageF0.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_VillageF1_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageF1/Normal_VillageF1.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Mutation"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageF1/Mutation/Decorative_Mutation.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Mutation2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageM1/Mutation/Decorative_Mutation2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Normal_ScytheM_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_ScytheM/Normal_ScytheM.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Decorative_Hat"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_ScytheM/Hat/ScytheM_Hat.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Axe"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Axe/HandAxe.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Dagger"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Dagger/Dagger.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Shield"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Shield/Shield.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Monster_Scythe"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Scythe/Monster_Scythe.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	//몬스터 Body

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0_Body"),
		CBody_VillageM0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1_Body"),
		CBody_VillageM1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0_Body"),
		CBody_VillageF0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1_Body"),
		CBody_VillageF1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM_Body"),
		CBody_ScytheM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Hat"),
		CDecorative_Hat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Mutation"),
		CDecorative_Mutation::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Mutation2"),
		CDecorative_Mutation2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//몬스터 Weapon

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Axe"),
		CWeapon_Axe::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Dagger"),
		CWeapon_Dagger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Shield"),
		CWeapon_Shield::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Monster_Scythe"),
		CWeapon_Monster_Scythe::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//몬스터 객체

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0"),
		CNormal_VillageM0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1"),
		CNormal_VillageM1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0"),
		CNormal_VillageF0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"),
		CNormal_VillageF1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM"),
		CNormal_ScytheM::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//몬스터 HP바
	/* For.Prototype_Component_Texture_Monster_HP*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Monster_HP/MonsterHP%d.dds"), 6))))
		return E_FAIL;
	/* For.Prototype_GameObject_Monster_HP_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Bar"),
		CMonster_HP_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Gage"),
		CMonster_HP_Gage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Gage_Effect"),
		CMonster_HP_Gage_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Bar_Effect"),
		CMonster_HP_Bar_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"),
		CLocked_On::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//몬스터용 노이즈 텍스쳐 /* For.Prototype_Component_Texture_Effect_Mesh_Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Mesh_Noise/texNoise%d.dds"), 19))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Emissive"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Boss/Boss_Bat/Bat_Spike/T_P_BatSpikes01_ORME.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Locked_On"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/LockOn/LockOn%d.dds"), 2))))
		return E_FAIL;
#pragma endregion 

#pragma region 건물 몬스터
	lstrcpyW(m_szLoadingText, TEXT("건물 몬스터 모델을 생성한다."));
	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Building_Circus_Balloon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Building/Circus_Balloon/Circus_Balloon.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Building_Circus_Balloon2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Building/Circus_Balloon/Circus_Balloon2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Building_Circus_Balloon"),
		CCircus_Balloon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region NPC / 고세미
	lstrcpyW(m_szLoadingText, TEXT("NPC 모델을 생성한다."));
	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPC_Aisemy_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/NPC/NPC_Aisemy.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy"),
		CAisemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy_Body"),
		CBody_Aisemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("귀신 버젼 NPC 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Model_GhoSemy_Body"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GhoSemy/ghostsemy2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_GhostSemy"), CGhostAisemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_GhoSemy_Body"), CBody_GhostSemy::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GhostSemyNoise"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/GhostSemy/T_TilingNoise16.png"), 1))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("고세미 램프 생성한다."));
	/* For.Prototype_GameObject_Lamp */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Lamp"), CLamp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//_matrix PreLampTransformMatrix = XMMatrixIdentity();
	////PreLampTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.f)) * XMMatrixRotationX(XMConvertToRadians(75.f)) * XMMatrixRotationY(XMConvertToRadians(155.f));
	//PreLampTransformMatrix *= XMMatrixRotationX(XMConvertToRadians(90.f));

	/* For.Prototype_Component_Model_Lamp */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Lamp"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Item/Lamp/NPCLamp.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
#pragma endregion


#pragma region BlackScreen
	lstrcpyW(m_szLoadingText, TEXT("Fade 용 Object 생성한다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Black"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/BlackScreen/Fade2.png"), 1))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), CBlackScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_FadeTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxFadeTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
#pragma endregion

#pragma region ITEM / SOULBRANCH
	lstrcpyW(m_szLoadingText, TEXT("ITEM 생성한다."));

	/* For.Prototype_GameObject_GameItem */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), CGameItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemSoul */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemSoul"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Item/Item02/T_Flare_04.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ItemSoulNoise */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ItemSoulNoise"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Item/Item01/T_Noise_03.png"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxItem */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxItem"), CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxItemTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_GameObject_DeadBranch */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DeadBranch"), CDeadBranch::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Model_DeadBranch */
	_matrix PreTransformBranchMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DeadBranch"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Branch/Branch.fbx", CModel::MODEL_NONANIM, PreTransformBranchMatrix))))
		return E_FAIL;

	///* For.Prototype_Component_Shader_VtxMesh */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"), CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
	//	return E_FAIL;

	/* For.Prototype_Component_Texture_BranchDissolve */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BranchDissolve"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Item/SoulBranch/T_Disolve_Swirling.png"), 1))))
		return E_FAIL;
#pragma endregion




#pragma region 셰이더

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));


	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))	
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstanceMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstanceMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstanceMesh.hlsl"), VTX_MODEL_INSTANCE::Elements, VTX_MODEL_INSTANCE::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shader_VtxMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMeshNoDefferd"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshNoDefferd.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMeshNoDefferd"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMeshNoDefferd.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))	
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexInstance.hlsl"), VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements))))
		return E_FAIL;



	/* For.Prototype_Component_Shader_VtxPosTex_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_UI.hlsl"), VTXPOSTEX_UI::Elements, VTXPOSTEX_UI::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Fog"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Fog.hlsl"), VTXNULL::Elements, VTXNULL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Water"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

#pragma endregion 



#pragma region UI 텍스쳐
	lstrcpyW(m_szLoadingText, TEXT("UI 생성한다."));

	//====================================================================================================================================== 게임 인트로
	/* For.Prototype_Component_Texture_UI_GameLogoImage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_GameLogoImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_GameLogoImage.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_GameLogoImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_GameLogoImage"),
		CUI_GameLogoImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ButtonHighlight*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ButtonHighlight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ButtonHighlight_0%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_ButtonHighlight"),
		CUI_ButtonHighlight::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//====================================================================================================================================== 이펙트
	/* For.Prototype_Component_Texture_UI_Effect_Frame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Effect_Frame_%d.dds"), 3))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Effect_Frame"),
		CUI_Effect_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Effect_SlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_SlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Effect_SlotFrame_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_SlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Effect_SlotFrame"),
		CUI_Effect_SlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Effect_SlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SkillIcon_%d.dds"), 22))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Effect_SlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SkillIcon"),
		CUI_SkillIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 키보드 텍스처
	/* For.Prototype_Component_Texture_UI_KeyBox_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_KeyBox_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_KeyBox_Long.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_KeyBox_Long"),
		CUI_KeyBox_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 텍스트 박스

	/* For.Prototype_GameObject_UI_TextBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_TextBox"),
		CUI_TextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 공용 꾸밈 텍스처
	/* For.Prototype_Component_Texture_UI_DecorationLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DecorationLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DecorationLine_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DecorationLine"),
		CUI_DecorationLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_UI_YellowScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_YellowScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_YellowScreen.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_YellowScreen"),
		CUI_YellowScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 넛지 텍스처
	/* For.Prototype_Component_Texture_UI_DiamondIcon_01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DiamondIcon_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DiamondIcon_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DiamondIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DiamondIcon"),
		CUI_DiamondIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Indicator_Selected*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Indicator_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Indicator_Selected_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Indicator_Selected */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Indicator_Selected"),
		CUI_Indicator_Selected::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LockedIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LockedIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LockedIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LockedIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LockedIcon"),
		CUI_LockedIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_NewHint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_NewHint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_NewHint.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_NewHint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_NewHint"),
		CUI_NewHint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 메뉴창(의자착석)
		/* For.Prototype_Component_Texture_UI_HighlightBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HighlightBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HighlightBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HighlightBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_HighlightBar"),
		CUI_HightlightBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LeftBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LeftBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LeftBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LeftBackground"),
		CUI_LeftBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_RightBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_RightBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_RightBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_RightBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_RightBackground"),
		CUI_RightBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LevelImage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LevelImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LevelImage_%d.dds"), 5))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LevelImage"),
		CUI_LevelImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 특성 창

	/* For.Prototype_Component_Texture_UI_Arrow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Arrow.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arrow"),
		CUI_Arrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Attribute_Slot_Active*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Attribute_Slot_Active"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Attribute_Slot_Active_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Attribute_Slot_Active */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Attribute_Slot_Active"),
		CUI_Attribute_Slot_Active::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BaseTalentIndicator*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BaseTalentIndicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_BaseTalentIndicator.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BaseTalentIndicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_BaseTalentIndicator"),
		CUI_BaseTalentIndicator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ContentBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ContentBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ContentBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ContentBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_ContentBackground"),
		CUI_ContentBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DescBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DescBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DescBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DescBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DescBackground"),
		CUI_DescBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Frame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Frame_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Frame"),
		CUI_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneAboveBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SceneAboveBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneAboveBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneAboveBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SceneAboveBackground"),
		CUI_SceneAboveBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SceneBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneBackground_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SceneBackground"),
		CUI_SceneBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneUnderBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SceneUnderBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneUnderBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneUnderBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SceneUnderBackground"),
		CUI_SceneUnderBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_UnderLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_UnderLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_UnderLine.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_UnderLine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_UnderLine"),
		CUI_UnderLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 레벨 업 창

		/* For.Prototype_Component_Texture_UI_Arrow_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Arrow_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Arrow_Long.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Arrow_Long"),
		CUI_Arrow_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BlackBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BlackBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_BlackBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BlackBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_BlackBar"),
		CUI_BlackBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DecorationFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DecorationFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DecorationFrame_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DecorationFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DecorationFrame"),
		CUI_DecorationFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SkillLeftBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ButtonBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ButtonBackground_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SkillLeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_ButtonBackground"),
		CUI_ButtonBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_TitleBackgroundBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_TitleBackgroundBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_TitleBackgroundBar"),
		CUI_TitleBackgroundBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//==================================================================================================================================== 플레이어 기본 화면

	/* For.Prototype_Component_Texture_UI_Feather_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Feather_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Feather_Icon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Feather_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Feather_Icon"),
		CUI_Feather_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_HPBar1_Border"),
		CUI_HPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_HPBar2_BG"),
		CUI_HPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar3_MainBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_HPBar3_MainBar"),
		CUI_HPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
		/* For.Prototype_Component_Texture_UI_HPBar5_Track*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_HPBar5_Track"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar5_Track.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar5_Track */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_HPBar5_Track"),
		CUI_HPBar5_Track::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MemoryShardsIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MemoryShardsIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MemoryShardsIcon"),
		CUI_MemoryShardsIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MPBar1_Border"),
		CUI_MPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar2_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar2_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MPBar2_BG"),
		CUI_MPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar3_MainBar.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar4_Delay*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MPBar4_Delay"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar4_Delay.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_MPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MPBar3_MainBar"),
		CUI_MPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_PlunderSlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlunderSlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_PlunderSlotFrame_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_PlunderSlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_PlunderSlotFrame"),
		CUI_PlunderSlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_FixSlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_FixSlotFrame"),
		CUI_FixSlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Potion_DefaultType*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Potion_DefaultType"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Potion_DefaultType.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Potion_DefaultType */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Potion_DefaultType"),
		CUI_Potion_DefaultType::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LandingScreen*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LandingScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Message/UI_LandingScreen_%d.dds"), 11))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LandingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LandingScreen"),
		CUI_LandingScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 인벤토리

	/* For.Prototype_Component_Texture_UI_ItemBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ItemBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_ItemBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ItemBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_ItemBackground"),
		CUI_ItemBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SquareFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SquareFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_SquareFrame.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SquareFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SquareFrame"),
		CUI_SquareFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SquareFrame_Hover*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SquareFrame_Hover"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_SquareFrame_Hover.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SquareFrame_Hover */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_SquareFrame_Hover"),
		CUI_SquareFrame_Hover::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Item_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Item_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_Item_Icon_%d.dds"), 25	))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Item_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Item_Icon"),
		CUI_Item_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DialogBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DialogBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Inventory/UI_DialogBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DialogBackground"),
		CUI_DialogBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================로딩창
	/* For.Prototype_Component_Texture_UI_LoadingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 7))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LoadingScreen */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LoadingScreen"),
		CUI_LoadingScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LoadingIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LoadingIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/UI_LoadingIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LoadingIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LoadingIcon"),
		CUI_LoadingIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_LootNotifyBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LootNotifyBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LootNotifyBackground_0%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LootNotifyBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_LootNotifyBackground"),
		CUI_LootNotifyBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	
	/* For.Prototype_Component_Texture_UI_Talent_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Talent_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/TalentIcon/UI_Talent_Icon_%d.dds"), 39))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Talent_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Talent_Icon"),
		CUI_Talent_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//====================================================================================================================================== 맵?

	/* For.Prototype_Component_Texture_UI_MapEntryFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapEntryFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MapEntryFrame_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MapEntryFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MapEntryFrame"),
		CUI_MapEntryFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Map_Save_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Save_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MapTabImage_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MapEntryFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Map_Save_Image"),
		CUI_Map_Save_Image::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MediaFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MediaFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/MapChange/UI_MediaFrame.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MediaFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MediaFrame"),
		CUI_MediaFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 스킬
		/* For.Prototype_GameObject_UI_Skill_Slot */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Skill_Slot"),
		CUI_Skill_Slot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================== 아이세미 대화


	/* For.Prototype_Component_Texture_UI_DialogueTalkBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DialogueTalkBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_DialogueTalkBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogueTalkBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DialogueTalkBackground"),
		CUI_DialogueTalkBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_DialogueWindowBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DialogueWindowBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_DialogueWindowBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DialogueWindowBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_DialogueWindowBackground"),
		CUI_DialogueWindowBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_Texture_UI_NextLineHint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_NextLineHint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Dialogue/UI_NextLineHint_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_NextLineHint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_NextLineHint"),
		CUI_NextLineHint::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================
/* For.Prototype_Component_Texture_UI_CloudNoise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CloudNoise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Noise/UI_CloudNoise.dds"), 1))))
		return E_FAIL;
	//====================================================================================================================================== 독 퀘스트 바



/* For.Prototype_Component_Texture_UI_QuestBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_QuestBackground.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bar_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Bar_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Frame_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Frame_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Frame_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Icon_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Icon_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Icon_Poison.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Bar_Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Bar_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Bar_Noise.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BarGlow_Poison*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BarGlow_Poison"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_BarGlow_Poison.dds"), 1))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_QuestBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_QuestBackground"),
		CUI_QuestBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Bar_Poison */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Bar_Poison"),
		CUI_Bar_Poison::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Frame_Poison */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Frame_Poison"),
		CUI_Frame_Poison::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================


	/* For.Prototype_Component_Texture_UI_EndingScene*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_EndingScene"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Ending/UI_EndingScene_0%d.dds"), 5))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_EndingScene */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_EndingScene"),
		CUI_EndingScene::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//====================================================================================================================================
	/* For.Prototype_Component_Texture_MouseCursor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MouseCursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/UI_GameCursor.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MouseCursor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MouseCursor"),
		CUI_MouseCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_GameIntro"),
		CUIGroup_GameIntro::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_TestScene_TextTest */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"),
		CUIGroup_PlayerMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"),
		CUIGroup_PlayerLevelUP::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"),
		CUIGroup_PlayerTalent::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"),
		CUIGroup_PlayerScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UIGroup_Iventory */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"),
		CUIGroup_Inventory::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIGroup_Loading */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Loading"),
		CUIGroup_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UIGroup_Landing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Landing"),
		CUIGroup_Landing::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UIGroup_Landing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_MapChange"),
		CUIGroup_MapChange::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_UIGroup_Skill */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Skill"),
		CUIGroup_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UIGroup_Dialogue */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Dialogue"),
		CUIGroup_Dialogue::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
#pragma region UI 텍스쳐
	lstrcpyW(m_szLoadingText, TEXT("UI 생성한다."));

	//==================================================================================================================================== 월드 상호 작용 ui

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InteractionButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_InteractableIndicator.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_InteractionButton"),
		CButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================
#pragma endregion


#pragma region 플레이어 락온 선 
	lstrcpyW(m_szLoadingText, TEXT("플레이어 락온 선"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Models/Line/LockOn0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_LockLine"), CLockLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixScaling(0.00015f, 0.00015f, 0.00015f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_LockLine"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Line/Effect_Mesh_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
#pragma endregion 


#pragma region Effect_Mesh
	lstrcpyW(m_szLoadingText, TEXT("Effect_Mesh 생성중"));

	_matrix PreTransformMatrix_Effect = XMMatrixIdentity(); //Effect 전용 Matrix
	PreTransformMatrix_Effect = XMMatrixScaling(0.005f, 0.005f, 0.005f);

	/* For.Prototype_Component_Shader_Effect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect_Mesh_Diffuse*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Mesh_Diffuse/texDiffuse%d.dds"), 52))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect_Mesh_Noise*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Mesh_Noise/texNoise%d.dds"), 40))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Effect_Mesh_Mask*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Mesh_Mask/texMask%d.dds"), 58))))
		return E_FAIL;



	/* For.Prototype_Component_Model_Effect_Donut*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Donut"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Donut.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_ThinDonut*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_ThinDonut"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_ThinDonut.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Rainbow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Rainbow"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Rainbow.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Claw*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Claw"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Claw.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Sword*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Sword"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Sword.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Twist*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Twist"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Twist.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Strange*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Strange"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Strange.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Tornado*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Tornado"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Tornado.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Cross*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Cross"), //9
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Cross.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Ring*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Ring"), //10
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Ring.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Circle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Circle"), //11
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Circle.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Quad*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Quad"), //12
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Quad.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_HealingCurve*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_HealingCurve"), //13
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_HealingCurve.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Rock */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Rock"), //14
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Rock.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Sphere"), //17 <- Tool 에서의 번호라고 보면됨
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Sphere.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_HemiShphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_HemiShphere"), //18
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_HemiShphere.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Cylinder */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Cylinder"), //19
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_MeshCylinder.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_1"), //20
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_2"), //21
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_3 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_3"), //22
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_4 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_4"), //23
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_4.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Cone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Cone"), //24
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Cone.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_5 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_5"), //25
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_5.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Curve_6 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Curve_6"), //26
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Curve_6.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Urd_Cone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Urd_Cone"), //27
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Urd_Cone.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	/* For.Prototype_Component_Model_Effect_Urd_Cone */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Effect_Urd_Cylinder"), //28
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Effect_Mesh_Urd_Cyliner.fbx", CModel::MODEL_NONANIM, PreTransformMatrix_Effect))))
		return E_FAIL;

	///* For.Prototype_GameObject_Effect_Mesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		CEffect_Mesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion

#pragma region Effect_Particle
	lstrcpyW(m_szLoadingText, TEXT("Effect_Particle 생성중"));

	//Particle Compute Shader 생성 시작

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Drop */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Drop", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Explostion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Explosion"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Explosion", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Spark */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Spark", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Holding */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Holding", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Blood*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Blood", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Dust*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Dust", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Dust_Delay", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Hurricane", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Scythe */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Scythe"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Scythe", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_World */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Holding_World", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Burst */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Burst"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Burst", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_World_Continue", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Round */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Round"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Round", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Falling_World */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Falling_World"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Falling_World", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Spark_Update */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark_Update"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Spark_Update", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue_Gas */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue_Gas"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_World_Continue_Gas", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_World_Blood */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World_Blood"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_World_Blood", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Round_Hurricane */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Round_Hurricane"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Round_Hurricane", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Falling_World_Axis */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Falling_World_Axis"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Falling_World_Axis", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Continue */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Continue"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Dust_Continue", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Round_Hurricane_Version2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Round_Hurricane_Version2"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Round_Hurricane_Version2", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Holding_Version2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding_Version2"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Holding_Version2", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Drop_Version2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop_Version2"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Drop_Version2", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	//Particle Compute Shader 생성 끝

	/* For.Prototype_Component_VIBuffer_Point_Compute */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point_Compute"),
		CVIBuffer_Point_Compute::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Particle_Image/texParticle%d.dds"), 30))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Particle*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		CEffect_Particle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region Sword
	lstrcpyW(m_szLoadingText, TEXT("Effect_Sword 생성중"));
	/* For.Prototype_Component_Shader_Effect_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Sword"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect_Sword.hlsl"), VTXST::Elements, VTXST::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sword_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sword_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Sword_Image/texSword%d.dds"), 11))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Sword */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sword"),
		CSwordTrail_Buffer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Sword*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
		CEffect_Sword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region Mesh_Instancing
	lstrcpyW(m_szLoadingText, TEXT("Effect_Instancing 생성중"));
	/* For.Prototype_Component_Shader_Effect_Instancing */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Instancing"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Effect_Instancing.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Instancing_Image*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Instancing_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Instancing_Image/texInstancing%d.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_Effect_Instancing*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Instancing"),
		CEffect_Instancing::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region 항상 있는 오브젝트(의자나 사다리 램프 같은거)

	_matrix SpecificPreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_P_Archive_Chair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Archive_Chair01/P_Archive_Chair01.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_NPCLamp"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Lamp/NPCLamp.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SM_Woods"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ladder/Ladder.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_ChairLamp*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChairLamp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Destruct/T_Noise_Small.png"), 1))))
		return E_FAIL;

	;

	SpecificPreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f)); // 맵툴과 똑같이 pretransform 적용

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Specific_Door"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_07.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_P_Ladder02_Up"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Circus_08/P_Ladder02_Up.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_P_Ladder02_Down"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Circus_08/P_Ladder02_Down.fbx", CModel::MODEL_NONANIM, SpecificPreTransformMatrix))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_SpecificObject"), CSpecificObject::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Chair"), CChair::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_ChairLamp"), CChairLamp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Elevator_Door"), CElevator_Door::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Ladder_Object"), CLadderObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Door_Object"), CDoorObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Door_Manager_Object"), CDoorManager::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion

#pragma region 부술 수 있는 오브젝트 

	_matrix DestructPreTransformMatrix = XMMatrixIdentity();
	DestructPreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_barrel"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/DestructibleObjects/barrel.fbx", CModel::MODEL_NONANIM, DestructPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Rock"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/DestructibleObjects/Rock.fbx", CModel::MODEL_NONANIM, DestructPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Fence2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/DestructibleObjects/Fence2.fbx", CModel::MODEL_NONANIM, DestructPreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TestFenceNoiseTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Destruct/T_Noise_Small.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxDestructMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxDestructMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_DestructObject"),
		CDestructObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region 나생문 오브젝트 ( 보스 방 특수 방벽 )
	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_Model_BarrierScreen"), CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/BarrierScreen/BarrierScreen.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_GameObject_BarrierScreen"), CBarrierScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BarrierScreenTexture"), CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Item/Item01/T_Noise_03.png"), 1))))
		return E_FAIL;
#pragma endregion


	/* 로딩이 완료되었습니다.*/
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Static_Logo::Loading_For_Level_Logo()
{
	m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.1f);
#pragma region Logo 
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 7))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },			
	//};

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("모델 원형을 생성한다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("객체 원형을 생성한다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

#pragma region Logo_Objects

	_matrix PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_DryTreeT1_branches_1_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT1_branches_1_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_SM_fence_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_13.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_SM_fence_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_19.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Model_SM_ground_Road_Middle_250x250cm_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Brick_Floor/brickFloor.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;



	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region Logo_Camera

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_Camera_Logo"),
		CCamera_Logo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
	/* 로딩이 완료되었습니다.*/
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;


	return S_OK;
#pragma endregion 
}

HRESULT CLoader_Static_Logo::Load_BinaryModels(const _char* pFilePath, _matrix PreTransformMatrix)
{
	return S_OK;
}


CLoader_Static_Logo * CLoader_Static_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader_Static_Logo*	pInstance = new CLoader_Static_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader_Static_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Static_Logo::Free()
{
	__super::Free();
}
