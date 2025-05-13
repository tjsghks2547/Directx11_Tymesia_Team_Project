#include "pch.h" 
#include "..\Public\Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;



	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	TERRAININFO* pDesc = static_cast<TERRAININFO*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vTerrainPos));

	return S_OK;
}	

void CTerrain::Priority_Update(_float fTimeDelta)
{
	//m_pNavigationCom->SetUp_WorldMatrix(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
	XMVECTOR vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_pVIBufferCom->Get_TerrainPos(), vPos);
	_float3 vPos_Float3 = {};
	XMStoreFloat3(&vPos_Float3, vPos);
	m_pVIBufferCom->Set_TerrainPos(vPos_Float3);
}

void CTerrain::Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	/* �׺��̰��̼� ���� Ȯ�ο����� ��� ��*/
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

#ifdef _DEBUG				
	//m_pNavigationCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTerrain::Ready_Components(void* _pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(_pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	/* Com_Texture */
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Texture_Terrain_Normal"),
		TEXT("Com_NormalTexture"), reinterpret_cast<CComponent**>(&m_pTextureNormalCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Texture_Terrain_ORM"),
		TEXT("Com_ORMTexture"), reinterpret_cast<CComponent**>(&m_pTextureORMCom))))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Texture_TerrainMask"),
		TEXT("Com_TerrainMaskTexture"), reinterpret_cast<CComponent**>(&m_pTextureMaskCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer_Terrain"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureNormalCom->Bind_ShaderResources(m_pShaderCom, "g_NormalTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureORMCom->Bind_ShaderResources(m_pShaderCom, "g_ORMTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureMaskCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureNormalCom);
	Safe_Release(m_pTextureORMCom);
	Safe_Release(m_pTextureMaskCom);
}
