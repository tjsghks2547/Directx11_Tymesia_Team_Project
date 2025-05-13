#pragma once

#include "State.h"

class CClawCommon_ChargeFull final : public CState
{
private:
	CClawCommon_ChargeFull();
	virtual ~CClawCommon_ChargeFull() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CClawCommon_ChargeFull* Create();
	virtual void Free() override;

};
