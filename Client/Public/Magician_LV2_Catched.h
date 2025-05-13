#pragma once

#include "State.h"


class CMagician_LV2_Catched final : public CState
{
private:
	CMagician_LV2_Catched();
	virtual ~CMagician_LV2_Catched() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

	_float m_fTurnSpeed = { 20.f };
	_float m_fWalkSpeed = { 0.075f };

public:
	static  CMagician_LV2_Catched* Create();
	virtual void Free() override;

};
