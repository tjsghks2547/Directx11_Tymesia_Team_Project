#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "PlayerSkillState.h"	

BEGIN(Engine)
class CGameObject;
END




class CPlayerSkillMgr : public CPlayerSkillState
{
protected:
	CPlayerSkillMgr();
	virtual ~CPlayerSkillMgr() = default;


public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta);
	HRESULT Update_Skill_CoolTime(class CGameObject* pGameObject, _float fTimeDelta);
	virtual void  Late_Update(class CGameObject* pGameObject, _float fTimeDelta);

	HRESULT UnLockSkill(PLAYER_SKILL _eSkill);
	Skill_Information* Bring_Skill_information(PLAYER_SKILL _eSkill);

	vector<CPlayerSkillState*>& Get_VecState() { return m_vecPlayerSkillState; }
private:
	vector<CPlayerSkillState*>  m_vecPlayerSkillState;


public:
	static CPlayerSkillMgr* Create();
	virtual void Free();

};
