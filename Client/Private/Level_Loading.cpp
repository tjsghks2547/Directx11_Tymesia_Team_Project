#include "pch.h" 
#include "Level_Loading.h"

#include "GameInstance.h"

#include "Loader.h"
#include "Loader_Static_Logo.h"
#include "Loader_Tutorial.h"
#include "Loader_SeaOfTrees.h"
#include "Loader_RoyalGarden.h"
#include "Loader_Ocean.h"
#include "Loader_Hill.h"

#include "Level_Logo.h"
#include "Level_Tutorial.h"
#include "Level_SeaOfTrees.h"
#include "Level_RoyalGarden.h"
#include "Level_Ocean.h"
#include "Level_Hill.h"


#include "UI_Scene.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_LoadingScreen.h"
#include "UI_LoadingIcon.h"

#include "Blackscreen.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID, _uint iLoadingNum, _bool bCheck)
{
	m_eNextLevelID = eNextLevelID;
	m_iLoadingeNum = iLoadingNum;
		//종한 0404 추가
	m_pGameInstance->Set_Boss_Dead(false);
	if (bCheck)
	{
  		if (FAILED(Ready_Layer_UIGroup_Loading(TEXT("Layer_Loading"))))
			return E_FAIL;	
		if (FAILED(Ready_Layer_Mouse(TEXT("Layer_Mouse"))))
			return E_FAIL;	

		//m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true);

		if (FAILED(Ready_Layer_Fade(TEXT("Layer_Fade"))))
			return E_FAIL;

		m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
		m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);
		m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_AUTO);
	}
	

	switch (m_eNextLevelID)
	{
	case Client::LEVEL_STATIC:
	case Client::LEVEL_LOGO:

		m_pLoader = CLoader_Static_Logo::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;

		break;
	case Client::LEVEL_TUTORIAL:
		m_pGameInstance->PlayBGM(L"TutoMapBGM.ogg", 0.15f);
		m_pLoader = CLoader_Tutorial::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;
		break;
	case Client::LEVEL_SEAOFTREES:
		m_pGameInstance->PlayBGM(L"Sea_of_Trees_Level_-_Exploration.ogg", 0.15f);
		m_pLoader = CLoader_SeaOfTrees::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;

		break;
	case Client::LEVEL_ROYALGARDEN:
		m_pGameInstance->PlayBGM(L"Garden_Level_-_Exploration.ogg", 0.15f);
		m_pLoader = CLoader_RoyalGarden::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;

		break;
	case Client::LEVEL_OCEAN:

		m_pLoader = CLoader_Ocean::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;

		break;
	case Client::LEVEL_HILL:
		m_pGameInstance->PlayBGM(L"Safe_House_-_Long.ogg", 0.8f);
		m_pLoader = CLoader_Hill::Create(m_pDevice, m_pContext, eNextLevelID);
		if (nullptr == m_pLoader)
			return E_FAIL;

		break;
	}

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (m_eNextLevelID > LEVEL_STATIC)
	{
		
		CUI_Scene* pScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");
		
		for (auto& Image : pScene->Find_UI_Image())
		{
			if (0 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CUI_LoadingScreen*>(Image)->Set_TexNumber(m_iLoadingeNum);
				break;
			}
		}
		
		for (auto& TextBox : pScene->Find_UI_TextBox())
		{
			if (0 == TextBox->Get_UI_GroupID())
			{
				TextBox->Set_OnOff(false);
				break;
			}
		}
		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Loading"), true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")), true);

		for (auto& Image : m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")->Find_UI_Image())
		{
			if (1 == Image->Get_UI_GroupID())
				Image->Set_OnOff(true);
		}

	}
	if (true == m_pLoader->isFinished())
	{
		if (m_eNextLevelID == LEVEL_STATIC)
		{
			m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOGO, 0, true)); // 여기 안에서 페이드 생성 

		}
		if (m_eNextLevelID == LEVEL_LOGO)
		{
			m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_Logo::Create(m_pDevice, m_pContext));
		}

	}

	if (m_eNextLevelID > LEVEL_LOGO) // 스테이지영역으로 가는 Loading이냐 
	{
		if (true == m_pLoader->isFinished())
		{
			CUI_Scene* pScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");
			for (auto& Image : m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")->Find_UI_Image())
			{
				if (1 == Image->Get_UI_GroupID())
					Image->Set_OnOff(false);
			}

			for (auto& TextBox : pScene->Find_UI_TextBox())
			{
				if (0 == TextBox->Get_UI_GroupID())

				{
					dynamic_cast<CUI_Text*>(TextBox)->Set_TextDrawType(Engine::TEXT_ALPHALOOP);
					TextBox->Set_OnOff(true);
					break;
				}
			}
			if (m_pGameInstance->isAnyEnter())
			{
				for (auto& Image : m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")->Find_UI_Image())
				{
					if (1 == Image->Get_UI_GroupID())
					{
						m_pGameInstance->Play_Sound(TEXT("Fantasy_Game_UI_Arcane_Select.ogg"), CHANNELID::SOUND_UI, 0.2f);
						dynamic_cast<CUI_LoadingIcon*>(Image)->Set_CurrentTime(0.0f);
					}
				}
				
				//m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.2f);

				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기

				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOADING, TEXT("Layer_Loading"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")), false);
				switch (m_eNextLevelID)
				{
				case Client::LEVEL_TUTORIAL:
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_Tutorial::Create(m_pDevice, m_pContext));
					break;

				case Client::LEVEL_SEAOFTREES:
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_SeaOfTrees::Create(m_pDevice, m_pContext));
					break;

				case Client::LEVEL_ROYALGARDEN:
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_RoyalGarden::Create(m_pDevice, m_pContext));
					break;

				case Client::LEVEL_OCEAN:
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_Ocean::Create(m_pDevice, m_pContext));
					break;

				case Client::LEVEL_HILL:
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_Hill::Create(m_pDevice, m_pContext));
					break;
				}
			}
		}
	}
}

#ifdef _DEBUG
HRESULT CLevel_Loading::Render()
{
	m_pLoader->SetUp_WindowText();

	return S_OK;
}
#endif // _DEBUG

HRESULT CLevel_Loading::Ready_Layer_UIGroup_Loading(const _tchar* pLayerTag)
{
 	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Loading"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Mouse(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MouseCursor"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), LEVEL_STATIC, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID, _uint iLoadingNum, _bool bCheck)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

  	if (FAILED(pInstance->Initialize(eNextLevelID, iLoadingNum, bCheck)))
	{
		MSG_BOX("Failed To Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
