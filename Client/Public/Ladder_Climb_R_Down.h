#pragma once

#include "State.h"

class CLadder_Climb_R_Down final : public CState
{
private:
	CLadder_Climb_R_Down();
	virtual ~CLadder_Climb_R_Down() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CLadder_Climb_R_Down* Create();
	virtual void Free() override;

};
