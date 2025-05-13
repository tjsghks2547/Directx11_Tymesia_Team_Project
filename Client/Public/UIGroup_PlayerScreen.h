#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
class CUI_Text;
END


class CPlayerSkillMgr;
BEGIN(Client)

class CUIGroup_PlayerScreen final : public CUIObject
{
	enum ITEM_STATE{ITEM_SAVE,ITEM_DROP,ITEM_END};
private:
	CUIGroup_PlayerScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerScreen(const CUIGroup_PlayerScreen& Prototype);
	virtual ~CUIGroup_PlayerScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Player_Info_GageBar();
	void UI_Direction_HPBar();
	void UI_Direction_MPBar();
	void UI_Player_Skill_ImageSet();

private:
	CUIObject*				m_pHPGageBar = { nullptr };
	CUIObject*				m_pHPGageTrack = { nullptr };

	CUIObject*				m_pMPGageBar = { nullptr };
	CPlayerSkillMgr*		m_pPlayerSkillMgr = { nullptr };

public:
	void Item_Nudge_Info(ITEM_TYPE eItemType, ITEM_STATE eState, _int iNum); // 아이템 획득 시 출력되는 알림
	
	void Item_Nudge_Check(_float fTimeDelta);
	void Item_In_Out_Pop();

public:
	void UI_Direction_Monster_MemoryGet();

public:
	HRESULT Ready_UIObject();
	void Ready_Skill_Slot();
	void Ready_Player_GageBar();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);

public:
	void Button_Skill(); // 스킬 버튼 입력 시 연출(임시?)

private:
	CGameObject*			m_pGroupSkill = { nullptr };
	
	CUIObject*				m_pPlunderSkill = { nullptr };
	CUIObject*				m_pRevolvingSkill_1 = { nullptr };
	CUIObject*				m_pRevolvingSkill_2= { nullptr };
	CUIObject*				m_pEffectSkill_2= { nullptr };
	CUIObject*				m_pRevolvingSkill_3 = { nullptr };

	_bool                   m_bSkillUse_Fix = { false };
	_bool                   m_bSkillUSe_Plunder = { false };
	_bool*					m_pPlayerSkillCoolTime = { nullptr };
private:
	CGameObject*			m_pGroupInven = { nullptr };
	CUI_Scene*				m_pItmeScreen = {}; //아이템 획득, 버리기 팝업
	_bool					m_bNudgeUse[4] = { false,false,false,false }; // 아이템 알림용
	_float					m_fNudgeSpeed = 5.0f;

private:
	_float					m_fTimeCheck[4] = {};
	LEVELID					m_eMyLevelID = {};
	CUI_Scene*				m_pMyScene = {}; //플레이어 화면
	class CGameObject*		m_pPlayer = { nullptr };

private:
	CUI_Text*				m_pMonsterText = {};
	_float					m_fMonsterTextOnTime = {};
	vector<UI_TextInfo>		m_TextInfo = {};



	CUI_Text*				m_pPlayerHPText = {};
	CUI_Text*				m_pPlayerMPText = {};
	_float					m_fHPTextOnTime = {};
	_float					m_fMPTextOnTime = {};

	
	_int*					m_pPotion_Count = {};
public:
	void Set_QuestCount_UP(_int iCount) { m_iQuestMonsterDeadCount += iCount; }
	_int Get_QuestCount() { return m_iQuestMonsterDeadCount; }
private:
	CUI_Scene*				m_pPoisonScreen = {}; //독 퀘스트 화면
	CUIObject*				m_pPoisonQuestDesc = { nullptr };
	_int                    m_iQuestMonsterDeadCount = {};

public:
	static CUIGroup_PlayerScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

