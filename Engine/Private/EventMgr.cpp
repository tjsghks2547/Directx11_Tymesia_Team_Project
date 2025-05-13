#include "EventMgr.h"
#include "GameObject.h"
#include "GameInstance.h"

CEventMgr::CEventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
	,m_pGameInstance(CGameInstance::GetInstance())	
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	Safe_AddRef(m_pGameInstance);	
}

HRESULT CEventMgr::Initialize()
{
	
	return S_OK;	
}

HRESULT CEventMgr::Event_Activate(_wstring _EventName)
{
	auto& iter = m_mapEvent.find(_EventName);

	/* ���� �ÿ� */
	if (iter == m_mapEvent.end())
		return E_FAIL;
	
	iter->second->Activate(_EventName);	

	return S_OK;
}

HRESULT CEventMgr::Add_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	auto& iter = m_mapEvent.find(_EventName);
	
	if (pGaemObject == nullptr)
		return E_FAIL; 

	/* �ش� �̺�Ʈ�� ó�� ��������� �ʿ� ����� �ȵ����� �� */
	if (iter == m_mapEvent.end())
	{
		CEvent* pEvent = CEvent::Create(m_pDevice, m_pContext);
		if (pEvent == nullptr)
			return E_FAIL; 

		//Safe_AddRef(pEvent);	
		
		if(FAILED(pEvent->Add_EventObject(pGaemObject)))
			return E_FAIL;

		m_mapEvent.emplace(_EventName, pEvent);
	}

	else
	{
		if (FAILED(iter->second->Add_EventObject(pGaemObject)))
			return E_FAIL;	
	}


	return S_OK;	
}

HRESULT CEventMgr::Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	auto& iter = m_mapEvent.find(_EventName);		

	if (pGaemObject == nullptr)	
		return E_FAIL;	

	/* �ش� �̺�Ʈ�� ó�� ��������� �ʿ� ����� �ȵ����� �� */
	if (iter == m_mapEvent.end())
	{
		return E_FAIL;	
	}

	else
	{
		auto& pEvent = iter->second;
		pEvent->Sub_EventObject(pGaemObject);
	}



	return S_OK;
}

HRESULT CEventMgr::Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject)	
{
	if (pGaemObject == nullptr)
		return E_FAIL; 

	m_mapDeadObejct.emplace(_LayerName, pGaemObject);	


	return S_OK;	
}

HRESULT CEventMgr::Add_DeadObjects(_wstring _LayerName, CGameObject* pGaemObject, _uint iCurrenteLevel)
{
	if (pGaemObject == nullptr)
		return E_FAIL;

	m_mapDeadObejcts.emplace(_LayerName, pGaemObject);

	m_iCurrent_Level = iCurrenteLevel;

	return S_OK;
}

HRESULT CEventMgr::Add_DeadEffect(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return E_FAIL;

	m_vecDeadEffect.push_back(pGameObject);
	/* Safe_AddRef�� ���� ���� ������ �ű� ������ �Ѵٸ� 2�� Safe_Release�� �ؾ��ϴ� ��Ȳ �߻� */

	return S_OK;	
}


HRESULT CEventMgr::Update()
{

	if (m_mapDeadObejct.size() > 0)	
	{	
		for (auto& iterator = m_mapDeadObejct.begin(); iterator != m_mapDeadObejct.end(); iterator++)
		{
			m_pGameInstance->Sub_GameObject_To_Layer(m_iCurrent_Level, (*iterator).first, (*iterator).second);
			m_mapDeadObejct.erase(iterator);	

			if (m_mapDeadObejct.size() == 0)
				break;	
		}
	}

	if (m_mapDeadObejcts.size() > 0)
	{
		for (auto& iterator = m_mapDeadObejcts.begin(); iterator != m_mapDeadObejcts.end();)
		{
			m_pGameInstance->Sub_GameObject_To_Layer(m_iCurrent_Level, (*iterator).first, (*iterator).second);
			iterator = m_mapDeadObejcts.erase(iterator);

			if (m_mapDeadObejcts.size() == 0)
				break;
		}
	}


	//if (m_vecDeadEffect.size() > 0)	
	//{
	//	for (auto& iterator = m_vecDeadEffect.begin(); iterator != m_vecDeadEffect.end(); iterator++)
	//	{
	//		m_pGameInstance->Sub_Effect(*iterator);	
	//		m_vecDeadEffect.erase(iterator);	
	//
	//		if (m_vecDeadEffect.size() == 0)	
	//			break;	
	//	}
	//}


	return S_OK;
}

CEventMgr* CEventMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEventMgr* pInstance = new CEventMgr(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CEventMgr");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CEventMgr::Free()
{
	__super::Free();

	/* �� �����̳�  ������ �۾� */
	for (auto& Pair : m_mapEvent)
		Pair.second->Release();

	m_mapEvent.clear();	

	for (auto& iter : m_vecDeadEffect)
		iter->Release();

	m_vecDeadEffect.clear();


	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
