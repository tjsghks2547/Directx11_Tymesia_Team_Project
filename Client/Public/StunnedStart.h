#pragma once

#include "State.h"


class CStunnedStart final : public CState
{
private:
	CStunnedStart();
	virtual ~CStunnedStart() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CStunnedStart* Create();
	virtual void Free() override;

};
