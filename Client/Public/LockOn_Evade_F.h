#pragma once

#include "State.h"


class CLockOn_Evade_F final : public CState
{
private:
	CLockOn_Evade_F();
	virtual ~CLockOn_Evade_F() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CLockOn_Evade_F* Create();
	virtual void Free() override;

};
