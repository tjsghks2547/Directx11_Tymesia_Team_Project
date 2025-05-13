#pragma once

#include "PlayerSkillState.h"

class CSkillScythe : public CPlayerSkillState
{
private:
	CSkillScythe();
	virtual ~CSkillScythe() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;

public:
	static CSkillScythe* Create();
	virtual void Free() override;

};

