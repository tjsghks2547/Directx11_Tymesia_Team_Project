#pragma once
#include "Base.h"


BEGIN(Engine)
class CUIObject;

class  CUI_Manager final : public CBase
{
public:
	struct PLAYER_SAVE_STATE // �÷��̾� �⺻ �ɷ�ġ
	{
		_int iLevel; // ����

		_int iPower; // �� ��ġ
		_int iVitality; // Ȱ�� ��ġ
		_int iPlague; // ���� ��ġ

		_int iAttackPower; // ���� �����
		_int iAmountOfWounds; // ��ó ��
		_int iClawAttackPower; // ���� �����
		_int iFullHP; // �ִ� ü�� 
		_int iFullMP; // �ִ� ������

		_int iTalentPoint; // Ư�� ����Ʈ
		_int iUnspent; // �̻�� Ư�� ����Ʈ
		
		_int iMemoryFragment; // ĳ���� ���� ����� ���� ����         
		_int iMemoryTotalUse; //���ݱ��� ����� ���� �����ε�...�𸣰ڴ�     

		_float fBonus_SkillDamage; // �߰� ��ų �����

	};

	struct PLAYER_SAVE_TALENT // Ư�� â�� ���� ��ȭ�� ���� = ĳ���� �����Դϴ�!
	{
		_int iAttackPower_Add;	// �߰� ���ݴ������
		_int iExecution_HP_MP_Add;	// ó�� �� �߰� ü��, ������ ȹ�淮
		_int iSwordAttack_MP_Add; // ������ �� ���� �� �߰� ������ ȸ����
		_int iParry_MP_Add; // ƨ�ܳ��� ���� �� �߰� ������ ȸ����

		_bool bLongClawSecondOpen; // �� ��° �� ���� ���� ���� 
		_int iLongClawAttack_Add; // �� ��° �� ���� ���� ������� ����
		_int iClawAttack_MP_Add; // ���� ���� ���� �� �߰� ������ ȹ�淮


		/* ���� �ǰ� �������� �߰� �� ����*/
	};

	struct UI_SAVE_TALENT // ui Ư�� â ���� ����
	{
		/*Talent GroupID , Ȱ��ȭ ����*/
		map<_uint, _bool> mapSlot_LightAttack;	// Ư�� : �� 
		map<_uint, _bool> mapSlot_Parry;			// Ư�� : ƨ�ܳ���
		map<_uint, _bool> mapSlot_Dodge;			// Ư�� : ȸ��
		map<_uint, _bool> mapSlot_Claw;			// Ư�� : ����
		map<_uint, _bool> mapSlot_Feather;		// Ư�� : ����
		map<_uint, _bool> mapSlot_Misc;			// Ư�� : ����

	};
	struct UI_SAVE_SKILL // ui Ư�� â ���� ����
	{
		/*SKILL GroupID , Ȱ��ȭ ����*/
		map<_uint, _bool> mapSlotInfo;	// Ư�� : �� 
		_uint			 iTake_Away_Skill;// ��Ż ��ų ����

	};

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;


public:
	HRESULT Initialize(_uint iNumScenes);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	void Clear(_uint iScenelIndex); // Ư�� UI �� �����
	void Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj); // UI ���� Ư�� ������Ʈ ����
	void Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag);; // UI ���� ������ ������Ʈ ����
	void Clear_ALL(); // UI ��� �� �����
	
public:
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();

public:
	// UIScene �� UIObj �߰� => A��� ���� 1,2,3 ������Ʈ �߰� B��� ���� 2,3,4 ������Ʈ �߰� => ������ ����� ������Ʈ�鸸 ���� �ִ� ���
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	
	_bool Get_Scene_Render_State(CUI_Scene* pScene); // A��� �� ��ü�� �׷����� �ִ��� Ȯ��
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene,_bool bOpen); // A��� ���� �ִ� ��� UIObj�� ����(�׸���)�� �Ѱ� ����
	HRESULT Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen); // UIObj�� �׸��� ������ ȭ�鿡 ���� �Ѱ� ����
	void Find_TextBox_PlayerScreen(CUI_Scene* pScene, _uint iGroupID, _uint iCount); // ���� ��� �� ��ġ �������� �κ��� ����...�ϴ� �߰� 

	void Set_Condition(CUI_Scene* pScene, _int iCondition1, _int iCondition2);// { m_iCondition1 = iCondition1, m_iCondition2 = iCondition2; }
	_int Get_Condition(CUI_Scene* pScene, _uint iConditionNum);// { return m_iCondition[iConditionNum]; }

	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName); // UI ������Ʈ ������ �ε�
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut); // �ؽ�Ʈ ������ �ε�

public:
	const PLAYER_SAVE_STATE& Get_Player_State_SaveData() const { return m_PlayerStat; }
	void Set_Player_State_SaveData(const PLAYER_SAVE_STATE& PlayerData) { m_PlayerStat = PlayerData; }

	const PLAYER_SAVE_TALENT& Get_Player_Talent_SaveData() const { return m_PlayerTalent; }
	void Set_Player_Talent_SaveData(const PLAYER_SAVE_TALENT& PlayerData) { m_PlayerTalent = PlayerData; }
	
	const UI_SAVE_TALENT& Get_UI_Talent_SaveData() const { return m_UITalent; }
	void Set_UI_Talent_SaveData(const UI_SAVE_TALENT& PlayerData) { m_UITalent = PlayerData; }

	const UI_SAVE_SKILL& Get_UI_Skill_SaveData() const { return m_UISkill; }
	void Set_UI_Skill_SaveData(const UI_SAVE_SKILL& PlayerData) { m_UISkill = PlayerData; }

private:

	PLAYER_SAVE_STATE m_PlayerStat = {};
	PLAYER_SAVE_TALENT m_PlayerTalent = {};
	UI_SAVE_TALENT m_UITalent = {};
	UI_SAVE_SKILL  m_UISkill = {};

private:
	map<const _wstring, class CUI_Scene*>*	m_pScenes = { nullptr };
	_uint									m_iNumScenes = {};

	class CGameInstance*					m_pGameInstance = { nullptr };



public:
	static CUI_Manager* Create(_uint iNumScenes);
	virtual void Free() override;

};

END