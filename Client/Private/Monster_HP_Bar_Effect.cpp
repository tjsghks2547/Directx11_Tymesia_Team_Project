#include "pch.h"
#include "Monster_HP_Bar_Effect.h"
#include "GameInstance.h"

CMonster_HP_Bar_Effect::CMonster_HP_Bar_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CMonster_HP_Bar_Effect::CMonster_HP_Bar_Effect(const CMonster_HP_Bar_Effect& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CMonster_HP_Bar_Effect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;

}

HRESULT CMonster_HP_Bar_Effect::Initialize(void* pArg)
{
	Monster_HP_Bar_Effect_DESC* pDesc = static_cast<Monster_HP_Bar_Effect_DESC*>(pArg);

	m_fCurHP = pDesc->fCurHP;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3(0.8f, 0.5f, 1.f));
	_vector vPos = { 0.f,0.f,0.f,1.f };

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	return S_OK;
}

void CMonster_HP_Bar_Effect::Priority_Update(_float fTimeDelta)
{
	if (*m_fCurHP <= 0.f)
		m_bActive = true;
	else
		m_bActive = false;

	if (m_bActive)
		m_fFlashTime += fTimeDelta;

}

void CMonster_HP_Bar_Effect::Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	if (m_fFlashTime >= 1.f)
	{
		m_fFlashTime = 0.f;
	}
	m_pTransformCom->Scaling(_float3(0.6f + (m_fFlashTime * 0.5f), 0.5f + (m_fFlashTime * 0.1f), 1.f));
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CMonster_HP_Bar_Effect::Late_Update(_float fTimeDelta)
{
	if (!m_bActive)
		return;
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CMonster_HP_Bar_Effect::Render()
{
	if (!m_bActive)
		return S_OK;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeDelta", &m_fFlashTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(15);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMonster_HP_Bar_Effect::Ready_Components()
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

HRESULT CMonster_HP_Bar_Effect::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMonster_HP_Bar_Effect* CMonster_HP_Bar_Effect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_HP_Bar_Effect* pInstance = new CMonster_HP_Bar_Effect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_HP_Bar_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_HP_Bar_Effect::Clone(void* pArg)
{
	CMonster_HP_Bar_Effect* pInstance = new CMonster_HP_Bar_Effect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster_HP_Bar_Effect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_HP_Bar_Effect::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
