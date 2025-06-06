#pragma once

#include "State.h"


class CPlayerSprint final : public CState
{
private:
	CPlayerSprint();
	virtual ~CPlayerSprint() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.1f };

public:
	static  CPlayerSprint* Create();
	virtual void Free() override;

};
