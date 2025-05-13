#pragma once

#include "State.h"


class CHArmorLV1_Execution final : public CState
{
private:
	CHArmorLV1_Execution();
	virtual ~CHArmorLV1_Execution() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CHArmorLV1_Execution* Create();
	virtual void Free() override;

};
