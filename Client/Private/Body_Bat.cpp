#include "pch.h"
#include "Body_Bat.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Camera_Free.h"
#include "Player.h"

CBody_Bat::CBody_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CBody_Bat::CBody_Bat(const CBody_Bat& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CBody_Bat::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Bat::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	CBody_Bat::BODY_BAT_DESC* pDesc = static_cast<CBody_Bat::BODY_BAT_DESC*>(pArg);

	m_pParentState = pDesc->pParentState;
	m_iMonster_Attack = pDesc->iAttack;
	m_bDead = pDesc->bDead;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(20, true);

	m_pSocketMatrix[0] = m_pModelCom->Get_BoneMatrix("ik_hand_r");
	m_pSocketMatrix[1] = m_pModelCom->Get_BoneMatrix("ik_hand_l");
	m_pSocketMatrix[2] = m_pModelCom->Get_BoneMatrix("ik_head");

	m_pActor[COLLIDER_LEFT_HAND] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.5f,1.f,2.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
	m_pActor[COLLIDER_RIGHT_HAND] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.5f,1.f,2.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
	m_pActor[COLLIDER_MOUTH] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.5f,1.f,2.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
	m_pActor[COLLIDER_BODY] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 4.f,1.5f,4.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
	m_pActor[COLLIDER_LARGE] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 7.f,1.5f,7.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
	m_pActor[COLLIDER_WHOLE] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 20.f,1.f,20.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_LEFT_HAND], _fvector{ 0.f,0.f,92.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_RIGHT_HAND], _fvector{ 0.f,0.f,92.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_MOUTH], _fvector{ 0.f,0.f,92.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_BODY], _fvector{ 0.f,0.f,92.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_LARGE], _fvector{ 0.f,0.f,92.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_WHOLE], _fvector{ 0.f,0.f,92.f,1.f });

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::DESTRUCT;

	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_LEFT_HAND], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_RIGHT_HAND], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_MOUTH], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_BODY], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_LARGE], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
	m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_WHOLE], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

	return S_OK;
}

void CBody_Bat::Priority_Update(_float fTimeDelta)
{
	//테스트 용 현재 튜토리얼 맵에서만 소환되므로 LEVEL_TUTORIAL -> 왕실정원이면 저거 LEVEL_RoyalGarden 으로 바꿔야함 ! 
	if (m_pCamera == nullptr)
		m_pCamera = static_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Layer_Camera"), "Camera_Free"));
	
	if (*m_pParentState == STATE_DEAD)
	{
		m_fDeadTimer += fTimeDelta * 1.5f;
		m_fFinishTime += fTimeDelta * 1.5f;
		if (m_fDeadTimer >= 1.5)
		{
			*m_bDead = true;
		}
	}
}

void CBody_Bat::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));

	if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
	{
		for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
		{
			if (iter.isPlay == false)
			{
				if (iter.eType == EVENT_STATE && iter.isEventActivate == true)	
				{
					if (!strncmp(iter.szName, "Camera_Shake", 12))	
					{
						m_pCamera->ShakeOn(600.f, 600.f, 15.f, 15.f);	
					}
				}	


				if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true)
				{
					if (!strncmp(iter.szName, "LeftHand", strlen("LeftHand")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_LEFT_HAND]);
					}
					else if (!strncmp(iter.szName, "RightHand", strlen("RightHand")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_RIGHT_HAND]);
					}
					else if (!strncmp(iter.szName, "Bite", strlen("Bite")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_MOUTH]);
					}
					else if (!strncmp(iter.szName, "Body", strlen("Body")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_BODY]);
					}
					else if (!strncmp(iter.szName, "Large", strlen("Large")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_LARGE]);
					}

					else if (!strncmp(iter.szName, "Whole", strlen("Whole")))
					{
						m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_WHOLE]);
					}
					iter.isPlay = true;
				}
			}
			else
			{
				if ((iter.eType == EVENT_COLLIDER && iter.isEventActivate == false) || m_bColliderOff == true)
				{
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_LEFT_HAND]);
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_RIGHT_HAND]);
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_MOUTH]);
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_BODY]);
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_LARGE]);
					m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_WHOLE]);

					m_bColliderOff = false;
					if (!iter.isEventActivate)
						iter.isPlay = false;
				}
			}
		}
	}
	else
	{
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_LEFT_HAND]);
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_RIGHT_HAND]);
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_MOUTH]);
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_BODY]);
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_LARGE]);
		m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_WHOLE]);
	}

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_LEFT_HAND])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_LEFT_HAND], XMLoadFloat4x4(m_pSocketMatrix[1]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_RIGHT_HAND])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_RIGHT_HAND], XMLoadFloat4x4(m_pSocketMatrix[0]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_MOUTH])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_MOUTH], XMLoadFloat4x4(m_pSocketMatrix[2]) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_BODY])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_BODY], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 0.f,0.f,1.f });

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_LARGE])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_LARGE], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 500.f,0.f,1.f });

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_WHOLE])))
		m_pGameInstance->Update_Collider(m_pActor[COLLIDER_WHOLE], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0, 500.f,0.f,1.f });

}

void CBody_Bat::Late_Update(_float fTimeDelta)
{
	//XMVectorCatmullRom()
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody_Bat::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (*m_pParentState == STATE_DEAD)
		{
			m_iPassNum = 5;
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 6)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
				return E_FAIL;
		}
		else
			m_iPassNum = 0;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네
			return E_FAIL;


		m_pShaderCom->Begin(m_iPassNum);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CBody_Bat::Render_Shadow()
{
	return S_OK;
}

HRESULT CBody_Bat::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Bat_Body"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Bat::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CBody_Bat::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
				m_pGameInstance->Play_Sound(L"Hit1.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
				break;
			case 1:
				m_pGameInstance->Play_Sound(L"Hit2.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
				break;
			case 2:
				m_pGameInstance->Play_Sound(L"Hit3.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
				break;
			case 3:
				m_pGameInstance->Play_Sound(L"Hit4.wav", CHANNELID::SOUND_MONSTER_WEAPON, 0.3f);
				break;
			}
		}
	}
}

void CBody_Bat::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBody_Bat::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBody_Bat* CBody_Bat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Bat* pInstance = new CBody_Bat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBody_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Bat::Clone(void* pArg)
{
	CBody_Bat* pInstance = new CBody_Bat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBody_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Bat::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
