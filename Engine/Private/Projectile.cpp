#include "Projectile.h"
#include "GameInstance.h"

CProjectile::CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CProjectile::CProjectile(const CProjectile& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CProjectile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CProjectile::Initialize(void* pArg)
{
	PROJECTILE_DESC* pDesc = static_cast<PROJECTILE_DESC*>(pArg);

	m_fDelete_Time = *pDesc->fDelete_Time;
	m_iMonster_Attack = pDesc->iDamage;
	pDesc->fScaling = _float3(0.01f, 0.01f, 0.01f);

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	return S_OK;
}

void CProjectile::Priority_Update(_float fTimeDelta)
{
	m_fDelete_Time -= fTimeDelta;
	if (m_fDelete_Time <= 0.f)
	{
		Stop_Projectile_Effect();
		Set_IsFire(false);
		return;
	}
}

void CProjectile::Update(_float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fTimeDelta);

}

void CProjectile::Late_Update(_float fTimeDelta)
{
}

HRESULT CProjectile::Render()
{
	return S_OK;
}

void CProjectile::Reset_Projectile()
{
	m_fDelete_Time = 5.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vStartPos));
}

void CProjectile::Set_Target(_vector vDir)
{
	m_pTransformCom->Look(vDir);
	Set_Projectile_Effect();
}

void CProjectile::Set_Projectile_Effect()
{

}

void CProjectile::Stop_Projectile_Effect()
{
}

void CProjectile::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile::Free()
{
	__super::Free();
	m_pGameInstance->Sub_Actor_Scene(m_pActor);
}
