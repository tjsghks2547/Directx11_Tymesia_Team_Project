#pragma once

#include "State.h"


class CNormal_Evade_FL final : public CState
{
private:
	CNormal_Evade_FL();
	virtual ~CNormal_Evade_FL() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.05f };

public:
	static  CNormal_Evade_FL* Create();
	virtual void Free() override;

};
