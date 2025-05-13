#pragma once

#include "State.h"

class CAttack_LongClaw_2 final : public CState
{
private:
	CAttack_LongClaw_2();
	virtual ~CAttack_LongClaw_2() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CAttack_LongClaw_2* Create();
	virtual void Free() override;

};
