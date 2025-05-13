#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CNavigation;
class CModel;
END

class CState abstract : public CBase
{
protected:
	CState();
	virtual ~CState() = default;


public:
	HRESULT Initialize();

	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Set_MonsterLookDir(_float4 _vMonsterLookDir) { m_vMonsterLookDir = _vMonsterLookDir; }
	virtual void Set_MonsterUpDir(_float4 _vMonsterUpDir) { m_vMonsterUpDir = _vMonsterUpDir; }		
	virtual void Set_GetMonsterPos(_float4 _vMonsterMonsterPos) { m_vMonsterPos = _vMonsterMonsterPos; }
	virtual void Set_MonsterModel(CModel* _pMonsterModel) { m_pMonsterModel = _pMonsterModel; }

	CModel* Get_MonsterModel() { return m_pMonsterModel; }

protected:
	CModel* m_pMonsterModel = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	_float4		   m_vMonsterLookDir = {};
	_float4		   m_vMonsterUpDir = {};	
	_float4        m_vMonsterPos = {};


public:
	virtual void Free();

};
