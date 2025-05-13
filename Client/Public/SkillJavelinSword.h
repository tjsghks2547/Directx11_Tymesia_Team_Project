#pragma once

#include "PlayerSkillState.h"

class CSkillJavelinSword : public CPlayerSkillState
{
private:
	CSkillJavelinSword();
	virtual ~CSkillJavelinSword() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;

public:
	static CSkillJavelinSword* Create();
	virtual void Free() override;

};

