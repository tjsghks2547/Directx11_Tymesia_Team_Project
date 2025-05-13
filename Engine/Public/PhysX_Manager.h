#pragma once 

#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CPhysX_SimulationEventCallBack;

class ENGINE_DLL CPhysX_Manager : public CBase
{
	CPhysX_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPhysX_Manager() = default;

public:

	HRESULT initialize();

	HRESULT Create_Scene();
	HRESULT Clear_Scene();

	XMVECTOR Convert_Vector(const PxVec3& In_PxVec3);
	XMVECTOR Convert_Vector(const PxVec4& In_PxVec4);

	PxRigidDynamic* Create_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis = { 0.f,0.f,0.f }, _float _degree = 0.f, CGameObject* _pGameObject = nullptr);
	HRESULT			Add_Actor_Scene(PxRigidDynamic* pActor);  // 여기서부터 작업 다시하기	
	HRESULT			Sub_Actor_Scene(PxRigidDynamic* pActor);  // 여기서부터 작업 다시하기	
	HRESULT         Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup);

	HRESULT         IsActorInScene(PxRigidDynamic* pActor);	

public:
	HRESULT Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet);
	HRESULT Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition);



private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };


private:
	PxFoundation* m_pFoundation = { nullptr };
	PxPhysics* m_pPhysics = { nullptr };
	PxScene* m_pScene = { nullptr };
	PxMaterial* m_pMaterial = { nullptr };
	PxPvd* m_pPvD = { nullptr };

	PxDefaultAllocator              m_DefaultAllocatorCallback;
	PxDefaultErrorCallback          m_DefualtErrorCallback;

	PxCudaContextManager* m_pCudaContextManager = { nullptr };	
	PxDefaultCpuDispatcher* m_pCpuDispatcher = { nullptr };	
	CPhysX_SimulationEventCallBack* m_pSimulationEventCallBack = { nullptr };

//#ifdef _DEBUG
	/*충돌체 렌더링*/
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
public:
	HRESULT 		Render_PhysXDebugger();
//#endif

public:
	HRESULT Update(_float fTimeDelta);

public:
	static CPhysX_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};
END