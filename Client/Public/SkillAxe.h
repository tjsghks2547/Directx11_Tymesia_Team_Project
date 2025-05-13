#pragma once

#include "PlayerSkillState.h"

class CSkillAxe : public CPlayerSkillState
{
private:
	CSkillAxe();
	virtual ~CSkillAxe() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;


public:
	static CSkillAxe* Create();
	virtual void Free() override;

};

