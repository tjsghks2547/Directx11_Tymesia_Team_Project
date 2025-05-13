#pragma once

#include "Base.h"
#include "Item.h"

BEGIN(Engine)
class CBase;
class CGameInstance;

class ENGINE_DLL CItemMgr : public CBase
{
private:
	CItemMgr();
	virtual ~CItemMgr() = default;


public:
	HRESULT Initialize();
	void	Update(_float _fTimeDelta);

	HRESULT Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, CItem* _pGameObject, _bool _bTaken);
	HRESULT Sub_Item(ITEM_TYPE _eItemType, const _wstring& _ItemName);

	void	Set_ItemPos(_fvector _vItemWorldPos);
	HRESULT Acquire_Item(ITEM_TYPE _eItemType);
	HRESULT	Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _GameObject, _uint _iDropItemCount = 1);
	HRESULT	Pop_Item(ITEM_TYPE _eItemType, _fvector _vPopPosition, class CGameObject* _GameObject, _uint _iPopItemCount);

	void Clear_ItemInfo();

	_uint Get_Item_Count(ITEM_TYPE _eItemType) { return m_mapItems[_eItemType].first; }
	
	list<ITEM_TYPE>&	 Get_Item_Save_Info() { return m_lSaveItem;} // ���� �������� ȹ���ϴ� ���� ȹ�� �˸��� ���� ���� �뵵
	list<ITEM_TYPE>&	 Get_Item_Drop_Info() { return m_lDropItem;} // ���� �������� ������ ���� ���� �˸��� ���� ���� �뵵

	const map<ITEM_TYPE, pair<_uint, vector<CItem*>>> Get_Item_Info() { return m_mapItems; } // �������� ����Ǵ� �����̳� �б� �������� ��ȯ  - ���� ui���� ���
	_bool Use_Item(ITEM_TYPE _eItemType,_uint iNum); // �κ��丮���� ������ ��� �� �����̳ʿ� �����ϰ� �ִ� ������ ���� ���� - ����
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	vector<CItem*>* Find_ItemVector(ITEM_TYPE _eItemType);

private:
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>>		m_mapItems;
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>>		m_mapTakenItems;
	list<ITEM_TYPE>									m_lSaveItem;
	list<ITEM_TYPE>									m_lDropItem;
	ITEM_TYPE										m_eItemType = { ITEM_TYPE::ITEM_END };
	_uint											m_eItemCount[(_uint)ITEM_TYPE::ITEM_END] = {};
public:
	static CItemMgr* Create();
	virtual void Free() override;

};
END
