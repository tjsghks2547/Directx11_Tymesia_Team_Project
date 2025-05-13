#pragma once

#include "Base.h"

/* 1.사본 객체들을 분류해놓은 하나의 묶음?  */

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

	/* 11월 11일 새로만듬*/
	CComponent* Find_GameObject(const wstring& strComponentName);	

	//유빈 - UIGroup 제어용
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