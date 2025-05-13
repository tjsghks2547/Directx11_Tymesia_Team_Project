#pragma once

#include "PlayerSkillState.h"

class CSkillGreadSword : public CPlayerSkillState
{
private:
	CSkillGreadSword();
	virtual ~CSkillGreadSword() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;

public:
	static CSkillGreadSword* Create();
	virtual void Free() override;

};

