#pragma once

#include "Base.h"
#include "Event.h"

BEGIN(Engine)
class CGameInstance; 

class CEventMgr : public CBase
{
private:
	CEventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEventMgr() = default; 

private:
	HRESULT  Initialize();

public:
	/* �̺�Ʈ ���� ��ü�� �־��� �� ���*/
	HRESULT  Add_EventObject(_wstring _EventName, CGameObject* pGaemObject);	
	HRESULT  Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject);	

	/* �̺�Ʈ ���� ��ü���� Ʈ���Ű� �߻��� �� �߻� */
	HRESULT  Event_Activate(_wstring _EventName);	

	/* ��ü�� ������ ������ ���õ� �Լ��� */
	HRESULT  Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject);
	HRESULT  Add_DeadObjects(_wstring _LayerName, CGameObject* pGaemObject, _uint iCurrenteLevel);

	HRESULT  Add_DeadEffect(CGameObject* pGameObject);	

	HRESULT  Update();

	void  Set_Level(_uint _iCurLevel) { m_iCurrent_Level = _iCurLevel; }
private:
	ID3D11Device*        m_pDevice       = { nullptr };
	ID3D11DeviceContext* m_pContext      = { nullptr };
	CGameInstance*       m_pGameInstance = { nullptr };

private:
	map<_wstring, CEvent*>				m_mapEvent; 
	map<_wstring, CGameObject*>			m_mapDeadObejct;
	multimap<_wstring, CGameObject*>	m_mapDeadObejcts;
	vector<CGameObject*>				m_vecDeadEffect; 

private:
	_uint m_iCurrent_Level = {};

public:
	static CEventMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override; 
		 
};

END	