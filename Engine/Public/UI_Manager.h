#pragma once
#include "Base.h"


BEGIN(Engine)
class CUIObject;

class  CUI_Manager final : public CBase
{
public:
	struct PLAYER_SAVE_STATE // 플레이어 기본 능력치
	{
		_int iLevel; // 레벨

		_int iPower; // 힘 수치
		_int iVitality; // 활력 수치
		_int iPlague; // 역병 수치

		_int iAttackPower; // 공격 대미지
		_int iAmountOfWounds; // 상처 수
		_int iClawAttackPower; // 발톱 대미지
		_int iFullHP; // 최대 체력 
		_int iFullMP; // 최대 에너지

		_int iTalentPoint; // 특성 포인트
		_int iUnspent; // 미사용 특성 포인트
		
		_int iMemoryFragment; // 캐릭터 보유 기억의 파편 개수         
		_int iMemoryTotalUse; //지금까지 사용한 개수 저장인데...모르겠당     

		_float fBonus_SkillDamage; // 추가 스킬 대미지

	};

	struct PLAYER_SAVE_TALENT // 특성 창에 따라 변화될 값들 = 캐릭터 버프입니다!
	{
		_int iAttackPower_Add;	// 추가 공격대미지량
		_int iExecution_HP_MP_Add;	// 처형 시 추가 체력, 에너지 획득량
		_int iSwordAttack_MP_Add; // 검으로 적 공격 시 추가 에너지 회복량
		_int iParry_MP_Add; // 튕겨내기 성공 시 추가 에너지 회복량

		_bool bLongClawSecondOpen; // 두 번째 긴 발톱 공격 오픈 
		_int iLongClawAttack_Add; // 두 번째 긴 발톱 공격 대미지량 증가
		_int iClawAttack_MP_Add; // 발톱 공격 성공 시 추가 에너지 획득량


		/* 깃털 탭과 전략탭은 추가 안 했음*/
	};

	struct UI_SAVE_TALENT // ui 특성 창 내용 저장
	{
		/*Talent GroupID , 활성화 여부*/
		map<_uint, _bool> mapSlot_LightAttack;	// 특성 : 검 
		map<_uint, _bool> mapSlot_Parry;			// 특성 : 튕겨내기
		map<_uint, _bool> mapSlot_Dodge;			// 특성 : 회피
		map<_uint, _bool> mapSlot_Claw;			// 특성 : 발톱
		map<_uint, _bool> mapSlot_Feather;		// 특성 : 깃털
		map<_uint, _bool> mapSlot_Misc;			// 특성 : 전략

	};
	struct UI_SAVE_SKILL // ui 특성 창 내용 저장
	{
		/*SKILL GroupID , 활성화 여부*/
		map<_uint, _bool> mapSlotInfo;	// 특성 : 검 
		_uint			 iTake_Away_Skill;// 약탈 스킬 저장

	};

private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;


public:
	HRESULT Initialize(_uint iNumScenes);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	void Clear(_uint iScenelIndex); // 특정 UI 씬 지우기
	void Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj); // UI 씬의 특정 오브젝트 삭제
	void Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag);; // UI 씬의 마지막 오브젝트 삭제
	void Clear_ALL(); // UI 모든 씬 지우기
	
public:
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();

public:
	// UIScene 에 UIObj 추가 => A라는 씬에 1,2,3 오브젝트 추가 B라는 씬에 2,3,4 오브젝트 추가 => 씬별로 사용할 오브젝트들만 집어 넣는 방식
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	
	_bool Get_Scene_Render_State(CUI_Scene* pScene); // A라는 씬 자체가 그려지고 있는지 확인
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene,_bool bOpen); // A라는 씬에 있는 모든 UIObj의 랜더(그리기)를 켜고 끄기
	HRESULT Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen); // UIObj를 그리고 있지만 화면에 끄고 켜고를 설정
	void Find_TextBox_PlayerScreen(CUI_Scene* pScene, _uint iGroupID, _uint iCount); // 몬스터 사망 시 수치 가져오는 부분을 위해...일단 추가 

	void Set_Condition(CUI_Scene* pScene, _int iCondition1, _int iCondition2);// { m_iCondition1 = iCondition1, m_iCondition2 = iCondition2; }
	_int Get_Condition(CUI_Scene* pScene, _uint iConditionNum);// { return m_iCondition[iConditionNum]; }

	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName); // UI 오브젝트 데이터 로드
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut); // 텍스트 데이터 로드

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