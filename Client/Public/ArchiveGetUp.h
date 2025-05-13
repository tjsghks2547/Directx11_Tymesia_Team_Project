#pragma once

#include "State.h"

class CArchiveGetUp final : public CState
{
private:
	CArchiveGetUp();
	virtual ~CArchiveGetUp() = default;

public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;


public:
	static CArchiveGetUp* Create();
	virtual void Free() override;

};
