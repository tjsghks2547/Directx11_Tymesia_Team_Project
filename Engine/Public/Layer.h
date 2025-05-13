#pragma once

#include "Base.h"

/* 1.�纻 ��ü���� �з��س��� �ϳ��� ����?  */

BEGIN(Engine)

class CGameObject;	
class CComponent;	

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT Add_GameObject(class CGameObject* pGameObject) {
		m_GameObjects.push_back(pGameObject);
		return S_OK;
	}

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	/* 11�� 11�� ���θ���*/
	CComponent* Find_GameObject(const wstring& strComponentName);	

	//���� - UIGroup �����
	HRESULT UIGroup_Render_OnOff(_bool bCheck);
	_bool UIGroup_Render_State();

	HRESULT Delete_GameObjects(const wstring& pLayerTag, _uint iCurrentLevel);

public:
	list<CGameObject*>& Get_GameObject_List() { return m_GameObjects; }	

private:
	list<class CGameObject*>			m_GameObjects;
	class CGameInstance*				m_pGameInstance = { nullptr };

public:
	static CLayer* Create();
	virtual void Free() override;
};

END