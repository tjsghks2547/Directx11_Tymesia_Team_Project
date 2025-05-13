#include "pch.h"
#include "Projectile_FireBall.h"
#include "GameInstance.h"
#include "Player.h"

CProjectile_FireBall::CProjectile_FireBall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CProjectile(pDevice, pContext)
{
}

CProjectile_FireBall::CProjectile_FireBall(const CProjectile_FireBall& Prototype)
	:CProjectile(Prototype)
{
}

HRESULT CProjectile_FireBall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CProjectile_FireBall::Initialize(void* pArg)
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

	XMStoreFloat4x4(&m_Test, XMMatrixIdentity());

	return S_OK;
}

void CProjectile_FireBall::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	if (m_bIsFire)
		m_pGameInstance->Add_Actor_Scene(m_pActor);
	else
		m_pGameInstance->Sub_Actor_Scene(m_pActor);
}

void CProjectile_FireBall::Update(_float fTimeDelta)
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

void CProjectile_FireBall::Late_Update(_float fTimeDelta)
{
}

HRESULT CProjectile_FireBall::Render()
{
	return S_OK;
}

void CProjectile_FireBall::Set_Projectile_Effect()
{
	m_pGameInstance->Play_Effect_Matrix_With_Socket(EFFECT_NAME::EFFECT_PARTICLE_WORLD_RESEARCHER_PROJECTILE_SMOKE, m_pTransformCom->Get_WorldMatrix_Ptr(), &m_Test);
}

HRESULT CProjectile_FireBall::Ready_Components()
{
	return S_OK;
}

HRESULT CProjectile_FireBall::Bind_ShaderResources()
{
	return S_OK;
}

void CProjectile_FireBall::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER", _pOther->Get_Name()) || !strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
	{
		Set_IsFire(false);
		m_pGameInstance->Sub_Actor_Scene(m_pActor);

		if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
		{
			m_pGameInstance->Play_Sound(TEXT("Researcher_Player_Hit.wav"), CHANNELID::SOUND_MONSTER_WEAPON, 20.f);
		}
	}
}

void CProjectile_FireBall::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CProjectile_FireBall::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CProjectile_FireBall* CProjectile_FireBall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CProjectile_FireBall* pInstance = new CProjectile_FireBall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CProjectile_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CProjectile_FireBall::Clone(void* pArg)
{
	CProjectile_FireBall* pInstance = new CProjectile_FireBall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CProjectile_FireBall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CProjectile_FireBall::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
