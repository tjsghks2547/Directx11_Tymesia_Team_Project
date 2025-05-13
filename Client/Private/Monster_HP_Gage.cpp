#include "pch.h"
#include "Monster_HP_Gage.h"
#include "GameInstance.h"

CMonster_HP_Gage::CMonster_HP_Gage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMonster_HP_Gage::CMonster_HP_Gage(const CMonster_HP_Gage& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster_HP_Gage::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_HP_Gage::Initialize(void* pArg)
{
	Monster_HP_Gage_DESC* pDesc = static_cast<Monster_HP_Gage_DESC*>(pArg);

	m_fMaxHP = pDesc->fMaxHP;
	m_fCurHP = pDesc->fCurHP;
	m_fShieldHP = pDesc->fShieldHP;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(0.8f, 0.5f, 0.1f));

	m_fOffsetZ = 0.99f;

	return S_OK;
}

void CMonster_HP_Gage::Priority_Update(_float fTimeDelta)
{
}

void CMonster_HP_Gage::Update(_float fTimeDelta)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CMonster_HP_Gage::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CMonster_HP_Gage::Render()
{
	for (_uint i = 4; i >= 3; i--)
	{
		_float pCurHP = {};
		if (i == 4)
		{
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos = XMVectorSetZ(vPos, m_fOffsetZ - 0.1f);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			pCurHP = *m_fCurHP / *m_fMaxHP;
		}
		else
		{
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			vPos = XMVectorSetZ(vPos, m_fOffsetZ);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
			pCurHP = *m_fShieldHP / *m_fMaxHP;
		}
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_CurHP", &pCurHP, sizeof(_float))))
			return E_FAIL;

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(13);

		m_pVIBufferCom->Bind_InputAssembler();

		m_pVIBufferCom->Render();

	}
	return S_OK;
}


HRESULT CMonster_HP_Gage::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_HP"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_HP_Gage::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMonster_HP_Gage* CMonster_HP_Gage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_HP_Gage* pInstance = new CMonster_HP_Gage(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_HP_Gage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_HP_Gage::Clone(void* pArg)
{
	CMonster_HP_Gage* pInstance = new CMonster_HP_Gage(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster_HP_Gage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_HP_Gage::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
