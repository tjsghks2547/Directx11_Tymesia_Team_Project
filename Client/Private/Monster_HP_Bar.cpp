#include "pch.h"
#include "GameInstance.h"
#include "Monster_HP_Bar.h"
#include "Monster_HP_Gage.h"
#include "Monster_HP_Gage_Effect.h"
#include "Monster_HP_Bar_Effect.h"

CMonster_HP_Bar::CMonster_HP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CMonster_HP_Bar::CMonster_HP_Bar(const CMonster_HP_Bar& Prototype)
	:CContainerObject(Prototype)
{
}

HRESULT CMonster_HP_Bar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_HP_Bar::Initialize(void* pArg)
{
	Monster_HP_Bar_DESC* pDesc = static_cast<Monster_HP_Bar_DESC*>(pArg);

	m_pParentState = pDesc->pParentState;
	m_pMonsterMatrix = pDesc->pMonsterMatrix;
	m_bHP_Bar_Active = pDesc->bHP_Bar_Active;
	m_bMonsterDead = pDesc->bDead;
	m_fHeight = pDesc->fHeight;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pDesc)))
		return E_FAIL;

	_vector vHPBar_Offset = { 0.f,*m_fHeight,1.f, 1.f };

	m_pTransformCom->Scaling(_float3(286.f, 23.f, 1.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vHPBar_Offset);

	return S_OK;
}

void CMonster_HP_Bar::Priority_Update(_float fTimeDelta)
{

	if (!*m_bHP_Bar_Active)
		return;

	__super::Priority_Update(fTimeDelta);
}

void CMonster_HP_Bar::Update(_float fTimeDelta)
{
	if (!*m_bHP_Bar_Active)
		return;
	Bill_Board();
	__super::Update(fTimeDelta);
}

void CMonster_HP_Bar::Late_Update(_float fTimeDelta)
{
	if (*m_bMonsterDead)
		m_pGameInstance->Add_DeadObject(TEXT("Layer_MonsterHP"), this);
	if (!*m_bHP_Bar_Active)
		return;
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	__super::Late_Update(fTimeDelta);
}

HRESULT CMonster_HP_Bar::Render()
{
	if (!*m_bHP_Bar_Active)
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(6);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();
	return S_OK;
}


void CMonster_HP_Bar::Bill_Board()
{
	_vector vParentTranslation, vParentRotation, vParentScale;
	XMStoreFloat4x4(&m_ResultMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pMonsterMatrix));
	XMMatrixDecompose(&vParentScale, &vParentRotation, &vParentTranslation, XMLoadFloat4x4(&m_ResultMatrix));
	_matrix matWithoutRotation = XMMatrixAffineTransformation(vParentScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), vParentTranslation);

	_vector vRight{}, vUp{}, vLook{};
	_vector vCamPos = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());
	vLook = matWithoutRotation.r[3] - vCamPos;

	vLook = XMVector3Normalize(vLook);
	vUp = matWithoutRotation.r[1];

	vRight = XMVector3Cross(vUp, vLook);
	vLook = XMVector3Cross(vRight, vUp);

	_float3 vScale = _float3(XMVectorGetX(XMVector3Length(matWithoutRotation.r[0])), XMVectorGetY(XMVector3Length(matWithoutRotation.r[1])), XMVectorGetZ(XMVector3Length(matWithoutRotation.r[2])));

	_float fDistance = XMVectorGetX(XMVector3Length(matWithoutRotation.r[3]) - vCamPos);
	_float fBaseSize = 50.f;
	_float fScaleFactor = fBaseSize * (fDistance + 1.f);

	fScaleFactor = max(0.1f, min(fScaleFactor, 1.0f));

	matWithoutRotation.r[0] = XMVector3Normalize(vRight) * vScale.x * fScaleFactor;
	matWithoutRotation.r[1] = XMVector3Normalize(vUp) * vScale.y * fScaleFactor;
	matWithoutRotation.r[2] = XMVector3Normalize(vLook) * vScale.z * fScaleFactor;


	XMStoreFloat4x4(&m_ResultMatrix, matWithoutRotation);

}


HRESULT CMonster_HP_Bar::Ready_Components()
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

HRESULT CMonster_HP_Bar::Ready_PartObjects(Monster_HP_Bar_DESC* pDesc)
{

	CMonster_HP_Gage::Monster_HP_Gage_DESC MonsterHP_Gage_Desc{};
	MonsterHP_Gage_Desc.pParentWorldMatrix = &m_ResultMatrix;
	MonsterHP_Gage_Desc.pParent = this;
	MonsterHP_Gage_Desc.fCurHP = pDesc->fCurHP;
	MonsterHP_Gage_Desc.fMaxHP = pDesc->fMaxHP;
	MonsterHP_Gage_Desc.fShieldHP = pDesc->fShieldHP;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Monster_HP_Gage"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Gage"), &MonsterHP_Gage_Desc)))
		return E_FAIL;

	CMonster_HP_Gage_Effect::Monster_HP_Gage_Effect_DESC MonsterHP_Gage_Effect_Desc{};
	MonsterHP_Gage_Effect_Desc.pParentWorldMatrix = &m_ResultMatrix;
	MonsterHP_Gage_Effect_Desc.pParent = this;
	MonsterHP_Gage_Effect_Desc.fCurHP = pDesc->fCurHP;
	MonsterHP_Gage_Effect_Desc.fMaxHP = pDesc->fMaxHP;
	MonsterHP_Gage_Effect_Desc.fShieldHP = pDesc->fShieldHP;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Monster_HP_Gage_Effect"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Gage_Effect"), &MonsterHP_Gage_Effect_Desc)))
		return E_FAIL;

	CMonster_HP_Bar_Effect::Monster_HP_Bar_Effect_DESC MonsterHP_Bar_Effect_Desc{};
	MonsterHP_Bar_Effect_Desc.pParentWorldMatrix = &m_ResultMatrix;
	MonsterHP_Bar_Effect_Desc.pParent = this;
	MonsterHP_Bar_Effect_Desc.fCurHP = pDesc->fCurHP;
	MonsterHP_Bar_Effect_Desc.fMaxHP = pDesc->fMaxHP;
	MonsterHP_Bar_Effect_Desc.fShieldHP = pDesc->fShieldHP;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Monster_HP_Bar_Effect"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Bar_Effect"), &MonsterHP_Bar_Effect_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_HP_Bar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_ResultMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CMonster_HP_Bar* CMonster_HP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_HP_Bar* pInstance = new CMonster_HP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_HP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_HP_Bar::Clone(void* pArg)
{
	CMonster_HP_Bar* pInstance = new CMonster_HP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster_HP_Bar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_HP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
