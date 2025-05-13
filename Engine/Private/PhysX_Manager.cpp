#include "PhysX_Manager.h"
#include "PhysX_SimulationEventCallBack.h"  

#include "GameInstance.h"
#include "GameObject.h"
#include "DebugDraw.h"

#pragma region CollisionFilter 
PxFilterFlags CollisionFilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    if (filterData0.word2 != 1 || filterData1.word2 != 1)  // word2 �� � �׷��� �������� 
    {
        return PxFilterFlag::eKILL;
    }

    if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) // word�� �׷�Ÿ��  word1�� � �׷�� �ٷ���  
    {
        pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND; //Collision Enter
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST; //Collision Exit
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_PERSISTS;//Collision Stay
        pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;// �浹 ���� ����(��ġ, ���� ����, ���� ��) ����
        pairFlags |= PxPairFlag::eTRIGGER_DEFAULT;
        //pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
        return PxFilterFlag::eDEFAULT;
    }
    else
    {
        return PxFilterFlag::eKILL;
    }

    return PxFilterFlag::eDEFAULT;
}
#pragma endregion 

CPhysX_Manager::CPhysX_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    , m_pContext(pContext)
    , m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CPhysX_Manager::initialize()
{
    //Step 1. PhysX ������ ���� ��ü ���� ( �ʼ� ) 
    m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefualtErrorCallback);

    //Step 2. PhysX �ٽ� ��ü(PxPhysics) ���� 
    m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvD);


    m_pPvD = PxCreatePvd(*m_pFoundation);


    /* �ش� Scene������ ��� Ʈ���� �̺�Ʈ �߻� ��� ��� �� */
    m_pSimulationEventCallBack = new CPhysX_SimulationEventCallBack();


    // Cuda ����
   /* PxCudaContextManagerDesc tCudaDesc;
    tCudaDesc.graphicsDevice = m_pDevice;

    m_pCudaContextManager = PxCreateCudaContextManager(*m_pFoundation, tCudaDesc, PxGetProfilerCallback());

    if (m_pCudaContextManager)
    {
        if (!m_pCudaContextManager->contextIsValid())
        {
            if (m_pCudaContextManager)
                m_pCudaContextManager->release();
            m_pCudaContextManager = nullptr;
        }
    }
    else
    {
        assert(false);
    }*/
    /*--------------------------*/


    SYSTEM_INFO info;
    GetSystemInfo(&info);
    m_pCpuDispatcher = PxDefaultCpuDispatcherCreate(info.dwNumberOfProcessors);  // ��Ƽ ������ ó��   

    if (!m_pCpuDispatcher)
        assert(false);


    /* Scene ���� */
    Create_Scene();

    return S_OK;
}

HRESULT CPhysX_Manager::Create_Scene()
{
    //Step 3. PhysX ��� (Scene) ���� 
// Scene ����
    PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);

    sceneDesc.cpuDispatcher = m_pCpuDispatcher;
    sceneDesc.filterShader = CollisionFilterShader;
    sceneDesc.simulationEventCallback = m_pSimulationEventCallBack;
    //sceneDesc.cudaContextManager = m_pCudaContextManager;
    sceneDesc.broadPhaseType = PxBroadPhaseType::eGPU;
    sceneDesc.flags |= PxSceneFlag::eENABLE_GPU_DYNAMICS;   	//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
    sceneDesc.flags |= PxSceneFlag::eENABLE_PCM;		    	//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
    sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;	    //Improve solver stability by enabling post-stabilization.
    //sceneDesc.flags|=PxSceneFlag::
    sceneDesc.gpuMaxNumPartitions = 8;			    			//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
    //A value of 8 generally gives best balance between performance and stability.
    sceneDesc.kineKineFilteringMode = PxPairFilteringMode::eKEEP; //Kinematic - Kinematic �浹 Ȱ��ȭ    
    sceneDesc.staticKineFilteringMode = PxPairFilteringMode::eKILL; //Static - Kinematic �浹 ��Ȱ��ȭ    
    //sceneDesc.flags |= PxSceneFlag::eENABLE_CCD;


    // Scene ��ü ���� 
    m_pScene = m_pPhysics->createScene(sceneDesc);



