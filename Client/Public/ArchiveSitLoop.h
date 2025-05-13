#pragma once

#include "State.h"

class CArchiveSitLoop final : public CState
{
private:
	CArchiveSitLoop();
	virtual ~CArchiveSitLoop() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CArchiveSitLoop* Create();
	virtual void Free() override;

};
