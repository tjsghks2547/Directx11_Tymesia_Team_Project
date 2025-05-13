#include "..\Public\Light_Manager.h"

#include "Light.h"
#include "Light_Dynamic.h"
#include "Transform.h"

CLight_Manager::CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex) const
{
	auto		iter = m_Lights.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Initialize()
{
	return S_OK;
}

void CLight_Manager::Update(_float _fTimeDelta)
{
	for (vector<LIGHT_RANGE_DESC>::iterator pRangeDesc = m_LightRangeDescs.begin(); pRangeDesc != m_LightRangeDescs.end();)
	{
		(*pRangeDesc).pDesc->Get_LightDesc()->fRange += _fTimeDelta * (*pRangeDesc).fRangeSpeed;

		if ((*pRangeDesc).fMAXRange < (*pRangeDesc).pDesc->Get_LightDesc()->fRange)
		{
			pRangeDesc = m_LightRangeDescs.erase(pRangeDesc);
		}
		else
			pRangeDesc++;
	}

	for (vector<LIGHT_RANGE_DESC_MINUS>::iterator pRangeMinDesc = m_LightRangeMinDescs.begin(); pRangeMinDesc != m_LightRangeMinDescs.end();)
	{
		(*pRangeMinDesc).pDesc->Get_LightDesc()->fRange -= _fTimeDelta * (*pRangeMinDesc).fRangeSpeed;

		if ((*pRangeMinDesc).fMINRange > (*pRangeMinDesc).pDesc->Get_LightDesc()->fRange)
		{
			if ((*pRangeMinDesc).pDesc->Get_LightDesc()->fRange <= 0)
				(*pRangeMinDesc).pDesc->Get_LightDesc()->fRange = 0.01f;

			pRangeMinDesc = m_LightRangeMinDescs.erase(pRangeMinDesc);
		}
		else
			pRangeMinDesc++;
	}
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& LightDesc, CTransform* pTransform)
{
	if (pTransform == nullptr)
	{
		CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
		if (nullptr == pLight)
			return E_FAIL;

		m_Lights.push_back(pLight);
	}
	else
	{
		CLight_Dynamic* pLight = CLight_Dynamic::Create(m_pDevice, m_pContext, LightDesc, pTransform);
		if (nullptr == pLight)
			return E_FAIL;

		m_LightDynamics.push_back(pLight);

	}
	return S_OK;
}

HRESULT CLight_Manager::Add_Light_Range(const LIGHT_DESC& LightDesc, _float fMaxRange, _float fRangeSpeed)
{
	CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	LIGHT_RANGE_DESC RangeDesc = {};
	RangeDesc.pDesc = pLight;
	RangeDesc.fMAXRange = fMaxRange;
	RangeDesc.fRangeSpeed = fRangeSpeed;
	m_LightRangeDescs.push_back(RangeDesc);

	return S_OK;
}

HRESULT CLight_Manager::Add_Light_Range_MINUS(const LIGHT_DESC& LightDesc, _float fMinRange, _float fRangeSpeed)
{
	CLight* pLight = CLight::Create(m_pDevice, m_pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	LIGHT_RANGE_DESC_MINUS RangeDesc = {};
	RangeDesc.pDesc = pLight;
	RangeDesc.fMINRange = fMinRange;
	RangeDesc.fRangeSpeed = fRangeSpeed;
	m_LightRangeMinDescs.push_back(RangeDesc);

	return S_OK;
}

void CLight_Manager::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		pLight->Render(pShader, pVIBuffer);
	}

	for (auto& pLightDynamic : m_LightDynamics)
	{
		pLightDynamic->Render(pShader, pVIBuffer);
	}
}

HRESULT CLight_Manager::Delete_Light_Type(LIGHT_DESC::TYPE eType)
{
	for (list<CLight*>::iterator pLight = m_Lights.begin(); pLight != m_Lights.end();)
	{
		if ((*pLight)->Delete_Light_Type(eType))
		{
			Safe_Release((*pLight));

			pLight = m_Lights.erase(pLight);
		}
		else
			pLight++;
	}

	return S_OK;
}

HRESULT CLight_Manager::Delete_Dynamic_Light(CTransform* pTransform)
{
	for (list<CLight_Dynamic*>::iterator pLight = m_LightDynamics.begin(); pLight != m_LightDynamics.end();)
	{
		if ((*pLight)->Delete_Light(pTransform))
		{
			Safe_Release((*pLight));

			pLight = m_LightDynamics.erase(pLight);
		}
		else
			pLight++;
	}

	return S_OK;
}

HRESULT CLight_Manager::Delete_Static_Light(_uint iCurrentLevel)
{
	for (list<CLight*>::iterator pLight = m_Lights.begin(); pLight != m_Lights.end();)
	{
		if ((*pLight)->Delete_Light(iCurrentLevel))
		{
			Safe_Release((*pLight));

			pLight = m_Lights.erase(pLight);
		}
		else
			pLight++;
	}

	return S_OK;
}

CLight_Manager* CLight_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLight_Manager* pInstance = new CLight_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLight_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLight_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	for (auto& pLightDynamic : m_LightDynamics)
		Safe_Release(pLightDynamic);

	m_Lights.clear();
}
