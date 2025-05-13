#include "..\Public\Object_Manager.h"
#include "Layer.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Component.h"


CObject_Manager::CObject_Manager()	
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pLayers = new map<const _wstring, CLayer*>[iNumLevels];

	return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
 		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Priority_Update(fTimeDelta);		
		}
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])	
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
}

HRESULT CObject_Manager::Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg, _char* pName)
{
	/* ���̾ �߰��� �纻��ü�� �����ؼ� �����´�. */
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	

	/* �纻��ü�� �߰��� ���̾ ã��. */
	CLayer*		pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* �纻��ü�� �߰��� ���̾ ������. \*/
	/* ������ ���̾�ü ���� �߰��ϱ����� ��ü����. */
	/* �׷��ϱ� ���̾ �����ߵȴ�. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);
		if (pName != nullptr)
			pGameObject->Set_Name(pName);

			m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else /* �纻��ü�� �߰��� ���̾ �̹� �־���. �׷��ϱ� �� ���̾ �׳� ������ ����./ */
	{
		pLayer->Add_GameObject(pGameObject);
		if (pName != nullptr)
			pGameObject->Set_Name(pName);	
	}

	return S_OK;
}

CGameObject* CObject_Manager::Add_GameObject_To_Layer_Take(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg, _char* pName)
{
	/* ���̾ �߰��� �纻��ü�� �����ؼ� �����´�. */
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return nullptr;

	/* �纻��ü�� �߰��� ���̾ ã��. */
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* �纻��ü�� �߰��� ���̾ ������. \*/
	/* ������ ���̾�ü ���� �߰��ϱ����� ��ü����. */
	/* �׷��ϱ� ���̾ �����ߵȴ�. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);
		if (pName != nullptr)
			pGameObject->Set_Name(pName);

		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else /* �纻��ü�� �߰��� ���̾ �̹� �־���. �׷��ϱ� �� ���̾ �׳� ������ ����./ */
	{
		pLayer->Add_GameObject(pGameObject);
		if (pName != nullptr)
			pGameObject->Set_Name(pName);
	}

	return pGameObject;
}

HRESULT CObject_Manager::Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject)	
{
	/* �纻��ü�� �߰��� ���̾ ã��. */
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);	

	if (pLayer == nullptr)
		return E_FAIL; 

	list<class CGameObject*>& listGameObject = pLayer->Get_GameObject_List();		

	for (auto& iterator = listGameObject.begin(); iterator != listGameObject.end(); iterator++)
	{
		if (*iterator == _pGameObject)
		{
			listGameObject.erase(iterator);
			Safe_Release(_pGameObject);
			return S_OK;	
		}
	}
	
	return E_FAIL;	
}

CGameObject* CObject_Manager::Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName)	
{
	
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);	

	if (pLayer == nullptr)
		return nullptr;

	list<class CGameObject*>& listGameObject = pLayer->Get_GameObject_List();

	for (auto& iter : listGameObject)	
	{
		if (!strcmp(iter->Get_Name(), ObjectName))	
			return iter;
	}

	MSG_BOX("�ش� ���̾��� ������Ʈ ã�� ����");	

	return nullptr;
}

HRESULT CObject_Manager::UIGroup_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (pLayer == nullptr)
		return E_FAIL;;


	return pLayer->UIGroup_Render_OnOff(bCheck);;
}
_bool CObject_Manager::UIGroup_Render_State(_uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/*if (pLayer == nullptr)
		return E_FAIL;;*/

	return pLayer->UIGroup_Render_State();;
}

CComponent* CObject_Manager::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName)
{
	/*������Ʈ �� ���̾� ã��*/
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	/* ���̾ �ִ� ���� ������Ʈ ã��*/
	CComponent* pComponents = pLayer->Find_GameObject(strComponentName);	

	if (pComponents != nullptr)	
	{

		return pComponents;	
	}
	/* ���ӿ�����Ʈ �ȿ��ִ� ������Ʈ ã��*/

	return nullptr;	
}

list<class CGameObject*>* CObject_Manager::Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);	

	if (pLayer == nullptr)
	{
		MSG_BOX("Failed to Find Layer");
	};

	//list<class CGameObject*>& listGameObject = pLayer->Get_GameObject_List();	
	return &pLayer->Get_GameObject_List();
}

HRESULT CObject_Manager::Delete_GameObject_From_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr != pLayer)
	{
		pLayer->Delete_GameObjects(strLayerTag, iLevelIndex);
	}

	return S_OK;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring & strLayerTag)
{
	auto	iter = m_pLayers[iLevelIndex].find(strLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;	
}

CObject_Manager * CObject_Manager::Create(_uint iNumLevels)
{
	CObject_Manager*	pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed To Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CObject_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])		
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	Safe_Delete_Array(m_pLayers);
	Safe_Release(m_pGameInstance);
}
