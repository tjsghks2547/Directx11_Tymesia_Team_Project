#pragma once

#include "PlayerSkillState.h"

class CSkillHalberd : public CPlayerSkillState
{
private:
	CSkillHalberd();
	virtual ~CSkillHalberd() = default;

public:
	HRESULT Initialize();

	virtual _bool Priority_Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, _float fTimeDelta) override;

public:
	static CSkillHalberd* Create();
	virtual void Free() override;

};

