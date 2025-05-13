
#pragma once

#include "PlayerSkillState.h"

class CSkillCane : public CPlayerSkillState
{
private:
	CSkillCane();
	virtual ~CSkillCane() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;

public:
	static CSkillCane* Create();
	virtual void Free() override;

};

