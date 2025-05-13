#pragma once

#include "State.h"

class CClawCommon_ChargeStart final : public CState
{
private:
	CClawCommon_ChargeStart();
	virtual ~CClawCommon_ChargeStart() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CClawCommon_ChargeStart* Create();
	virtual void Free() override;

};
