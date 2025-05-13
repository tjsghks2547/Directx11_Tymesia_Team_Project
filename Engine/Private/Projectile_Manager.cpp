#include "Projectile_Manager.h"
#include "GameInstance.h"

CProjectile_Manager::CProjectile_Manager()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CProjectile_Manager::Initialize()
{
	return S_OK;
}

void CProjectile_Manager::Priority_Update(_float _fTimeDelta)
{
	for (auto& iter : m_pFireProjectiles)
		iter->Priority_Update(_fTimeDelta);
}

void CProjectile_Manager::Update(_float _fTimeDelta)
{
	for (auto& iter : m_pFireProjectiles)
		iter->Update(_fTimeDelta);
}

void CProjectile_Manager::Late_Update(_float _fTimeDelta)
{
	for (auto& iter = m_pFireProjectiles.begin(); iter != m_pFireProjectiles.end();)
	{
		if ((*iter)->Get_IsFire() == false)
		{
			(*iter)->Reset_Projectile();

			if (m_pFireProjectiles.size() == 1)
			{
				m_pFireProjectiles.clear();
				break;
			}
			iter = m_pFireProjectiles.erase(iter);
		}
		else
		{
			(*iter)->Late_Update(_fTimeDelta);
			iter++;
		}
	}
}

HRESULT CProjectile_Manager::Add_Projectile(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, PROJECTILE_CATEGORY _eCategory, void* _pArg)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, _iPrototypeLevelIndex, _strPrototypeTag, _pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CProjectile* pProjectile = dynamic_cast<CProjectile*>(pGameObject);
	if (pProjectile == nullptr)
	{
		Safe_Release(pProjectile);
		return E_FAIL;
	}

	m_pProjectiles[_eCategory].push_back(pProjectile);

	return S_OK;
}


//2발 이상
HRESULT CProjectile_Manager::Fire_Multi_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos, _uint iCount, _bool bReverse)
{
	if (m_pProjectiles[_eCategory].size() < iCount)
		return E_FAIL;

	_vector vDir = XMVector3Normalize(vEndPos - vStartPos);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vDir, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	_float fOffsetStep = 0.5f;
	_float fBackWardStep = 2.f;

	_uint iMidIndex = iCount / 2;


	_uint iFiredCount = 0;

	for (auto& iter : m_pProjectiles[_eCategory])
	{
		if (!iter->Get_IsFire())
		{
			_vector vNewStart = vStartPos;

			_int iOffsetIndex = iFiredCount - (iCount / 2);
			vNewStart += vRight * (fOffsetStep * iOffsetIndex);

			if (bReverse)
				vNewStart += vDir * (fBackWardStep * iFiredCount);
			else
				vNewStart -= vDir * (fBackWardStep * iFiredCount);

			iter->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vNewStart);
			iter->Set_Target(vDir);
			iter->Set_IsFire(true);
			m_pFireProjectiles.push_back(iter);

			if (++iFiredCount >= iCount)
				break;
		}

	}

	return S_OK;
}

//1발 무조건
HRESULT CProjectile_Manager::Fire_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos)
{
	_vector vDir = XMVector3Normalize(vEndPos - vStartPos);

	for (auto& iter : m_pProjectiles[_eCategory])
	{
		if (!iter->Get_IsFire())
		{
			iter->Set_IsFire(true);
			iter->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vStartPos);
			iter->Set_Target(vDir);
			m_pFireProjectiles.push_back(iter);
			break;
		}
	}
	return S_OK;
}

CProjectile_Manager* CProjectile_Manager::Create()
{
	CProjectile_Manager* pInstance = new CProjectile_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CProjectile_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CProjectile_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_pProjectiles)
	{
		for (auto& iter : Pair.second)
		{
			Safe_Release(iter);
		}
		Pair.second.clear();
	}

	m_pProjectiles.clear();

	Safe_Release(m_pGameInstance);
}
