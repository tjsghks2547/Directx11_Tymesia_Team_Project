
#pragma once

#include "State.h"

class CArchiveSitLight_Up final : public CState
{
private:
	CArchiveSitLight_Up();
	virtual ~CArchiveSitLight_Up() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CArchiveSitLight_Up* Create();
	virtual void Free() override;

};
