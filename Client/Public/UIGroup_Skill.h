#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_Skill_Slot.h"
BEGIN(Engine)
class CUI_Scene;
END

class CPlayerSkillMgr;
BEGIN(Client)

class CUIGroup_Skill final : public CUIObject
{
private:
	CUIGroup_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Skill(const CUIGroup_Skill& Prototype);
	virtual ~CUIGroup_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Slot_Setting();
	void Slot_Update_State();
	void Slot_Update_State_Value(SkILL_SLOTSTATE eSteteNum, CUI_Skill_Slot* pSlotUIObj);

	void Slot_Contion_Check(); // 아이템 조건 체크

	void Skill_Equip_Weapon();

	void SKill_Current_Slot(CUI_Skill_Slot* pSkill, _bool bOpen);

public:
	void Condition_Text_Update(ITEM_TYPE eItemtype, _bool bCheck);

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};

	CUI_Scene* m_pBaseScene = { nullptr };
	//CUI_Scene* m_pEquipWeapon = { nullptr };
	//CUI_Scene* m_pEquipWeapon_2 = { nullptr }; // 슬롯 2개 활성화 되어 있는 경우
	CUI_Scene* m_pEquipCondition = { nullptr };

	CUI_Button* m_pCurrentSkill = { nullptr };
	CUI_Button* m_pEquipSkill_1 = { nullptr };

	CPlayerSkillMgr* m_pPlayerSkillMgr = { nullptr };

	_bool m_bEscape = { false };

public:
	deque<PLAYER_SKILL>& Get_PlayerSkill_List() { return m_deqOpenSkill; }

private:
	LEVELID m_eMyLevelID = {};
	map<_uint, pair<_bool, CUI_Skill_Slot*>> m_mapSlotInfo;
	deque<PLAYER_SKILL> m_deqOpenSkill = {}; // 해금된 스킬들을 vector에 저장한다

public:
	static CUIGroup_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
