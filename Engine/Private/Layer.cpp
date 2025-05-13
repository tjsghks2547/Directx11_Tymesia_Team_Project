#include "..\Public\Layer.h"
#include "UIObject.h"
#include "GameObject.h"
#include "Component.h"
#include "GameInstance.h"

CLayer::CLayer()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}


void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(fTimeDelta);
}

CComponent* CLayer::Find_GameObject(const wstring& strComponentName)
{
	for (auto& pGameObejct : m_GameObjects)
	{
		for (auto& iter : *(pGameObejct->Get_Components()))
		{
			if (false == lstrcmp(iter.first.c_str(), strComponentName.c_str()))
				return iter.second;
		}
	}

	return nullptr;

}

HRESULT CLayer::UIGroup_Render_OnOff(_bool bCheck)
{
	dynamic_cast<CUIObject*>(*m_GameObjects.begin())->Set_Render_OnOff(bCheck);
	return S_OK;
}

_bool CLayer::UIGroup_Render_State()
{
	return dynamic_cast<CUIObject*>(*m_GameObjects.begin())->Get_Render_State();
}

HRESULT CLayer::Delete_GameObjects(const wstring& pLayerTag, _uint iCurrentLevel)
{
	for (auto& pObject : m_GameObjects)
	{
		m_pGameInstance->Add_DeadObjects(pLayerTag, pObject, iCurrentLevel);
	}

	return S_OK;
}
CLayer * CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();

	Safe_Release(m_pGameInstance);
}
