#pragma once

#include "State.h"



class CPlayerIdle final : public CState
{
private:
	CPlayerIdle();
	virtual ~CPlayerIdle() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CPlayerIdle* Create();
	virtual void Free() override;

};
