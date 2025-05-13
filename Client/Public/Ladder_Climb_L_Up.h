#pragma once

#include "State.h"

class CLadder_Climb_L_Up final : public CState
{
private:
	CLadder_Climb_L_Up();
	virtual ~CLadder_Climb_L_Up() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CLadder_Climb_L_Up* Create();
	virtual void Free() override;

};
