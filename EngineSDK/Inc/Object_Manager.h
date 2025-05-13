#pragma once

#include "Base.h"

/*1. ������ü�� �����Ͽ� == CPrototype_Manager�� �߰��س��� ������ü�� Ŭ���Ͽ� ������Ʈ �Ŵ����� �����ϳ�. */
/*2. �����ÿ� CLayerŬ������ �з��Ͽ� ��ü���� �����س���. */
/*3. �����ϰ� �ִ� ��ü���� �ݺ������� Update�� ȣ�����ش�. */
/*4. ��Ƴ��� ��ü���� �����Լ��� ȣ���Ѵ�.(x) */

BEGIN(Engine)

class CComponent;
class CGameObject; 

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	HRESULT Initialize(_uint iNumLevels);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	void Clear(_uint iLevelIndex);

public:
	/* ������ü�� ã�� -> �����ϰ� -> ������ ���̾ �߰��Ѵ�. */
	HRESULT Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, _char* pName = nullptr);	
	CGameObject* Add_GameObject_To_Layer_Take(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, _char* pName = nullptr);
	HRESULT Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject);		
	CGameObject* Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName);

	HRESULT UIGroup_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck);
	_bool UIGroup_Render_State(_uint iLevelIndex, const _wstring& strLayerTag);

	CComponent* Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName);
	list<class CGameObject*>* Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag);	

	map<const _wstring, class CLayer*>* Get_Layers()   { return m_pLayers; }	
	_uint								Get_NumLevel() { return m_iNumLevels; }	

	HRESULT Delete_GameObject_From_Layer(_uint iLevelIndex, const _wstring& strLayerTag);

private:
	map<const _wstring, class CLayer*>*			m_pLayers = { nullptr };
	_uint										m_iNumLevels = {};

	class CGameInstance*						m_pGameInstance = { nullptr };

private:
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	

public:
	static CObject_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END