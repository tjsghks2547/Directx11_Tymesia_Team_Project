//#pragma once
//
//
//#include "Base.h"
//BEGIN(Engine)
//
//class CGameObject; 
//class CCollider;
//
//union COLLIDER_ID
//{
//	struct
//	{
//		UINT Left_id;
//		UINT Right_id;
//	};
//	ULONGLONG ID;	
//};
//
//
//class ENGINE_DLL CCollisionMgr final : public CBase
//{
//
//private:
//	map <ULONGLONG, bool> m_mapColInfo;//�浹ü ���� ���� ������ �浹 ����	
//	UINT m_arrCheck[(UINT)GROUP_TYPE::GROUP_END]; // �׷� ���� �浹 üũ ��Ʈ����			
//
//private:
//	CCollisionMgr(); 
//	virtual ~CCollisionMgr() = default;		
//
//public:
//	HRESULT Initialize(); 
//	HRESULT Update(); 
//	HRESULT CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
//
//	void Reset()
//	{
//		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::GROUP_END);	
//	}
//
//	HRESULT Add_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject);	 
//	HRESULT Sub_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject);
//
//private:
//	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
//	bool isCollision(CCollider* _pLeftCol, CCollider* _pRightCol);	
//	CGameObject* Find_GameObject(GROUP_TYPE _GroupType, CGameObject* _pGameObject);
//
//private:
//	vector<CGameObject*>		m_arrObjCollider[(UINT)GROUP_TYPE::GROUP_END];			
//
//public:
//	static CCollisionMgr* Create(); 
//	virtual void Free() override; 
//
//};
//
//END