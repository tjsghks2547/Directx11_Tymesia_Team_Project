#include "Monster_Manager.h"
#include "GameInstance.h"

CMonster_Manager::CMonster_Manager()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMonster_Manager::Initialize()
{
	return S_OK;
}

void CMonster_Manager::Priority_Update(_float _fTimeDelta)
{
#ifdef _DEBUG
	if (m_pGameInstance->isKeyEnter(DIK_L))
	{
		Respawn_Monster(CATEGORY_BOSS);
	}
	if (m_pGameInstance->isKeyEnter(DIK_P))
	{
		Respawn_Monster(CATEGORY_ELITE);
	}
#endif // _DEBUG
	Active_Monster();

	for (auto& iter : m_pCheck_Monsters)
		iter->Priority_Update(_fTimeDelta);

	Find_Closest_Monster();
}

void CMonster_Manager::Update(_float _fTimeDelta)
{
	for (auto& iter : m_pCheck_Monsters)
		iter->Update(_fTimeDelta);
}

void CMonster_Manager::Late_Update(_float _fTimeDelta)
{
	for (auto& iter = m_pCheck_Monsters.begin(); iter != m_pCheck_Monsters.end();)
	{
		if (!(*iter)->Get_Active() || (*iter)->Get_Dead())
		{
			if ((!(*iter)->Get_IsRender()))
			{
				(*iter)->Reset_Info();

				if (m_pCheck_Monsters.size() == 1)
				{
					m_pCheck_Monsters.clear();
					break;
				}
				iter = m_pCheck_Monsters.erase(iter);
			}
			else
			{
				(*iter)->Late_Update(_fTimeDelta);
				++iter;
			}
		}
		else
		{
			(*iter)->Late_Update(_fTimeDelta);
			++iter;
		}
	}

	Delete_Monster();
}

HRESULT CMonster_Manager::Add_Monster(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, MONSTER_CATEGORY _eCategory, void* _pArg)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, _iPrototypeLevelIndex, _strPrototypeTag, _pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CMonster* pMonster = dynamic_cast<CMonster*>(pGameObject);
	if (pMonster == nullptr)
	{
		Safe_Release(pMonster);
		return E_FAIL;
	}
	m_pMonsters[_eCategory].push_back(pMonster);

	MONSTER_INFO pInfo = {};
	pInfo._iPrototypeLevelIndex = _iPrototypeLevelIndex;
	pInfo._strPrototypeTag = _strPrototypeTag;
	pInfo._eCategory = _eCategory;
	pInfo.pArg.fPosition = static_cast<CGameObject::GAMEOBJECT_DESC*>(_pArg)->fPosition;
	pInfo.pArg.iCurLevel = static_cast<CGameObject::GAMEOBJECT_DESC*>(_pArg)->iCurLevel;

	m_MonsterInfos[_eCategory].push_back(pInfo);

	return S_OK;
}

HRESULT CMonster_Manager::Respawn_Monster(MONSTER_CATEGORY _eCategory)
{
	m_pCheck_Monsters.clear();
	for (auto& iter : m_pMonsters[_eCategory])
	{
		if (_eCategory == CATEGORY_BOSS)
		{
			m_bBoss_Active = false;
		}
		iter->Set_HP_Bar_ActiveOff();

		Safe_Release(iter);
	}
	m_pMonsters[_eCategory].clear();

	for (_uint i = 0; i < m_MonsterInfos[_eCategory].size(); i++)
	{
		CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT,
			m_MonsterInfos[_eCategory][i]._iPrototypeLevelIndex,
			m_MonsterInfos[_eCategory][i]._strPrototypeTag,
			&m_MonsterInfos[_eCategory][i].pArg));

		if (nullptr == pGameObject)
			return E_FAIL;

		CMonster* pMonster = dynamic_cast<CMonster*>(pGameObject);

		if (pMonster == nullptr)
		{
			Safe_Release(pMonster);
			return E_FAIL;
		}
		m_pMonsters[_eCategory].push_back(pMonster);
	}

	return S_OK;
}

