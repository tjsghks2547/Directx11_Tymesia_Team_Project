#include "pch.h"
#include "EnvironmentObject.h"

CEnvironmentObject::CEnvironmentObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{ _pDevice, _pContext }
{
}

CEnvironmentObject::CEnvironmentObject(const CEnvironmentObject& _Prototype)
    :CGameObject(_Prototype)
{
}

HRESULT CEnvironmentObject::Initialize(void* _pArg)
{
    CEnvironmentObject::ENVIRONMENT_OBJECT_DESC* pDesc = static_cast<CEnvironmentObject::ENVIRONMENT_OBJECT_DESC*>(_pArg);

	m_eCurrentLevel = pDesc->eLevelID;

    m_fFrustumRadius = pDesc->fFrustumRadius;
	m_iPassIndex = pDesc->iPassIndex;
	m_iInstanceType = pDesc->iInstanceType;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

	for (auto& iter : m_vecInstancePosition)
	{
		pDesc->fPosition.x = iter.x;
		pDesc->fPosition.y = iter.y;
		pDesc->fPosition.z = iter.z;
	}
	for (auto& iter : m_vecInstanceRotation)
	{
		pDesc->fRotation.x = iter.x;
		pDesc->fRotation.y = iter.y;
		pDesc->fRotation.z = iter.z;
	}
	for (auto& iter : m_vecInstanceScale)
	{
		pDesc->fScaling.x = iter.x;
		pDesc->fScaling.y = iter.y;
		pDesc->fScaling.z = iter.z;
	}

    strcpy_s(m_EnvironmentMeshName, pDesc->ObjectName.c_str());

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMConvertToRadians(pDesc->fRotation.x), XMConvertToRadians(pDesc->fRotation.y), XMConvertToRadians(pDesc->fRotation.z));

    return S_OK;
}

HRESULT CEnvironmentObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxInstanceMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CEnvironmentObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
