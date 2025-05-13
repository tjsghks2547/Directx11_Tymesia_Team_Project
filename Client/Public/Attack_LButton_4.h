#pragma once

#include "State.h"

class CAttack_LButton_4 final : public CState
{
private:
	CAttack_LButton_4();
	virtual ~CAttack_LButton_4() = default;


public:
	HRESULT Initialize();

public:
	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta) override;

public:
	static CAttack_LButton_4* Create();
	virtual void Free() override;

};
