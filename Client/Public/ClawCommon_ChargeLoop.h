#pragma once

#include "State.h"

class CClawCommon_ChargeLoop final : public CState
{
private:
	CClawCommon_ChargeLoop();
	virtual ~CClawCommon_ChargeLoop() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CClawCommon_ChargeLoop* Create();
	virtual void Free() override;

};
