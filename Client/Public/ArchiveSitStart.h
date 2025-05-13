#pragma once

#include "State.h"

class CArchiveSitStart final : public CState
{
private:
	CArchiveSitStart();
	virtual ~CArchiveSitStart() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CArchiveSitStart* Create();
	virtual void Free() override;

};
