#pragma once

#include "State.h"


class CRock_On_Run_B final : public CState
{
private:
	CRock_On_Run_B();
	virtual ~CRock_On_Run_B() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.05f };

public:
	static  CRock_On_Run_B* Create();
	virtual void Free() override;

};
