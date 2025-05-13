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
	HRESULT Ready_UIObject(); // 인벤용 ui 로드
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	void Set_Item_Default_Info();

public:
	void Ready_MiniView_ItemInfo(); // 반고정 내용 바뀌는 텍스트들 그냥 편하게 가지고 있기
	void Connect_MiniView_ItemInfo(UI_Item ItemInfo); // 미리 준바 해둔 텍스트 박스 정보에 연결
	void DisConnect_MiniView_ItemInfo(); // 아이템 선택 없을 시 빈 공간 출력

public:
	void Change_UI_Item_Tab(); // 아이템 메뉴 탭

public:
	void Update_Get_ItemMgr(); // 아이템 매니저에서 가져온 정보를 인벤용으로 정보를 가공해서 저장 (구조체)
	void Update_ItemInfo(); // 인벤용으로 정보를 저장한 컨테이너 안에 아이템 타입에 따른 정보 집어넣기

	void Itme_View_Inventory(vector<UI_Item>& vecContainer); // 저장된 컨테이너들을 토대로 인벤토리 슬롯에 정보 넣기

public:
	void Slot_Button_Select_Check(vector<UI_Item>& vecContainer); // 인벤토리 슬롯을 마우스 선택 했을 때의 반응
	void Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer);// 인벤토리 슬롯에 마우스를 올렸을 때의 반응

public:
	void ItemType_PopUP_Open(ITEM_TYPE eItemType, _float3 fSetPos); // 팝업을 오픈할 위치
	void ItemType_PopUP_State(_bool bOpen); // 현재 팝업의 끄고 켜고 처리
	void ItemType_PopUP_Button(); //팝업에 있는 버튼 눌렀을 때의 처리

	void ItemUse_PopUP_Open(); // 2번 팝업 오픈 => 어떤 팝업을 부를지 설정
	void ItemUse_PopUP_Use_Button(); // 아이템 사용 팝업 설정 
	void ItemUse_PopUP_Drop_Button();// 아이템 버리기 팝업 설정
	void ItemUse_Update(); // 아이템을 버리거나 사용 시 인벤 정보 업데이트 => 아이템 매니저를 다시 받아오는게 아니어서 문제가 발생할 것 같음

public:
	void Item_Use_Set(ITEM_TYPE etype, _uint iCount); // 사용한 아이템에 따라 기능 구현

private:
	CGameObject* m_pItemTextBoxName = {};		//200 이름
	CGameObject* m_pItemTextBoxDesc = {};		//230 설명
	CGameObject* m_pItemTextBoxType = {};		//210 타입 텍스트
	CGameObject* m_pItemTextBoxCount = {};		//220 수량 텍스트
	CGameObject* m_pItemTextBoxCountNum = {};	//221 n/n 설정

private:
	_bool m_bCommonOpen = { true }; // 일반 탭 오픈
	_bool m_bSkillOpen = { false }; // 기술의 파편 탭 오픈
	_bool m_bHerbOpen = { false }; // 재료 탭 오픈
	_bool m_bStoryOpen = { false }; // 이야기 탭 오픈

private:
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>> m_ItemMgrContainerRef; // 아이템 매니저에 적재되어 있는 아이템 정보
	vector<UI_Item> m_InvenItemCommon = {}; // 일반 아이템 저장
	vector<UI_Item> m_InvenItemSkill = {}; // 스킬 아이템 저장
	vector<UI_Item> m_InvenItemStory = {}; // 이야기 아이템 저장
	vector<UI_Item> m_InvenItemHerb = {}; // 허브 아이템 저장

private:
	_uint m_iMouseOnLastSlot = {}; // 마지막으로 마우스가 가리킨 슬롯의 그룹 아이디

private:
	_bool m_bItemUsePopOpen = { false }; // 2번째 팝업이 켜진 경우 팝업을 종료하기 전까지는 다른 작업을 하지 못함
	_bool m_bItemTypePopOpen = { false }; // 1번째 팝업이 켜진 경우 슬롯 정보를 다시 받아오지 않음

	_uint m_iItemStatCount = { 1 };
	_uint m_iItemMaxCount = {};

	UI_Item m_CurrentItemInfo = {};

	_uint m_iPopUpOpenNum = {};
private:
	CUI_Scene* m_pMyBaseScene = {}; // Base 배경
	CUI_Scene* m_pItemScene = {}; // UI슬롯
	CUI_Scene* m_pItemTypePopUp = {}; // 1 번 팝업
	CUI_Scene* m_pItemUsePopUp = {}; // 2 번 팝업
	class CGameObject* m_pPlayer = { nullptr };
	LEVELID m_eMyLevel = {};

public:
	static CUIGroup_Inventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

public:
	const vector<UI_Item> Get_Vector_Itme_default_Info() { return m_vecItemDefaultInfo; } // 내 정보 읽어가

private:
	map<ITEM_TYPE, UI_Item> m_mapDropItemInfo; // 버릴 아이템 정보 저장 => 플레이어 스크린에서 아이템 알림 할 용도
	vector<UI_Item> m_vecItemDefaultInfo; // 아이템 기본 정보
	CGameObject* m_pGroupPlayerScreen = { nullptr };


};

END

