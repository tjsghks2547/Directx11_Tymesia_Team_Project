#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_Frame.h"
BEGIN(Engine)
class CUI_Scene;
class CUI_Text;
END

BEGIN(Client)

class CUIGroup_PlayerTalent final : public CUIObject
{
public:
	enum TABSTATE { TAB_LIGHTATTACK, TAB_PARRY, TAB_DODGE, TAB_CLAW, TAB_FEATHER, TAB_MISC, TAB_END };

private:
	CUIGroup_PlayerTalent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerTalent(const CUIGroup_PlayerTalent& Prototype);
	virtual ~CUIGroup_PlayerTalent() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName, vector<UI_TextInfo>& vecText);

public:
	void Talent_Tab_Change();
	void Talent_Info_Setting();
	void Find_Text_Info(TABSTATE eTab);
	void Talent_Mouse_Check(TABSTATE eTab);

public:
	void Player_Talent_Update();
	void Slot_Setting();
	void Slot_Update_State(TABSTATE eTab); //
	void Slot_Update_State_Value(SLOTSTATE eSteteNum, CUI_Frame* pSlotUIObj );
private:
	CGameObject* m_pGroupLevelUp = { nullptr };
	LEVELID		m_eMyLevelID = {};
	class CGameObject* m_pPlayer = { nullptr };

	_int m_iTalentPoint = {0};
	_bool m_bNotFindConditionID = false;
private:
	CUIObject* m_pText_TalentPoint = {}; 
	CUIObject* m_pText_TalentName = {};  
	CUIObject* m_pText_TalentDesc = {};  
	CUIObject* m_pText_TalentOpen = {};  
	//CUIObject* m_pImage_Frame = {};  
	CUIObject* m_pImage_Desc = {};  

private:
	TABSTATE m_eTabCheck = { TAB_LIGHTATTACK };
	CUIObject* m_pCurrentTalent = { nullptr };
	const _tchar* m_CurrentName = {};
	const _tchar* m_CurrentDesc = {};

private:
	CUI_Scene* m_pMySceneBase = {}; // ±âº» °ø¿ë 
	CUI_Scene* m_pSlot_LightAttack = {}; //°Ë 
	CUI_Scene* m_pSlot_Parry = {}; //Æ¨°Ü³»±â 
	CUI_Scene* m_pSlot_Dodge = {}; //È¸ÇÇ
	CUI_Scene* m_pSlot_Claw = {}; //¹ßÅé
	CUI_Scene* m_pSlot_Feather = {}; //±êÅÐ
	CUI_Scene* m_pSlot_Misc = {}; //Àü·«
	_bool m_bEscape = { false };

private:
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_LightAttack;
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_Parry;
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_Dodge;
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_Claw;
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_Feather;
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_Misc;

private:
	vector<UI_TextInfo> m_LightAttack_Text = {};
	vector<UI_TextInfo> m_ParryText= {};
	vector<UI_TextInfo> m_DodgeText = {};
	vector<UI_TextInfo> m_ClawText = {};
	vector<UI_TextInfo> m_FeatherText = {};
	vector<UI_TextInfo> m_MiscText = {};

public:
	static CUIGroup_PlayerTalent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END