#pragma region ����� ������ ���� 
    PxPvdSceneClient* pvdClient = m_pScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    /*�浹ü �ð�ȭ ���� ����*/
    m_pScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_EDGES, 2.0f);
    m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 2.0f);
    m_pScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_STATIC, 2.0f);
    m_pScene->setVisualizationParameter(PxVisualizationParameter::eCONTACT_NORMAL, 2.0f);

#pragma endregion 
//#ifdef _DEBUG
    // �浹ü �������� IA ����
    m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
    m_pEffect = new BasicEffect(m_pDevice);
    m_pEffect->SetVertexColorEnabled(true);
    
    const void* pShaderByteCode = { nullptr };
    size_t	iShaderCodeLength = { 0 };
    
    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderCodeLength);

    if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderCodeLength, &m_pInputLayout)))
        return E_FAIL;
//#endif

    return S_OK;
}

HRESULT CPhysX_Manager::Clear_Scene()
{
    // ���� ���� �ִ� ��� ���� ��������
    PxU32 actorCount = m_pScene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);

    if (actorCount > 0)
    {
        std::vector<PxActor*> actors(actorCount);
        m_pScene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, actors.data(), actorCount);

        // ��� ���� ����
        for (PxActor* actor : actors)
        {
            if (actor)
            {
                m_pScene->removeActor(*actor);
                actor->release(); // �޸� ����     
            }
        }
    }
    return S_OK;
}

XMVECTOR CPhysX_Manager::Convert_Vector(const PxVec3& In_PxVec3)
{
    return XMVectorSet(_float(In_PxVec3.x), _float(In_PxVec3.y), _float(In_PxVec3.z), 0.f);
}

XMVECTOR CPhysX_Manager::Convert_Vector(const PxVec4& In_PxVec4)
{
    return XMVectorSet(_float(In_PxVec4.x), _float(In_PxVec4.y), _float(In_PxVec4.z), _float(In_PxVec4.w));
}

