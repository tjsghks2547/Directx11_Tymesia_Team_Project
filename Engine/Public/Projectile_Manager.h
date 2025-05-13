#pragma once

#include "Base.h"
#include "Projectile.h"
#include "Engine_Defines.h"


BEGIN(Engine)
class ENGINE_DLL CProjectile_Manager : public CBase
{
private:
	CProjectile_Manager();
	virtual ~CProjectile_Manager() = default;

public:
	HRESULT Initialize();
	void Priority_Update(_float _fTimeDelta);
	void Update(_float _fTimeDelta);
	void Late_Update(_float _fTimeDelta);

public:
	HRESULT Add_Projectile(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, PROJECTILE_CATEGORY _eCategory, void* _pArg);

public:
	HRESULT Fire_Multi_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos, _uint iCount, _bool bReverse);
	HRESULT Fire_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos);

private:
	map<PROJECTILE_CATEGORY, vector<class CProjectile*>> m_pProjectiles;
	deque<class CProjectile*> m_pFireProjectiles;
	class CGameInstance* m_pGameInstance = { nullptr };
public:
	static CProjectile_Manager* Create();
	virtual void Free() override;
};
END
