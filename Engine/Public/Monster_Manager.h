#pragma once

#include "Base.h"
#include "Monster.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CMonster_Manager : public CBase
{
private:
	struct MONSTER_INFO
	{
		_uint _iPrototypeLevelIndex = {};
		_wstring _strPrototypeTag = {};
		MONSTER_CATEGORY _eCategory = {};
		CGameObject::GAMEOBJECT_DESC pArg = {};
	};

private:
	CMonster_Manager();
	virtual ~CMonster_Manager() = default;

public:
	HRESULT Initialize();
	void	Priority_Update(_float _fTimeDelta);
	void	Update(_float _fTimeDelta);
	void	Late_Update(_float _fTimeDelta);

public:
	HRESULT Add_Monster(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, MONSTER_CATEGORY _eCategory, void* _pArg);
	HRESULT Respawn_Monster(MONSTER_CATEGORY _eCategory);
	deque<class CMonster*>& Get_Check_Monsters() { return m_pCheck_Monsters; }
	void Set_Boss_Dead(_bool bCheck) { m_bStage_Boss_Dead = bCheck; }
	void Set_Boss_Active(_bool bCheck) { m_bBoss_Active = bCheck; }
	_bool Get_Boss_Dead()const { return m_bStage_Boss_Dead; }
	_bool Get_Boss_Active() const { return m_bBoss_Active; }

public:
	HRESULT Active_Monster();
	HRESULT Find_Closest_Monster();
	HRESULT Add_Delete_Monster(class CMonster* pMonster);
	HRESULT Delete_Monster();
	HRESULT Delete_All_Monster();
private:
	map<MONSTER_CATEGORY, vector<class CMonster*>> m_pMonsters;
	map<MONSTER_CATEGORY, vector<CMonster_Manager::MONSTER_INFO>> m_MonsterInfos;
	deque<class CMonster*> m_pCheck_Monsters;
	deque<class CMonster*> m_pDelete_Monsters;
	vector<_float>		   m_fDistances;
	class CGameInstance* m_pGameInstance = { nullptr };

private:
	_bool m_bStage_Boss_Dead = { false };
	_bool m_bBoss_Active = { false };
public:
	static CMonster_Manager* Create();
	virtual void Free() override;
};

END