PxRigidDynamic* CPhysX_Manager::Create_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis,
    _float _degree, CGameObject* _pGameObject)
{
    PxMaterial* pMaterial = m_pPhysics->createMaterial(0.0f, 0.0f, 0.0f);
    PxRigidDynamic* pActor = nullptr;


    if (_eType == COLLIDER_TYPE::COLLIDER_BOX)
    {
        // Step 1. Box �浹ü ��� �����   -> �ڽ���� �����ϸ�� 
        PxBoxGeometry BOX(_Scale.x, _Scale.y, _Scale.z);
        PxQuat rotation(XMConvertToRadians(_degree), PxVec3(_Axis.x, _Axis.y, _Axis.z));
        // Step 2. ���� ������Ʈ(RigidDynamic) ����         
        pActor = PxCreateDynamic(*m_pPhysics, PxTransform(PxVec3(0.f, 0.f, 0.f)), BOX, *pMaterial, 1.0f);
        // Step 3. Scene�� �߰��Ͽ� �浹 ���� Ȱ��ȭ                

        PxShape* pShape;
        pActor->getShapes(&pShape, 1);
        pShape->setLocalPose(PxTransform(rotation));
        // ���߿� �������ٷ��� Ű��ƽ���� ������ ���־����.  
        pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // PxFilterData ����
        PxFilterData filterData;
        filterData.word0 = 0x0001; // �� ��ü�� �׷� (��: �÷��̾�)                
        filterData.word1 = 0x0011; // �浹 ������ �׷� (��=0b0010, ��=0b0001)                   
        filterData.word2 = 0x0001; //       

        // shape�� �浹 ���� ����
        pShape->setSimulationFilterData(filterData);

        pActor->userData = _pGameObject;


    }
    else if (_eType == COLLIDER_TYPE::COLLIDER_CAPSULE)
    {
        PxCapsuleGeometry capsuleGeom(_Scale.x, _Scale.y);

        PxQuat rotation(XMConvertToRadians(_degree), PxVec3(_Axis.x, _Axis.y, _Axis.z));
        pActor = PxCreateDynamic(*m_pPhysics, PxTransform(PxVec3(0.f, 0.f, 0.f)), capsuleGeom, *pMaterial, 1.0f);
        PxShape* pShape;
        pActor->getShapes(&pShape, 1);
        pShape->setLocalPose(PxTransform(rotation));
        pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // PxFilterData ����
        PxFilterData filterData;
        filterData.word0 = 0x0001; // �� ��ü�� �׷� (��: �÷��̾�)                
        filterData.word1 = 0x0011; // �浹 ������ �׷� (��=0b0010, ��=0b0001)                    
        filterData.word2 = 0x0001; //        

        // shape�� �浹 ���� ����
        pShape->setSimulationFilterData(filterData);

        pActor->userData = _pGameObject;

    }

    else if (_eType == COLLIDER_TYPE::COLLIDER_SPHERE)
    {
        PxSphereGeometry sphereGeom(_Scale.x);

        PxQuat rotation(XMConvertToRadians(_degree), PxVec3(_Axis.x, _Axis.y, _Axis.z));
        pActor = PxCreateDynamic(*m_pPhysics, PxTransform(PxVec3(0.f, 0.f, 0.f)), sphereGeom, *pMaterial, 1.0f);

        PxShape* pShape;
        pActor->getShapes(&pShape, 1);
        pShape->setLocalPose(PxTransform(rotation));
        pActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

        // PxFilterData ����
        PxFilterData filterData;
        filterData.word0 = 0x0001; // �� ��ü�� �׷� (��: �÷��̾�)                    
        filterData.word1 = 0x0011; // �浹 ������ �׷� (��=0x0010, ��=0b0001)                        
        filterData.word2 = 0x0001; //           

        // shape�� �浹 ���� ����
        pShape->setSimulationFilterData(filterData);

        pActor->userData = _pGameObject;


    }


    if (pActor == nullptr)
    {
        MSG_BOX("���� ���� ����");
    }

    return pActor;
}


HRESULT CPhysX_Manager::Add_Actor_Scene(PxRigidDynamic* pActor)
{
    if (pActor == nullptr)
        return E_FAIL;

    if (pActor->getScene() != m_pScene)
        m_pScene->addActor(*pActor);

    return S_OK;
}


HRESULT CPhysX_Manager::Sub_Actor_Scene(PxRigidDynamic* pActor)
{
    if (pActor == nullptr)
        return E_FAIL;

    if (pActor->getScene() == m_pScene)
        m_pScene->removeActor(*pActor);

    return S_OK;
}

HRESULT CPhysX_Manager::Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup)
{
    PxShape* pShape;
    pActor->getShapes(&pShape, 1);

    // PxFilterData ����
    PxFilterData filterData;
    filterData.word0 = (PxU32)_eMeType; // �� ��ü�� �׷� (��: �÷��̾�)                               
    filterData.word1 = (PxU32)_ColliderGroup;
    filterData.word2 = 0x0001;

    // shape�� �浹 ���� ����
    pShape->setSimulationFilterData(filterData);

    return S_OK;
}

HRESULT CPhysX_Manager::IsActorInScene(PxRigidDynamic* pActor)
{
    if (pActor == nullptr)
        return E_FAIL;

    if (pActor->getScene() == m_pScene) 
        return S_OK;    
    else
        return E_FAIL;
}

