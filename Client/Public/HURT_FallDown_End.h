#pragma once

#include "State.h"


class CHURT_FallDown_End final : public CState
{
private:
	CHURT_FallDown_End();
	virtual ~CHURT_FallDown_End() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CHURT_FallDown_End* Create();
	virtual void Free() override;

};