HRESULT CMonster_Manager::Active_Monster()
{
	for (auto& Pair : m_pMonsters)
	{
		for (auto& pMonster : Pair.second)
		{
			//Active가 아니고 플레이어랑 좀 멀어서 절두체 안에만 있다는 가정하면 Update 이런것들은 돌아야하니까 검사.
			//기존 컬링방식 일듯?
			if (!pMonster->Get_Active())
			{
				if (m_pGameInstance->isIn_Frustum_WorldSpace(pMonster->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER))
				{
					if (pMonster->Get_IsRender())
						continue;

					pMonster->Set_IsRender(true);
					m_pCheck_Monsters.push_back(pMonster);
				}
				else
					pMonster->Set_IsRender(false);
			}
		}
	}
	return S_OK;
}

HRESULT CMonster_Manager::Find_Closest_Monster()
{
	if (m_pCheck_Monsters.empty())
		return E_FAIL;

	for (auto& monster : m_pCheck_Monsters)
	{
		monster->Set_IsClosest(false);
	}

	CMonster* pClosestMonster = nullptr;
	_float fMinDistance = FLT_MAX; // 최댓값으로 초기화
	_bool bFirst = true;

	for (auto& Monster : m_pCheck_Monsters)
	{
		_float fDistance = Monster->Get_Distance();

		if (bFirst)
		{
			fMinDistance = fDistance;
			pClosestMonster = Monster;
			bFirst = false;
			continue;
		}

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			pClosestMonster = Monster;
		}
	}

	if (pClosestMonster)
	{
		pClosestMonster->Set_IsClosest(true);
	}

	return S_OK;
}

HRESULT CMonster_Manager::Add_Delete_Monster(CMonster* pMonster)
{
	m_pDelete_Monsters.push_back(pMonster);
	vector<MONSTER_CATEGORY> emptyKeys;

	for (auto& Pair : m_pMonsters)
	{
		auto& vMonsters = Pair.second;
		for (auto iter = vMonsters.begin(); iter != vMonsters.end(); )
		{
			if (std::find(m_pDelete_Monsters.begin(), m_pDelete_Monsters.end(), *iter) != m_pDelete_Monsters.end())
			{
				iter = vMonsters.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		if (vMonsters.empty())
		{
			emptyKeys.push_back(Pair.first);
			vMonsters.clear();
		}
	}
	for (const auto& key : emptyKeys)
	{
		m_pMonsters.erase(key);
	}

	return S_OK;
}

HRESULT CMonster_Manager::Delete_Monster()
{
	for (auto& iter = m_pDelete_Monsters.begin(); iter != m_pDelete_Monsters.end();)
	{

		Safe_Release((*iter));
		m_pDelete_Monsters.erase(iter);

		if (m_pDelete_Monsters.size() == 0)
			break;

		++iter;
	}
	return S_OK;
}

HRESULT CMonster_Manager::Delete_All_Monster()
{
	m_pCheck_Monsters.clear();

	for (auto& Pair : m_pMonsters)
	{
		for (auto& iter : Pair.second)
		{
			Safe_Release(iter);
		}
		Pair.second.clear();
	}

	for (auto& Pair : m_MonsterInfos)
	{
		Pair.second.clear();
	}

	m_MonsterInfos.clear();

	m_pMonsters.clear();

	return S_OK;
}

CMonster_Manager* CMonster_Manager::Create()
{
	CMonster_Manager* pInstance = new CMonster_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMonster_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Manager::Free()
{
	__super::Free();

	m_pCheck_Monsters.clear();

	for (auto& Pair : m_pMonsters)
	{
		for (auto& iter : Pair.second)
		{
			Safe_Release(iter);
		}
		Pair.second.clear();
	}

	m_pMonsters.clear();

	Safe_Release(m_pGameInstance);
}