HRESULT CPhysX_Manager::Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet)
{
    if (Actor == nullptr)
        return E_FAIL;


    _vector CurPosition = XMVector3TransformCoord(_vOffSet, _WorldMatrix);

    _WorldMatrix.r[3] = CurPosition;

    // DirectX�� XMMATRIX�� �����Ͽ� PhysX PxTransform���� ��ȯ       
    XMVECTOR scale, rotationQuat, translation;
    XMMatrixDecompose(&scale, &rotationQuat, &translation, _WorldMatrix);

    // DirectX�� Quaternion�� PhysX Quaternion���� ��ȯ
    PxQuat pxRotation;
    XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(&pxRotation), rotationQuat);

    // ��ȯ�� PxTransform ��ȯ
    PxTransform newTransform(
        PxVec3(XMVectorGetX(translation), XMVectorGetY(translation), XMVectorGetZ(translation)),
        pxRotation);

    Actor->setGlobalPose(newTransform);
    Actor->setKinematicTarget(newTransform);

    return S_OK;
}

HRESULT CPhysX_Manager::Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition)
{
    if (Actor == nullptr)
        return E_FAIL;

    PxVec3 Position = { _fPosition.m128_f32[0],_fPosition.m128_f32[1],_fPosition.m128_f32[2] };

    // �浹ü ��ġ ����    
    PxTransform newTransform(Position);

    Actor->setGlobalPose(newTransform);

    return S_OK;
}

//#ifdef _DEBUG   
HRESULT CPhysX_Manager::Render_PhysXDebugger()
{
    m_pContext->GSSetShader(nullptr, nullptr, 0);

    m_pBatch->Begin();

    m_pEffect->SetWorld(XMMatrixIdentity());
    m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
    m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

    m_pContext->IASetInputLayout(m_pInputLayout);

    m_pEffect->Apply(m_pContext);

    const PxRenderBuffer& rb = m_pScene->getRenderBuffer();

    for (PxU32 i = 0; i < rb.getNbTriangles(); i++)
    {
        const PxDebugTriangle& triangle = rb.getTriangles()[i];
        // render the point
        DX::DrawTriangle(m_pBatch, Convert_Vector(triangle.pos0), Convert_Vector(triangle.pos1), Convert_Vector(triangle.pos2), XMVectorSet(1.f, 0.f, 1.f, 1.f));
    }

    for (PxU32 i = 0; i < rb.getNbLines(); i++)
    {
        const PxDebugLine& line = rb.getLines()[i];
        // render the point
        VertexPositionColor verts[2];
        XMStoreFloat3(&verts[0].position, Convert_Vector(line.pos0));
        XMStoreFloat3(&verts[1].position, Convert_Vector(line.pos1));

        XMStoreFloat4(&verts[0].color, XMVectorSet(1.f, 0.f, 1.f, 1.f));
        XMStoreFloat4(&verts[1].color, XMVectorSet(1.f, 0.f, 1.f, 1.f));
        m_pBatch->DrawLine(verts[0], verts[1]);
    }

    m_pBatch->End();
    return S_OK;
}
//#endif // _DEBUG


HRESULT CPhysX_Manager::Update(_float fTimeDelta)
{
    if (m_pScene)
    {
        m_pScene->simulate(fTimeDelta);  // ���� �ùķ��̼� ����     
        m_pScene->fetchResults(true);    // ��� ��������     
    }

    return S_OK;
}

CPhysX_Manager* CPhysX_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPhysX_Manager* pInstance = new CPhysX_Manager(pDevice, pContext);
    if (FAILED(pInstance->initialize()))
    {
        MSG_BOX("Failed to Created : PhysX");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPhysX_Manager::Free()
{
    __super::Free();
    PX_RELEASE(m_pScene);
    PX_RELEASE(m_pCpuDispatcher);
    PX_RELEASE(m_pPvD);
    PX_RELEASE(m_pCudaContextManager);

    PX_RELEASE(m_pPhysics);
    PX_RELEASE(m_pFoundation);


//#ifdef _DEBUG
    Safe_Delete(m_pEffect);
    Safe_Delete(m_pBatch);
    Safe_Release(m_pInputLayout);
//#endif

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pGameInstance);

    Safe_Delete(m_pSimulationEventCallBack);
}
