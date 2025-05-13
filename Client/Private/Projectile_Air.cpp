#include "pch.h"
#include "Projectile_Air.h"
#include "GameInstance.h"

CProjectile_Air::CProjectile_Air(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CProjectile(pDevice, pContext)
{
}

CProjectile_Air::CProjectile_Air(const CProjectile_Air& Prototype)
	:CProjectile(Prototype)
{
}

HRESULT CProjectile_Air::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CProjectile_Air::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.3f,0.3f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

	m_pGameInstance->Add_Actor_Scene(m_pActor);

	return S_OK;
}

void CProjectile_Air::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (m_bIsFire)
		m_pGameInstance->Add_Actor_Scene(m_pActor);
	else
		m_pGameInstance->Sub_Actor_Scene(m_pActor);
}

void CProjectile_Air::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
	{
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f, 0.f,1.f });
#pragma region EFFECT
		for (_uint i = 0; i < 3; i++)
			m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_BAT_AIR, m_pTransformCom->Get_WorldMatrix_Ptr());

#pragma endregion
	}
}

void CProjectile_Air::Late_Update(_float fTimeDelta)
{
}

void CProjectile_Air::Set_Projectile_Effect()
{
}

HRESULT CProjectile_Air::Render()
{
	return S_OK;
}

HRESULT CProjectile_Air::Ready_Components()
{
	return S_OK;
}

HRESULT CProjectile_Air::Bind_ShaderResources()
{
	return S_OK;
}

void CProjectile_Air::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER", _pOther->Get_Name()))
	{
		Set_IsFire(false);
		m_pGameInstance->Sub_Actor_Scene(m_pActor);
		for (_uint i = 0; i < 3; i++)
			m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_BAT_AIR);
	}
}

void CProjectile_Air::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile_Air::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CProjectile_Air* CProjectile_Air::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CProjectile_Air* pInstance = new CProjectile_Air(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CProjectile_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CProjectile_Air::Clone(void* pArg)
{
	CProjectile_Air* pInstance = new CProjectile_Air(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CProjectile_Air");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CProjectile_Air::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
