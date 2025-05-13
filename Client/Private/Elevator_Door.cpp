    #include "pch.h"
#include "Elevator_Door.h"
#include "GameInstance.h"

#include "StaticObject.h"
#include "GroundObject.h"

CElevator_Door::CElevator_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSpecificObject{ pDevice, pContext }
{
}

CElevator_Door::CElevator_Door(const CElevator_Door& Prototype)
    :CSpecificObject(Prototype)
{
}

HRESULT CElevator_Door::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElevator_Door::Initialize(void* pArg)
{
    ElevatorDoorDesc* pDesc = static_cast<ElevatorDoorDesc*>(pArg);

    m_iCurrentLevel = pDesc->iCurLevel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 2.f, 2.f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);
    m_bColliderOn = true;

    m_isCulling = pDesc->isCulling;
    m_wstrCullingLayerTag = pDesc->pCullingLayerTag;
    m_wstrLoadLayerTag = pDesc->pLoadLayerTag;
    m_vecLoadIndex = pDesc->iLoadIndex;
    m_pPlayerTransform = pDesc->pPlayerTransform;
    m_pPlayerNavigation = pDesc->pPlayerNavigation;

    return S_OK;
}

void CElevator_Door::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CElevator_Door::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CElevator_Door::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CElevator_Door::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElevator_Door::Render_Glow()
{
    if (FAILED(__super::Render_Glow()))
        return E_FAIL;

    return S_OK;
}

void CElevator_Door::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vDoorPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vDoorPos = XMVectorSetY(vDoorPos, XMVectorGetY(vDoorPos) + 1.0f);

    _float4 vDoorPosition;
    XMStoreFloat4(&vDoorPosition, vDoorPos);

    m_pButton->Set_WorldPosition(vDoorPosition);
    m_bInteractOn = true;
    m_bFadingIn = true;
    m_bFadingOut = false;
}

void CElevator_Door::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Set_ButtonText(TEXT("E"), TEXT("ÀÌµ¿"));
    m_pButton->Activate_Button(true);

    if (m_pGameInstance->isKeyEnter(DIK_E) )
    {
        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 1.f);
    }

    if (m_isCulling && m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
    {
        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 1.f);

        for (auto& pIndex : m_vecLoadIndex)
        {
            if (FAILED(Load_Objects(pIndex)))
            {
                MSG_BOX("Failed To LoadObjects!");
            }
        }

        m_pGameInstance->Delete_GameObject_From_Layer(m_iCurrentLevel, m_wstrCullingLayerTag);


        if (!strncmp(m_szName, "Elevator_Up", 11))
        {
            _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            vPosition.m128_f32[1] += 16.f;

            m_pPlayerNavigation->Set_CurCellIndex(m_pPlayerNavigation->Find_Closest_Cell(vPosition));
        }
        else if (!strncmp(m_szName, "Elevator_Down", 13))
        {
            _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            vPosition.m128_f32[1] -= 16.f;

            m_pPlayerNavigation->Set_CurCellIndex(m_pPlayerNavigation->Find_Closest_Cell(vPosition));
        }

    }
    else if (!m_isCulling && m_pGameInstance->Is_Fade_Complete(TRIGGER_TYPE::TT_FADE_OUT))
    {
        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 1.f);

        if (!strncmp(m_szName, "Elevator_Up", 11))
        {
            _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            vPosition.m128_f32[1] += 16.f;

            m_pPlayerNavigation->Set_CurCellIndex(m_pPlayerNavigation->Find_Closest_Cell(vPosition));
        }
        else if (!strncmp(m_szName, "Elevator_Down", 13))
        {
            _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

            vPosition.m128_f32[1] -= 16.f;

            m_pPlayerNavigation->Set_CurCellIndex(m_pPlayerNavigation->Find_Closest_Cell(vPosition));
        }
    }
}

void CElevator_Door::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
    m_bInteractOn = false;
    m_bFadingIn = false;
    m_bFadingOut = true;
}

HRESULT CElevator_Door::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Specific_Door"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CElevator_Door::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

HRESULT CElevator_Door::Load_Objects(_int iObject_Level)
{
    _ulong dwByte = {};
    _ulong dwByte2 = {};

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
        string strName = szLoadName;

        Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

        CObject* pObject = nullptr;

        if (Desc.iObjectType == CObject::OBJECT_DEFAULT)
        {
            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_SEAOFTREES, m_wstrLoadLayerTag, &Desc)))
                return E_FAIL;
        }
        else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
        {
            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_BillBoardObject"), LEVEL_SEAOFTREES, m_wstrLoadLayerTag, &Desc)))
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

        CEnvironmentObject* pEnvironment = reinterpret_cast<CEnvironmentObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_SEAOFTREES, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_SEAOFTREES, m_wstrLoadLayerTag, &Desc));

        if (nullptr == pEnvironment)
            return E_FAIL;
    }

    CloseHandle(hFile);

    return S_OK;
}

CElevator_Door* CElevator_Door::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CElevator_Door* pInstance = new CElevator_Door(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CElevator_Up");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CElevator_Door::Clone(void* pArg)
{
    CElevator_Door* pInstance = new CElevator_Door(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Created : CElevator_Door");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CElevator_Door::Free()
{
    __super::Free();

    m_pGameInstance->Sub_Actor_Scene(m_pActor);
}
