#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
class CItem;
//class CUI_Text;
END

BEGIN(Client)

class CUIGroup_Inventory final : public CUIObject
{
private:
	CUIGroup_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Inventory(const CUIGroup_Inventory& Prototype);
	virtual ~CUIGroup_Inventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject(); // �κ��� ui �ε�
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	void Set_Item_Default_Info();

public:
	void Ready_MiniView_ItemInfo(); // �ݰ��� ���� �ٲ�� �ؽ�Ʈ�� �׳� ���ϰ� ������ �ֱ�
	void Connect_MiniView_ItemInfo(UI_Item ItemInfo); // �̸� �ع� �ص� �ؽ�Ʈ �ڽ� ������ ����
	void DisConnect_MiniView_ItemInfo(); // ������ ���� ���� �� �� ���� ���

public:
	void Change_UI_Item_Tab(); // ������ �޴� ��

public:
	void Update_Get_ItemMgr(); // ������ �Ŵ������� ������ ������ �κ������� ������ �����ؼ� ���� (����ü)
	void Update_ItemInfo(); // �κ������� ������ ������ �����̳� �ȿ� ������ Ÿ�Կ� ���� ���� ����ֱ�

	void Itme_View_Inventory(vector<UI_Item>& vecContainer); // ����� �����̳ʵ��� ���� �κ��丮 ���Կ� ���� �ֱ�

public:
	void Slot_Button_Select_Check(vector<UI_Item>& vecContainer); // �κ��丮 ������ ���콺 ���� ���� ���� ����
	void Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer);// �κ��丮 ���Կ� ���콺�� �÷��� ���� ����

public:
	void ItemType_PopUP_Open(ITEM_TYPE eItemType, _float3 fSetPos); // �˾��� ������ ��ġ
	void ItemType_PopUP_State(_bool bOpen); // ���� �˾��� ���� �Ѱ� ó��
	void ItemType_PopUP_Button(); //�˾��� �ִ� ��ư ������ ���� ó��

	void ItemUse_PopUP_Open(); // 2�� �˾� ���� => � �˾��� �θ��� ����
	void ItemUse_PopUP_Use_Button(); // ������ ��� �˾� ���� 
	void ItemUse_PopUP_Drop_Button();// ������ ������ �˾� ����
	void ItemUse_Update(); // �������� �����ų� ��� �� �κ� ���� ������Ʈ => ������ �Ŵ����� �ٽ� �޾ƿ��°� �ƴϾ ������ �߻��� �� ����

public:
	void Item_Use_Set(ITEM_TYPE etype, _uint iCount); // ����� �����ۿ� ���� ��� ����

private:
	CGameObject* m_pItemTextBoxName = {};		//200 �̸�
	CGameObject* m_pItemTextBoxDesc = {};		//230 ����
	CGameObject* m_pItemTextBoxType = {};		//210 Ÿ�� �ؽ�Ʈ
	CGameObject* m_pItemTextBoxCount = {};		//220 ���� �ؽ�Ʈ
	CGameObject* m_pItemTextBoxCountNum = {};	//221 n/n ����

private:
	_bool m_bCommonOpen = { true }; // �Ϲ� �� ����
	_bool m_bSkillOpen = { false }; // ����� ���� �� ����
	_bool m_bHerbOpen = { false }; // ��� �� ����
	_bool m_bStoryOpen = { false }; // �̾߱� �� ����

private:
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>> m_ItemMgrContainerRef; // ������ �Ŵ����� ����Ǿ� �ִ� ������ ����
	vector<UI_Item> m_InvenItemCommon = {}; // �Ϲ� ������ ����
	vector<UI_Item> m_InvenItemSkill = {}; // ��ų ������ ����
	vector<UI_Item> m_InvenItemStory = {}; // �̾߱� ������ ����
	vector<UI_Item> m_InvenItemHerb = {}; // ��� ������ ����

private:
	_uint m_iMouseOnLastSlot = {}; // ���������� ���콺�� ����Ų ������ �׷� ���̵�

private:
	_bool m_bItemUsePopOpen = { false }; // 2��° �˾��� ���� ��� �˾��� �����ϱ� �������� �ٸ� �۾��� ���� ����
	_bool m_bItemTypePopOpen = { false }; // 1��° �˾��� ���� ��� ���� ������ �ٽ� �޾ƿ��� ����

	_uint m_iItemStatCount = { 1 };
	_uint m_iItemMaxCount = {};

	UI_Item m_CurrentItemInfo = {};

	_uint m_iPopUpOpenNum = {};
private:
	CUI_Scene* m_pMyBaseScene = {}; // Base ���
	CUI_Scene* m_pItemScene = {}; // UI����
	CUI_Scene* m_pItemTypePopUp = {}; // 1 �� �˾�
	CUI_Scene* m_pItemUsePopUp = {}; // 2 �� �˾�
	class CGameObject* m_pPlayer = { nullptr };
	LEVELID m_eMyLevel = {};

public:
	static CUIGroup_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	const vector<UI_Item> Get_Vector_Itme_default_Info() { return m_vecItemDefaultInfo; } // �� ���� �о

private:
	map<ITEM_TYPE, UI_Item> m_mapDropItemInfo; // ���� ������ ���� ���� => �÷��̾� ��ũ������ ������ �˸� �� �뵵
	vector<UI_Item> m_vecItemDefaultInfo; // ������ �⺻ ����
	CGameObject* m_pGroupPlayerScreen = { nullptr };


};

END

