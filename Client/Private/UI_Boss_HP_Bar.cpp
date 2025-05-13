#include "pch.h"
#include "UI_Boss_HP_Bar.h"
#include "GameInstance.h"

CUI_Boss_HP_Bar::CUI_Boss_HP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI_Image(pDevice, pContext)
{
}

CUI_Boss_HP_Bar::CUI_Boss_HP_Bar(const CUI_Boss_HP_Bar& Prototype)
	:CUI_Image(Prototype)
{
}

HRESULT CUI_Boss_HP_Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_HP_Bar::Initialize(void* pArg)
{
	m_fSizeX = 718.f;
	m_fSizeY = 19.f;
	UI_BOSS_HP_BAR_DESC* pDesc = static_cast<UI_BOSS_HP_BAR_DESC*>(pArg);

	pDesc->fNear = 0.f;
	pDesc->fFar = 1.f;
	pDesc->fSizeX = m_fSizeX;
	pDesc->fSizeY = m_fSizeY;
	pDesc->fX = 800;
	pDesc->fY = 50;
	pDesc->fZ = 0.1f;
	pDesc->fSpeedPerSec = 5.f;
	pDesc->fRotationPerSec = XMConvertToRadians(90.f);

	m_bBoss_HP_Bar_Active = pDesc->bBoss_HP_Bar_Active;
	m_bBossDead = pDesc->bBossDead;
	m_sBossName = pDesc->sBossName;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage"), pDesc->iCurLevel, TEXT("Layer_UIScene"), pArg)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage_Effect"), pDesc->iCurLevel, TEXT("Layer_UIScene"), pArg)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Phase"), pDesc->iCurLevel, TEXT("Layer_UIScene"), pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Boss_HP_Bar::Priority_Update(_float fTimeDelta)
{
	if (!(*m_bBoss_HP_Bar_Active))
		return;
}

void CUI_Boss_HP_Bar::Update(_float fTimeDelta)
{
	if (!(*m_bBoss_HP_Bar_Active))
		return;
}

void CUI_Boss_HP_Bar::Late_Update(_float fTimeDelta)
{
	if (*m_bBossDead)
		m_pGameInstance->Add_DeadObject(TEXT("Layer_UIScene"), this);
	if (!(*m_bBoss_HP_Bar_Active))
		return;

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_HP_Bar::Render()
{
	if (!(*m_bBoss_HP_Bar_Active))
		return S_OK;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	_float3 vpos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION);

	m_pGameInstance->Render_Font(TEXT("Font_NotoSansKR18"), m_sBossName.c_str(), { vpos.x - 330.f, vpos.y + 5.f }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 0.8f, vpos.z);

	return S_OK;
}

HRESULT CUI_Boss_HP_Bar::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HP"),
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

CUI_Boss_HP_Bar* CUI_Boss_HP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_HP_Bar* pInstance = new CUI_Boss_HP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Boss_HP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Boss_HP_Bar::Clone(void* pArg)
{
	CUI_Boss_HP_Bar* pInstance = new CUI_Boss_HP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Boss_HP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_HP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
