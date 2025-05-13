#include "pch.h"
#include "Weapon_GreatSword.h"
#include "HArmorLV2.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Player.h"

CWeapon_GreatSword::CWeapon_GreatSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)\
	:CPartObject(pDevice, pContext)
{
}

CWeapon_GreatSword::CWeapon_GreatSword(const CWeapon_GreatSword& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CWeapon_GreatSword::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_GreatSword::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	WEAPON_GreatSword_DESC* pDesc = static_cast<WEAPON_GreatSword_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_pParentModelCom = pDesc->pParentModel;
	m_iMonster_Attack = pDesc->iAttack;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(_float3{ 0.4f, 0.4f, 0.4f });
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,90.f,1.f });

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

	return S_OK;
}

void CWeapon_GreatSword::Priority_Update(_float fTimeDelta)
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

void CWeapon_GreatSword::Update(_float fTimeDelta)
{
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
		SocketMatrix *  /* 로컬 스페이스 영역 */
		XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
	);
	if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
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

#pragma region Effect
			if (iter.eType == EVENT_EFFECT && iter.isEventActivate == true)  // 여기가 EVENT_EFFECT
			{
				if (!strcmp(iter.szName, "Weapon_Trail")) //Trail 시작해야하는 부분
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Matrix(EFFECT_NAME::EFFECT_SWORD_HARMOR, &m_CombinedWorldMatrix);
					}
				}
				else if (!strcmp(iter.szName, "Effect_Vertical"))
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						_vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
						_vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_VERTICAL_DUST, vPos, vDir);
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_VERTICAL, vPos, vDir);
					}
				}
				else if (!strcmp(iter.szName, "Effect_Narrow"))
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						_vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
						_vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT_STAB, vPos, vDir);
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUST_NARROW, vPos, vDir);
					}
				}
				else if (!strcmp(iter.szName, "Effect_Horizon"))
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						_vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
						_vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUST_HORIZON, vPos, vDir);
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_HORIZON, vPos, vDir);
					}
				}
				else if (!strcmp(iter.szName, "Effect_Horizon_Back"))
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						_vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
						_vector vDir = { m_pParentWorldMatrix->_31 * -1.f, m_pParentWorldMatrix->_32 * -1.f, m_pParentWorldMatrix->_33 * -1.f, 0.f };
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_DUST_HORIZON, vPos, vDir);
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_HORIZON, vPos, vDir);
					}
				}
				else if (!strcmp(iter.szName, "Effect_Left"))
				{
					if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime && false == iter.isPlay)
					{
						_vector vPos = { m_pParentWorldMatrix->_41, m_pParentWorldMatrix->_42, m_pParentWorldMatrix->_43, 1.f };
						_vector vDir = { m_pParentWorldMatrix->_31, m_pParentWorldMatrix->_32, m_pParentWorldMatrix->_33, 0.f };
						iter.isPlay = true;      // 한 번만 재생 되어야 하므로             
						m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT_STAB, vPos, vDir);
					}
				}
			}
			else if (iter.eType == EVENT_EFFECT && iter.isEventActivate == false && true == iter.isPlay)
			{
				if (!strcmp(iter.szName, "Weapon_Trail")) //Trail이 꺼져야 하는 부분
				{
					m_pGameInstance->Stop_Effect(EFFECT_NAME::EFFECT_SWORD_HARMOR);
					iter.isPlay = false;
				}
			}
#pragma endregion

		}
	}
	else
		m_pGameInstance->Sub_Actor_Scene(m_pActor);


	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 10, 0.f,0.f,1.f });
}

void CWeapon_GreatSword::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_GreatSword::Render()
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

HRESULT CWeapon_GreatSword::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_GreatSword"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_GreatSword::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_GreatSword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER", _pOther->Get_Name()))
	{
		m_bColliderOff = true;

		if (static_cast<CPlayer*>(_pOther)->Get_PhaseState() & CPlayer::PHASE_HITTED)
		{
			_uint iRandSoundFileNum = {};
			iRandSoundFileNum = rand() % 4;

			switch (iRandSoundFileNum)
			{
			case 0:
				m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
				break;
			case 1:
				m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
				break;
			case 2:
				m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
				break;
			case 3:
				m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.6f);
				break;
			}
		}
	}
}

void CWeapon_GreatSword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_GreatSword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_GreatSword* CWeapon_GreatSword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_GreatSword* pInstance = new CWeapon_GreatSword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CWeapon_GreatSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_GreatSword::Clone(void* pArg)
{
	CWeapon_GreatSword* pInstance = new CWeapon_GreatSword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CWeapon_GreatSword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_GreatSword::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
