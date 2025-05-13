#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

class CPlayerSkillState abstract : public CBase
{
protected:
	//struct Skill_Information
	//{
	//	_float* fSkillCoolTime = {};	        // 전체 스킬 쿨 타임 시간 
	//	_float* fSkillCurrentCoolTime = {};	    // 현재 스킬 쿨 타임 지난 시간 
	//	_bool*  bSkill_Unlocked = {};	        // 스킬이 해금 되었는 지 안 되었는 지 확인 
	//	_bool*  bSkill_CanUse = {};		        // 스킬이 현재 쿨타임이 지나 사용이 가능 한 지, 안 한지 체크. 
	//	PLAYER_SKILL* eSkill_Name = {};         // 스킬 이름 
	//};

protected:
	CPlayerSkillState();
	virtual ~CPlayerSkillState() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta);
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta);
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta);

	PLAYER_SKILL       Get_Skill_Name() { return m_eSkill_Name; }
	void               Set_Skill_UnLock(_bool _OnOff) { m_bSkill_Unlocked = _OnOff; }
	Skill_Information* Get_Skill_Information() { return m_pSkill_information; }
	_bool			   Check_UnLocked() { return m_bSkill_Unlocked; }	

protected:

	Skill_Information* m_pSkill_information = { nullptr };

	PLAYER_SKILL  m_eSkill_Name = PLAYER_SKILL::PLAYER_SKILL_START;
	_float		  m_fSkill_CoolTime = { 2.f };
	_float		  m_fSkill_CurrentCoolTime = { 0.f };
	_bool		  m_bSkill_Unlocked = { false };
	_bool		  m_bSkill_CanUse = { true };


public:
	virtual void Free() override;

};
