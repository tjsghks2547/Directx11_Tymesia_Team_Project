#include "pch.h"
#include "Weapon_Shield.h"
#include "GameInstance.h"
#include "Normal_VillageM1.h"
#include "Animation.h"

CWeapon_Shield::CWeapon_Shield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CWeapon_Shield::CWeapon_Shield(const CWeapon_Shield& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CWeapon_Shield::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_Shield::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	WEAPON_SHIELD_DESC* pDesc = static_cast<WEAPON_SHIELD_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_pParentModelCom = pDesc->pParentModel;
	m_iMonster_Attack = pDesc->iAttack;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
	m_pTransformCom->Rotation(XMConvertToRadians(210.f), XMConvertToRadians(-10.f), XMConvertToRadians(90.f));
	_vector vOffSetPos = { 20.f,0.f,-10.f,1.f };
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vOffSetPos);

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,80.f,1.f });

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

	return S_OK;
}

void CWeapon_Shield::Priority_Update(_float fTimeDelta)
{
	if (*m_pParentState == STATE_DEAD)
	{
		m_fDeadTimer += fTimeDelta * 0.5f;
		m_fFinishTime += fTimeDelta * 0.5f;
	}
	if (m_iPreAnimIndex != m_pParentModelCom->Get_Current_Animation_Index())
	{
		m_bColliderOff = false;
		m_iPreAnimIndex = m_pParentModelCom->Get_Current_Animation_Index();
	}

}

void CWeapon_Shield::Update(_float fTimeDelta)
{
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
		SocketMatrix *  /* 로컬 스페이스 영역 */
		XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
	);
	if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD && *m_pParentState == MONSTER_STATE::STATE_PARRY)
	{
		for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
		{
			if (iter.isPlay == false)
			{
				if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true)
				{
					m_pGameInstance->Add_Actor_Scene(m_pActor);
					iter.isPlay = true;
				}
			}
			else
			{
				if ((iter.eType == EVENT_COLLIDER && iter.isEventActivate == false) || m_bColliderOff == true)
				{
					m_pGameInstance->Sub_Actor_Scene(m_pActor);

					m_bColliderOff = false;
					if (!iter.isEventActivate)
						iter.isPlay = false;
				}
			}
		}
	}
	else
		m_pGameInstance->Sub_Actor_Scene(m_pActor);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 10, 300.f,0.f,1.f });

}

void CWeapon_Shield::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CWeapon_Shield::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

		if (*m_pParentState == STATE_DEAD)
		{
			m_iPassNum = 9;
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 6)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
				return E_FAIL;
		}
		else
			m_iPassNum = 17;// 0;


		m_pShaderCom->Begin(m_iPassNum);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CWeapon_Shield::Render_Shadow()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;/*
	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
		return E_FAIL;*/


	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pShaderCom->Begin(2);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CWeapon_Shield::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Shield"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Shield::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Shield::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER", _pOther->Get_Name()))
	{
		m_bColliderOff = true;
		m_pGameInstance->Play_Sound(L"Villager_WoodShieldSound.ogg", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
	}
}

void CWeapon_Shield::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Shield::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Shield* CWeapon_Shield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Shield* pInstance = new CWeapon_Shield(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CWeapon_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Shield::Clone(void* pArg)
{
	CWeapon_Shield* pInstance = new CWeapon_Shield(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CWeapon_Shield");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Shield::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
