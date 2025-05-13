#pragma once

#include "State.h"


class CParryDeflect_RUp final : public CState
{
private:
	CParryDeflect_RUp();
	virtual ~CParryDeflect_RUp() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CParryDeflect_RUp* Create();
	virtual void Free() override;

};
