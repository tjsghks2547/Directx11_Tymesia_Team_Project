//#include "CollisionMgr.h"
//#include "Collider.h"
//#include "GameObject.h"
//
//CCollisionMgr::CCollisionMgr()
//    :m_arrCheck{}   
//{
//}
//
//HRESULT CCollisionMgr::Initialize()
//{
//    return S_OK;
//}
//
//HRESULT CCollisionMgr::Update()
//{
//	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::GROUP_END; ++iRow)	
//	{
//
//		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::GROUP_END; ++iCol)
//		{
//			if (m_arrCheck[iRow] & (1 << iCol))
//			{
//				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);	
//			}
//		}
//	}
//
//    return S_OK;    
//}
//
//HRESULT CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)	
//{
//	// �� ���� ���� �׷� Ÿ���� ������, 
//	// ū ���� ��(��Ʈ) �� ���
//
//	UINT iRow = (UINT)_eLeft;
//	UINT iCol = (UINT)_eRight;
//
//	if (iCol < iRow)
//	{
//		iRow = (UINT)_eRight;
//		iCol = (UINT)_eLeft;
//	}
//
//	if (m_arrCheck[iRow] & (1 << iCol))
//	{
//		m_arrCheck[iRow] &= ~(1 << iCol);
//	}
//
//	else
//	{
//		m_arrCheck[iRow] |= (1 << iCol);
//	}
//    return S_OK;    
//}
//
//HRESULT CCollisionMgr::Add_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject)
//{
//	if (_pGameObject == nullptr || (UINT)_GroupType >= (UINT)GROUP_TYPE::GROUP_END)
//		return E_FAIL; 
//
//	if (Find_GameObject(_GroupType, _pGameObject) == nullptr)	
//	{
//		Safe_AddRef(_pGameObject);	
//		m_arrObjCollider[(UINT)_GroupType].push_back(_pGameObject);		
//	}
//
//	return S_OK;		
//}
//
//HRESULT CCollisionMgr::Sub_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject)
//{
//	if (_pGameObject == nullptr || (UINT)_GroupType >= (UINT)GROUP_TYPE::GROUP_END)	
//		return E_FAIL;	
//
//
//	if (m_arrObjCollider[(UINT)_GroupType].size() > 0)
//	{
//
//		for (auto& iter = m_arrObjCollider[(UINT)_GroupType].begin(); iter != m_arrObjCollider[(UINT)_GroupType].end();)
//		{
//			if (*iter == _pGameObject)
//			{
//				m_arrObjCollider[(UINT)_GroupType].erase(iter);
//				Safe_Release(_pGameObject);
//				return S_OK;	
//			}
//
//			else
//				iter++;
//		}
//	}
//
//	return S_OK;
//}
//
//void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
//{
//
//	const vector<CGameObject*>& vecLeft  = m_arrObjCollider[(UINT)_eLeft];	
//	const vector<CGameObject*>& vecRight = m_arrObjCollider[(UINT)_eRight];	
//
//	map<ULONGLONG, bool>::iterator iter; 
//
//	for(size_t i=0; i< vecLeft.size(); ++i)
//	{
//		for(size_t j=0; j< vecRight.size(); ++j)
//		{
//			CCollider* pLeftCol  = dynamic_cast<CCollider*>(vecLeft[i]->Find_Component(TEXT("Com_Collider")));
//			CCollider* pRightCol = dynamic_cast<CCollider*>(vecRight[j]->Find_Component(TEXT("Com_Collider")));
//
//
//			// �� �浹ü ���� ���̵� ����
//			COLLIDER_ID ID; 
//			ID.Left_id  = pLeftCol->Get_ID();
//			ID.Right_id = pRightCol->Get_ID();
//			ID.ID;
//
//			iter = m_mapColInfo.find(ID.ID);
//
//			if (vecLeft[i] == vecRight[j])	
//			{
//				continue; //�ٽ� ������ ������ ��.�Ʒ��ξȰ��� ( �ڱ� �ڽŰ� �浹üũ �ϴ� ��츦 ���ܽ�Ű�� �� ) 
//			}
//
//			// �浹 ������ �� ��� ������ ��� ��� ( �浹���� �ʾҴٷ�) �� ó�� �ʿ� insert �Ǵ� ���
//
//			if (m_mapColInfo.end() == iter)
//			{
//				m_mapColInfo.insert(make_pair(ID.ID, false));
//				iter = m_mapColInfo.find(ID.ID);	
//			}
//
//			if(isCollision(pLeftCol, pRightCol))	
//			{
//				int a = 4; 
//
//				if (iter->second)
//				{
//					// ���ݵ� �浹���ε� �������� �浹�ϰ� �ִ� ���
//					vecLeft[i]->OnCollision(vecRight[j]);	
//					vecRight[j]->OnCollision(vecLeft[i]);	
//				}
//				else
//				{
//					// �������� �浹���� �ʾҴ�.
//					vecLeft[i]->OnCollisionEnter(vecRight[j]);	
//					vecRight[j]->OnCollisionEnter(vecLeft[i]);	
//					iter->second = true;	
//				}
//
//			}
//
//			else
//			{
//				//���� �浹 ���� ��� 
//				if (iter->second)	
//				{
//					// �������� �浹�ϰ� �־���.
//					vecLeft[i]->OnCollisionExit(vecRight[j]);	
//					vecRight[j]->OnCollisionExit(vecLeft[i]);	
//					iter->second = false;
//				}
//			}
//
//		}
//	}
//
//
//}
//
//bool CCollisionMgr::isCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
//{
//	//_pRightCol->Intersect(_pLeftCol);	 �̰� ������� ������ ���ϳ� �̰� ���� �ٽ��غ��� !!!! 01_10
//	// �׷� m_isColl�� true��������ϳ�. 
//
//	bool leftResult = _pLeftCol->Intersect(_pRightCol);		
//	bool rightResult = _pRightCol->Intersect(_pLeftCol);			
//
//	return leftResult || rightResult;		
//}
//
//CGameObject* CCollisionMgr::Find_GameObject(GROUP_TYPE _GroupType, CGameObject* _pGameObject)	
//{
//
//	for (auto& iter : m_arrObjCollider[(UINT)_GroupType])		
//	{	
//		if (iter == _pGameObject)			
//			return _pGameObject;			
//	}
//	
//
//	return nullptr;	
//}
//
//CCollisionMgr* CCollisionMgr::Create()
//{
//    CCollisionMgr* pInstance = new CCollisionMgr(); 
//
//    if (FAILED(pInstance->Initialize()))
//    {
//        MSG_BOX("Failed to Created : CollisionMgr");
//        Safe_Release(pInstance);    
//    }
//    
//    return pInstance;
//}
//
//void CCollisionMgr::Free()
//{
//    __super::Free();    
//
//	for(int i =0; i< (UINT)GROUP_TYPE::GROUP_END; i++)
//	{
//		for (auto& pGameObject : m_arrObjCollider[i])
//			Safe_Release(pGameObject);
//
//		m_arrObjCollider[i].clear();	
//	}
//
//
//}
