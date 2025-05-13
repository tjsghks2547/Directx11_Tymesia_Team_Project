#include "pch.h" 
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "BlackScreen.h"

#include "StaticObject.h"
#include "GroundObject.h"

#include "Camera_Logo.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
    m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Loading"), false);
    m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")), false);

    m_iCurrentLevel = LEVEL_LOGO;

	m_pGameInstance->Set_Level_ForEventManager(m_iCurrentLevel);


  /*if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;*/
	if (FAILED(Ready_Layer_UIGroup_GameIntro(TEXT("Layer_GameIntro"))))
		return E_FAIL;

    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Structure(TEXT("Layer_Object"))))
        return E_FAIL;

    if (FAILED(Ready_Lights()))
        return E_FAIL;

	m_pGameInstance->PlayBGM(L"LogoSound2.ogg", 0.15f);

	

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
  /*  m_bNextLevelOpen = true;
    m_iNextLevel = LEVEL_TUTORIAL;*/
    if (!m_bNextLevelOpen)
    {
        if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT)) // 로고 전에 화면 가려 놓았음
        {
            m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.9f); // 로고에 들어와서 첫 업데이트 하면 페이드 인으로 화면이 보임 

        }
        if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_IN)) // 화면이 다 보이면
        {
            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOGO, TEXT("Layer_GameIntro"), true); // 인트로 연출 시작
            m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);

        }
    }
	else
	{
        // 다음 레벨로 넘어가라! 라고 값이 들어오면 m_bNextLevelOpen = true;
		if (m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
		{
			m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOGO, TEXT("Layer_GameIntro"), false); // 인트로 ui 종료
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), false);

            /*m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
			m_pGameInstance->PlayBGM(L"LogoSound1.ogg", 0.15f);*/
			m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_iNextLevel), 6, false)); // 다음 레벨로
		}
	}
	
}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		LEVEL_LOGO, pLayerTag, &Desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_LOGO, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Camera(const _tchar* pLayerTag)
{
    CCamera_Logo::LOGO_CAMERA_DESC		Desc = {};

    Desc.vEye = _float3(-379.52f, 9.96f, 62.53f);
    Desc.vAt = _float3(-379.52f, 9.96f, 65.f);

    Desc.fFovy = XMConvertToRadians(50.f);
    Desc.fNear = 0.1f;
    Desc.fFar = 800.f;
    Desc.fSpeedPerSec = 25.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_LOGO, TEXT("Prototype_GameObject_Camera_Logo"), LEVEL_LOGO, pLayerTag, &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	if (FAILED(Load_Objects(0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Lights()
{
    CTransform* pCamTransform = (m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Logo"))->Get_Transfrom();

    _matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
    _vector vCamInfo = { 60.f, 16.f / 9.f , 0.1f, 800.f };

    if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(0.f, 100.3f, -1.f, 1.0f), XMVectorSet(100.f, 0.f, -1.f, 1.f),
        XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
        , matView, vCamInfo, pCamTransform)))
        return E_FAIL;

    LIGHT_DESC LightDesc{};

    ZeroMemory(&LightDesc, sizeof(LightDesc));

    LightDesc.eType = LIGHT_DESC::TYPE_POINT;
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
    LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.vPosition = _float4(-379.520233f, 9.962774f, 62.532284f, 1.f);
    LightDesc.fRange = 70.f;
    LightDesc.iCurrentLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
        return E_FAIL;


    //FOGPARAMS FogDesc{};
    //FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
    //FogDesc.fFogStartDistance = _float2(0.07f, 8.f);
    //FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
    //FogDesc.g_FogColor = _float4(0.f, 0.f, 0.f, 1.f);

    //m_pGameInstance->Set_FogFactors(FogDesc);

    return S_OK;
}

HRESULT CLevel_Logo::Load_Objects(_int iObject_Level)
{
    _ulong dwByte = {};
    _ulong dwByte2 = {};

    string strDataPath = "../Bin/DataFiles/ObjectData/Level_Logo/ObjectData";

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
            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_LOGO, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_LOGO, TEXT("Layer_Object"), &Desc)))
                return E_FAIL;
        }
        else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
        {
            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_LOGO, TEXT("Prototype_GameObject_Object_BillBoardObject"), LEVEL_LOGO, TEXT("Layer_Object"), &Desc)))
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

        CEnvironmentObject* pEnvironment = reinterpret_cast<CEnvironmentObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_LOGO, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_TUTORIAL, TEXT("Layer_GroundObject"), &Desc));

        if (nullptr == pEnvironment)
            return E_FAIL;
    }

    CloseHandle(hFile);

    return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();

    m_pGameInstance->Delete_Static_Light(m_iCurrentLevel);
}
