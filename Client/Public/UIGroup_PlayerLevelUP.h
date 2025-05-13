#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_PlayerLevelUP final : public CUIObject
{
private:
	CUIGroup_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerLevelUP(const CUIGroup_PlayerLevelUP& Prototype);
	virtual ~CUIGroup_PlayerLevelUP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Button_Input_Check(); // 버튼 누를 시 발생 이벤트
	HRESULT Button_Render_Check(); // 버튼이 그려지는 조건
	HRESULT Button_Image_On_Check(); // 연출 - 어떤 정보가 달라지는 지 보여주는 부분 
	HRESULT Button_Image_OnOff_Check(_bool bPower, _bool bVitality, _bool bPlaque); // 연출 - 어떤 정보가 달라지는 지 보여주는 부분 
	HRESULT LevelUP_Memory_Check();
	HRESULT LevelUP_Status_Check();
	HRESULT LevelUP_Status_Info_Check();
	HRESULT LevelUP_TalentPoint_Check();

public:

	HRESULT LevelUP_Apply_Button(); // 적용 팝업안의 버튼
	HRESULT LevelUP_Reset_Button(); // 리셋 팝업안의 버튼

	HRESULT LevelUP_Apply(); // 적용
	HRESULT LevelUP_Reset(); // 리셋

private:
	_bool m_bApplyOn = false;
	_bool m_bResetOn = false;
	_bool m_bEscapeCheck = false;


private:
	// 레벨
	_uint m_iCurrentLevel = {};
	_uint m_iNextLevel = {};

	// 기억 파편
	_int m_iMemoryCurrentCount = {}; // 유저가 갖고 있는 기억의 파편 개수
	_int m_iMemoryNextCount = {}; // 뺄셈해서 보여줄 => 레벨업 진행 시 남는 기억의 파편 개수

	_int m_iCurrentMemoryNeed = {}; // 다음 레벨 업에 필요한 기억의 파편 수를 받아 옴
	_int m_iNextMemoryNeed = {}; // 다음 레벨 업에 필요한 기억의 파편 수를 받아 옴

	_int m_iMemoryTotalUse = { 0 }; //지금까지 플레이어가 레벨업에 사용한 기억 파편 개수를 저장함


	// 캐릭터 능력치 변수
	_uint m_iCurrentPower = {}; // 힘 수치
	_uint m_iNextPower = {}; // 힘 수치

	_uint m_iCurrentVitality = {}; // 활력 수치
	_uint m_iNextVitality = {}; // 활력 수치

	_uint m_iCurrentPlague = {}; // 역병 수치
	_uint m_iNextPlague = {}; // 역병 수치

	// 캐릭터 상태 변수
	_uint		m_iCurrentAttackPower = {}; // 공격 대미지 
	_uint		m_iNextAttackPower = {}; // 공격 대미지

	_uint		m_iCurrentAmountOfWounds = {}; // 상처 수
	_uint		m_iNextAmountOfWounds = {}; // 상처 수

	_uint		m_iCurrentClawAttackPower = {}; // 발톱대미지
	_uint		m_iNextClawAttackPower = {}; // 발톱대미지

	_uint		m_iCurrentFullHp = {}; // 체력
	_uint		m_iNextFullHp = {}; // 체력

	_uint		m_iCurrentFullMp = {}; // 에너지
	_uint		m_iNextFullMp = {}; // 에너지

	// 플레이어 스킬들 공격력 상승 설정을 위한 값
	_float     m_fCurrentBonus_SkillDamage = {};
	_float     m_fNextBonus_SkillDamage = {};

	// 특성 변수
	_uint       m_iCurrentTalentPoint = {};
	_uint       m_iNextTalentPoint = {};


public:
	void Set_TalentPoint_ALlUnspent(_int iPoint) { m_iCurrentUnspent = iPoint, m_iNextUnspent = iPoint; }
	_int Get_TalentPoint_CurrentUnspent() {return  m_iCurrentUnspent; }

private:
	_int       m_iCurrentUnspent = {};
	_int       m_iNextUnspent = {};

	LEVELID		m_eMyLevelID = {};

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	void Setting_Default_Status_Value(); // 최초, 나중에 플레이어가 초기화 하는 경우 설정되는 값

private:
	CUI_Scene* m_pMyScene = {}; //레벨 업 ui 그룹
	class CGameObject* m_pPlayer = { nullptr };


	CUI_Scene* m_pApplyPopUp = {}; // 1 번 팝업 적용
	CUI_Scene* m_pResetPopUp = {}; // 2 번 팝업 초기화(완전 초기화가 아닌 유저가 임의로 올린 수치 값들을 리셋)

private:
	vector<UI_TextInfo> m_TextInfo = {};

public:
	static CUIGroup_PlayerLevelUP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END